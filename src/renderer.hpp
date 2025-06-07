#pragma once
#include <iomanip>
#include <iostream>
#include <string_view>

#include "session.hpp"
class Renderer {
 public:
  explicit Renderer();
  ~Renderer();
  void draw(TypingSession&) const;
  void showcursor() const;
  void hidecursor() const;
  void clr() const;

 private:
  static constexpr std::string_view RED = "\x1b[31m";
  static constexpr std::string_view GREEN = "\x1b[32m";
  static constexpr std::string_view RESET = "\x1b[0m";
  std::string show_typed_line(TypingSession&) const;
  std::string show_lesson_line(TypingSession&) const;
};
