#include "renderer.hpp"

Renderer::Renderer(TypingSession &session) : session_{session} {
  Renderer::clr();
}

Renderer::~Renderer() { Renderer::showcursor(); }

void Renderer::draw() const {
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
  std::cout << session_.cursor() << " " << session_.lesson_line_str_size()
            << std::flush;
  std::cout << "\033[" << 2 << ";" << 1 << "H" << std::flush;
  std::cout << show_lesson_line() << std::flush;
  std::cout << "\033[" << 3 << ";" << 1 << "H" << std::flush;
  std::cout << show_typed_line() << std::flush;
}

void Renderer::hidecursor() const { std::cout << "\033[?25l" << std::flush; }
void Renderer::showcursor() const { std::cout << "\033[?25h" << std::flush; }
void Renderer::clr() const { std::cout << "\033[2J" << std::flush; }

std::string Renderer::show_lesson_line() const {
  return session_.lesson_lines()[session_.typed_line()] + "⏎";
}

size_t Renderer::show_lesson_line_num() const {
  return session_.lesson_lines().size();
}

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
