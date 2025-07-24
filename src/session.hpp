#pragma once
#include <chrono>
#include <string>
#include <vector>

#include "key_event.hpp"
#include "key_event_pool.hpp"

class TypingSession {
 public:
  enum class Phase { Ready, Running, Finished };
  explicit TypingSession(std::string lesson);
  void set_lesson(std::string lesson) { lesson_ = std::move(lesson); }

  // input  event
  bool update(const KeyEvent& ev);  // true: to render

  // time base update
  void tick(std::chrono::steady_clock::time_point now);
  // error/cusor base update
  void corect_update();

  // status for render
  Phase phase() const noexcept { return phase_; }
  const std::vector<KeyEventPool>& typed_key() const noexcept {
    return typed_keys_;
  }

  // レッスン全量
  const std::string& lesson_str() const noexcept { return lesson_; }
  // レッスン全量vec（コンストラクタで生成される）
  const std::vector<std::string>& lesson_lines() const noexcept {
    return lesson_lines_;
  }
  // タイプ中のライン数
  const size_t typed_line() const noexcept { return typed_lines_; }
  // タイプ中のレッスンライン

  const std::string& lesson_line_str() const noexcept {
    return lesson_lines_[typed_lines_];
  }
  // タイプ中のレッスンライン中の字数
  size_t lesson_line_str_size() const noexcept {
    return lesson_lines_[typed_lines_].size();
  }

  //---------------------------------------
  size_t cursor() const noexcept { return cursor_; }
  size_t errors() const noexcept { return errors_; }
  double wpm() const noexcept { return wpm_cached_; }
  double correct() const noexcept { return corect_cached_; }

 private:
  // status
  Phase phase_{Phase::Ready};
  std::vector<KeyEventPool> typed_keys_;

  std::string lesson_;
  std::vector<std::string> lesson_lines_;
  //----------------------------------------

  size_t cursor_{0};

  size_t alltypecount_{0};
  size_t errors_{0};
  // total sum
  std::chrono::steady_clock::time_point t0_;
  size_t wpm_typed_chars_{0};
  size_t typed_lines_{0};
  double corect_cached_{0.0};
  double wpm_cached_{0.0};
};
