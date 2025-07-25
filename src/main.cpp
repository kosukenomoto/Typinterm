#include <chrono>
#include <thread>

#include "filereader.cpp"
#include "input.hpp"
#include "key_event.hpp"
#include "llm_exerciseMaterial.hpp"
#include "llm_infobody.hpp"
#include "renderer.hpp"
#include "savematerial.hpp"
#include "session.hpp"
#include "terminal.hpp"
namespace {

constexpr int kFps = 120;
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
    const std::string typing_prompt =
        R"(Please create an English passage of exactly 100 words for typing practice on a technical topic(about c++) related to computer science.
- Include every category of printable 1-byte ASCII characters:
  - Letters A-Z and a-z
  - Digits 0-9
  - A wide variety of punctuation and symbols, e.g. ! @ # $ % ^ & * ( ) _ + - = { } [ ] | : ; " ' < > , . ? / \ ` ~
- Use only printable 1-byte ASCII characters; do not use any Unicode or multibyte characters (no accented letters, curly quotes, emojis, etc.).
- Keep the passage exactly 100 words (no more, no less).)";

    Llm_infobody llmin{"http://localhost:11434/api/generate", "deepseek-r1:7b",
                       typing_prompt};
    ExerciseMaterial exmat{llmin};
    if (std::optional<std::string> text = exmat.material()) {
      filetext = *text;
    }
  }
  {
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
  }

  if (argc == 1) {
    SaveMaterial smaterial{filetext};
    smaterial.save();
  }

  return 0;
}
