#include <chrono>
#include <thread>

#include "filereader.cpp"
#include "input.hpp"
#include "key_event.hpp"
#include "llm_exerciseMaterial.hpp"
#include "llm_infobody.hpp"
#include "renderer.hpp"
#include "session.hpp"
#include "terminal.hpp"

namespace {

constexpr int kFps = 10;
constexpr auto kFrameDuration = std::chrono::nanoseconds{1'000'000'000 / kFps};
using clock = std::chrono::steady_clock;

}  // namespace

int main(int argc, char* argv[]) {
  std::string filetext;

  if (argc > 2) {
    std::cerr << "usage: " << argv[0] << " <file>\n";
    return 1;
  }
  if (argc == 2) {
    Filereader filereader{argv[1]};
    try {
      filetext = filereader.read_file_skip_blanks();
    } catch (const std::exception& e) {
      std::cerr << e.what() << '\n';
      return 1;
    }
  }
  if (argc == 1) {
    Llm_infobody llmin{
        "http://localhost:11434/api/generate", "deepseek-r1:7b",
        R"(Please create an English passage consisting of 100 words for typing practice. Include as many letters, punctuation marks, and numbers as possible. A technical topic related to computer science is preferred.)"};
    ExerciseMaterial exmat{llmin};
    if (std::optional<std::string> text = exmat.material()) {
      filetext = *text;
    }
  }
  TerminalGuard term;

  // session creaste
  TypingSession session(filetext);

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
