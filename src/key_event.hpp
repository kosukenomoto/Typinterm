#pragma once
#include <chrono>

struct KeyEvent{
   // wchar_t wc;
   char c;
   std::chrono::steady_clock::time_point ts;
};
