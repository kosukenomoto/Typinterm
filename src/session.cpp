#include "session.hpp"

TypingSession::TypingSession(std::string lesson) : lesson_{std::move(lesson)} {
  std::istringstream iss{lesson_};
  std::string line;
  while (std::getline(iss, line, '\n')) {
    lesson_line_.push_back(line);
  }
}

bool TypingSession::update(const KeyEvent& ev) {
  if (phase_ == Phase::Ready) {
    phase_ = Phase::Running;
    t0_ = ev.ts;
  }

  typed_keys_.push_back(ev);
  typed_chars_++;

  if (ev.c == '\n') {
    typed_lines_++;
  }

  if (cursor_ < lesson_.size()) {
    if (lesson_[cursor_] != ev.c) {
      errors_++;
    }
  }
  cursor_++;

  if (cursor_ >= lesson_.size() && !lesson_.empty()) {
    phase_ = Phase::Finished;
  }

  return true;
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
  wpm_cached_ = (typed_chars_ / 5.0) / minutes;
}
