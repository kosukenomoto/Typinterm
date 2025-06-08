#pragma once
#include <chrono>
#include <iomanip>
#include <iostream>
#include <string_view>

#include "session.hpp"
class Renderer {
 public:
  explicit Renderer(TypingSession &);
  ~Renderer();
  void draw() const;
  void showcursor() const;
  void hidecursor() const;
  void clr() const;

 private:
  static constexpr std::string_view RED = "\x1b[37;41m";
  static constexpr std::string_view GREEN = "\x1b[32m";
  static constexpr std::string_view RESET = "\x1b[0m";
  std::string show_typed_line() const;
  std::string show_lesson_line() const;
  size_t show_lesson_line_num() const;
  TypingSession &session_;
};
