#include <chrono>
#include <ncpp/NotCurses.hh>
#include <thread>

#include "Contents.cpp"
#include "Inputs.cpp"
#include "Timer.cpp"
#include "input.cpp"
#include "key_event.hpp"
#include "renderer.hpp"
#include "session.hpp"
#include "terminal.hpp"

namespace {

constexpr int kFps = 10;
constexpr auto kFrameDuration = std::chrono::nanoseconds{1'000'000'000 / kFps};
using clock = std::chrono::steady_clock;

}  // namespace

int main() {
  TerminalGuard term;

  // session creaste
  TypingSession session("This test was very cool.\nand Line2\nand Line3");

  std::unique_ptr<InputLoop> inputloop = InputLoop::create();
  Renderer render{session};

  while (session.phase() != TypingSession::Phase::Finished) {
    auto next_frame_time = clock::now();

    if (std::optional<KeyEvent> ev = inputloop->poll()) {
      session.update(*ev);
    }
    if (session.phase() != TypingSession::Phase::Finished) {
      render.draw();
    }

    next_frame_time += kFrameDuration;
    std::this_thread::sleep_until(next_frame_time);
  }
  return 0;
}
