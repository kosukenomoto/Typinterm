#pragma once
#include <unistd.h>

#include <iostream>
#include <optional>

#include "key_event.hpp"

class InputLoop {
  int fd_;
  explicit InputLoop() : fd_{STDIN_FILENO} {};

 public:
  static std::unique_ptr<InputLoop> create() {
    return std::unique_ptr<InputLoop>{new InputLoop()};
  }
  std::optional<KeyEvent> poll() {
    char c;
    ssize_t n = ::read(fd_, &c, 1);
    //    if (n == 1 && (c != '\t' && c != '\b')) {
    if (n == 1 && ((c == '\n') || (c == '\r') ||
                   !std::iscntrl(static_cast<unsigned char>(c)))) {
      return KeyEvent{c, std::chrono::steady_clock::now()};
    }
    return std::nullopt;
  }
};
