#include <iostream>
#include "renderer.hpp"
#include "session.hpp"

Renderer::Renderer(){
    Renderer::clr();
    Renderer::hidecursor();
};
Renderer::~Renderer(){
    Renderer::showcursor();
};

void Renderer::hidecursor()const {std::cout << "\033[?25l"<<std::flush;};
void Renderer::showcursor()const {std::cout << "\033[?25h"<<std::flush;};
void Renderer::clr()const {std::cout << "\033[2J"<<std::flush;};

void Renderer::draw(TypingSession& session) const {
    std::string result;
    result.reserve(session.typed_key().size());
    for(const KeyEvent& c : session.typed_key()){
        result += c.c;
    }
    std::cout << "\033[" << 1 << ";" << 1 << "H" << std::flush;
    std::cout << result  << std::flush;
}
