
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>
class Timer {
  int id_;
  std::chrono::steady_clock::time_point last_;
  explicit Timer(int id) : id_{id}, last_{std::chrono::steady_clock::now()} {}

 public:
  static std::unique_ptr<Timer> create(int id) {
    return std::unique_ptr<Timer>(new Timer{id});
  }
  int elapsedMilliseconds() const {
    auto now = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(now - last_)
        .count();
  }
};
