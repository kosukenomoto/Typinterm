#pragma once
#include <chrono>

struct KeyEvent{
    char ch;
    std::chrono::steady_clock::time_point ts;
};
