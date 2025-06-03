#include "session.hpp"
TypingSession::TypingSession(){};
bool TypingSession::update(const KeyEvent& ev){
    typed_keys_.push_back(ev);
    return true;
}
