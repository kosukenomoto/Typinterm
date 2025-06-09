#pragma once
#include <unistd.h>

#include <memory>
#include <optional>

#include "key_event.hpp"

class InputLoop {
 public:
  static std::unique_ptr<InputLoop> create();
  std::optional<KeyEvent> poll();

 private:
  int fd_{-1};
  explicit InputLoop();
};
