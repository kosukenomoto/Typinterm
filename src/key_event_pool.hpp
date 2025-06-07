#pragma once
#include "key_event.hpp"

struct KeyEventPool {
  // wchar_t wc;
  struct KeyEvent ev;
  bool error;
};
