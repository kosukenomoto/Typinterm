#include "input.hpp"

#include <unistd.h>

#include <cctype>

InputLoop::InputLoop() : fd_{STDIN_FILENO} {};

std::unique_ptr<InputLoop> InputLoop::create() {
  return std::unique_ptr<InputLoop>{new InputLoop()};
}

std::optional<KeyEvent> InputLoop::poll() {
  char c;
  ssize_t n = ::read(fd_, &c, 1);
  if (n == 1 && ((c == '\n') || (c == '\r') ||
                 !std::iscntrl(static_cast<unsigned char>(c)))) {
    return KeyEvent{c, std::chrono::steady_clock::now()};
  }
  return std::nullopt;
}
