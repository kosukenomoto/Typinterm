#include "session.hpp"

TypingSession::TypingSession(std::string lesson) : lesson_{std::move(lesson)} {
  std::istringstream iss{lesson_};
  std::string line;

  while (std::getline(iss, line, '\n')) {
    lesson_lines_.push_back(line);
  }
}

bool TypingSession::update(const KeyEvent& ev) {
  if (phase_ == Phase::Ready) {
    phase_ = Phase::Running;
    t0_ = ev.ts;
  }

  if (cursor_ <= lesson_line_str().size()) {
    if (lesson_line_str()[cursor_] != ev.c) {
      // 誤入力エラー
      errors_++;
      typed_keys_.push_back(KeyEventPool{ev, true});
    } else {
      // 正常入力しかwpmカウントしない
      wpm_typed_chars_++;
      typed_keys_.push_back(KeyEventPool{ev, false});
    }
  } else {
    // レッスン中の文字の長さを超えて入力されたらエラー判定
    errors_++;
    typed_keys_.push_back(KeyEventPool{ev, true});
  }

  tick(ev.ts);

  // 桁位置加算
  cursor_++;

  if (ev.c == '\n') {
    cursor_ = 0;  // 桁位置初期化
    typed_lines_++;
  }

  alltypecount_++;
  corect_update();

  // sizeだから１始まり　typedlinesは0始まり
  if ((typed_lines_ + 1) > lesson_lines().size()) {
    phase_ = Phase::Finished;
  }

  return true;
}

void TypingSession::corect_update() {
  corect_cached_ = alltypecount_ == 0
                       ? 0.0
                       : static_cast<double>(alltypecount_ - errors_) /
                             alltypecount_ * 100.0;
}

void TypingSession::tick(std::chrono::steady_clock::time_point now) {
  if (phase_ != Phase::Running) return;
  auto elapsed =
      std::chrono::duration_cast<std::chrono::milliseconds>(now - t0_).count();
  if (elapsed <= 0) {
    wpm_cached_ = 0.0;
    return;
  }
  double minutes = elapsed / 60000.0;
  wpm_cached_ = (wpm_typed_chars_ / 5.0) / minutes;
}
