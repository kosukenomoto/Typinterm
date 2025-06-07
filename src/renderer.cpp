#include "renderer.hpp"

Renderer::Renderer() {
  Renderer::clr();
  // Renderer::hidecursor();
}
Renderer::~Renderer() { Renderer::showcursor(); }

void Renderer::draw(TypingSession &session) const {
  // std::string result;
  // result.reserve(session.typed_key().size());
  // for (const KeyEvent &c : session.typed_key()) {
  //   result += c.c;
  // }
  clr();
  std::cout << "\033[" << 1 << ";" << 1 << "H" << std::flush;
  std::cout << "\033[30;43m" << " TypinTerm " << "\033[0m" << std::flush;
  std::cout << "\033[30;44m" << " WPM: " << std::fixed << std::setprecision(2)
            << session.wpm() << " \033[0m" << std::flush;
  std::cout << "\033[30;45m" << " Correct%: " << std::fixed
            << std::setprecision(2) << session.correct() << " \033[0m"
            << std::flush;
  std::cout << "\033[" << 2 << ";" << 1 << "H" << std::flush;
  std::cout << show_lesson_line(session) << std::flush;
  std::cout << "\033[" << 3 << ";" << 1 << "H" << std::flush;
  std::cout << show_typed_line(session) << std::flush;
}

void Renderer::hidecursor() const { std::cout << "\033[?25l" << std::flush; }
void Renderer::showcursor() const { std::cout << "\033[?25h" << std::flush; }
void Renderer::clr() const { std::cout << "\033[2J" << std::flush; }

std::string Renderer::show_lesson_line(TypingSession &session) const {
  std::vector<std::string> lesson_line_;
  std::istringstream iss{session.lesson_str()};
  std::string line;
  while (std::getline(iss, line, '\n')) {
    lesson_line_.push_back(line);
  }
  return lesson_line_[session.typed_line()];
}

std::string Renderer::show_typed_line(TypingSession &session) const {
  std::string result_;
  size_t retnum_{0};
  result_.reserve(session.typed_key().size());
  for (const KeyEventPool &c : session.typed_key()) {
    if (c.ev.c == '\n') {
      retnum_++;
      continue;
    }
    if (retnum_ == session.typed_line()) {
      if (c.error) result_ += RED;
      result_ += c.ev.c;
      if (c.error) result_ += RESET;
    }
  }
  return result_;
}
