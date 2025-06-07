#pragma once
#include <chrono>
#include <iostream>
#include <string>
#include <vector>

#include "key_event.hpp"

class TypingSession {
 public:
  enum class Phase { Ready, Running, Finished };
  explicit TypingSession(std::string lesson);
  void set_lesson(std::string lesson) { lesson_ = std::move(lesson); }

  // input  event
  bool update(const KeyEvent& ev);  // true: to render

  // time base update
  void tick(std::chrono::steady_clock::time_point now);

  // status for render
  Phase phase() const noexcept { return phase_; }
  const std::vector<KeyEvent>& typed_key() const noexcept {
    return typed_keys_;
  }

  const std::string show_typed_line() const noexcept {
    std::string result_;
    size_t retnum_{0};
    result_.reserve(typed_key().size());
    for (const KeyEvent& c : typed_key()) {
      if (c.c == '\n') {
        retnum_++;
        continue;
      }
      if (retnum_ == typed_line()) {
        result_ += c.c;
      }
    }
    return result_;
  }

  const std::string& lesson_str() const noexcept {
    return lesson_line_[typed_line()];
  }
  const size_t typed_line() const noexcept { return typed_lines_; }

  //---------------------------------------
  size_t cursor() const noexcept { return cursor_; }
  size_t errors() const noexcept { return errors_; }
  double wpm() const noexcept { return wpm_cached_; }

 private:
  // status
  Phase phase_{Phase::Ready};
  std::vector<KeyEvent> typed_keys_;
  std::vector<std::string> lesson_line_;
  std::string lesson_;
  //----------------------------------------

  size_t cursor_{0};
  size_t errors_{0};
  // total sum
  std::chrono::steady_clock::time_point t0_;
  size_t typed_chars_{0};
  size_t typed_lines_{0};
  double wpm_cached_{0.0};
};
