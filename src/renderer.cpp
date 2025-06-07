#include "renderer.hpp"

Renderer::Renderer(TypingSession &session) : session_{session} {
  Renderer::clr();
  std::istringstream iss{session_.lesson_str()};
  std::string line;

  while (std::getline(iss, line, '\n')) {
    lesson_line_.push_back(line);
  }
  // Renderer::hidecursor();
}

Renderer::~Renderer() { Renderer::showcursor(); }

void Renderer::draw() const {
  // std::string result;
  // result.reserve(session_.typed_key().size());
  // for (const KeyEvent &c : session_.typed_key()) {
  //   result += c.c;
  // }
  clr();
  session_.tick(std::chrono::steady_clock::now());
  std::cout << "\033[" << 1 << ";" << 1 << "H" << std::flush;
  std::cout << "\033[30;43m" << " TypinTerm " << "\033[0m" << std::flush;
  std::cout << "\033[30;44m" << " WPM: " << std::fixed << std::setprecision(2)
            << session_.wpm() << " \033[0m" << std::flush;
  std::cout << "\033[30;45m" << " Correct%: " << std::fixed
            << std::setprecision(2) << session_.correct() << " \033[0m"
            << std::flush;
  std::cout << "\033[30;46m" << " Line: " << session_.typed_line() + 1 << " of "
            << show_lesson_line_num() << " \033[0m" << std::flush;
  std::cout << "\033[" << 2 << ";" << 1 << "H" << std::flush;
  std::cout << show_lesson_line() << std::flush;
  std::cout << "\033[" << 3 << ";" << 1 << "H" << std::flush;
  std::cout << show_typed_line() << std::flush;
}

void Renderer::hidecursor() const { std::cout << "\033[?25l" << std::flush; }
void Renderer::showcursor() const { std::cout << "\033[?25h" << std::flush; }
void Renderer::clr() const { std::cout << "\033[2J" << std::flush; }

std::string Renderer::show_lesson_line() const {
  return lesson_line_[session_.typed_line()] + "⏎";
}

size_t Renderer::show_lesson_line_num() const { return lesson_line_.size(); }

std::string Renderer::show_typed_line() const {
  std::string result_;
  size_t retnum_{0};
  result_.reserve(session_.typed_key().size());
  for (const KeyEventPool &c : session_.typed_key()) {
    if (c.ev.c == '\n') {
      retnum_++;
      continue;
    }
    if (retnum_ == session_.typed_line()) {
      if (c.error) result_ += RED;
      result_ += c.ev.c;
      if (c.error) result_ += RESET;
    }
  }
  return result_;
}
