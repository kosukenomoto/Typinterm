#pragma once
#include "session.hpp"

class Renderer{
public:
    explicit Renderer();
    ~Renderer();
    void draw(TypingSession&) const;
    void showcursor() const;
    void hidecursor() const;
    void clr() const;
private:

};
