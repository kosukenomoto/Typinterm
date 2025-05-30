#pragma once
#include <termios.h>
#include <unistd.h>

class TerminalGuard{
    termios orig_{};

public:
    TerminalGuard(){
        tcgetattr(STDIN_FILENO, &orig_); // 現在の端末設定を取得

        termios raw = orig_;
        cfmakeraw(&raw);
        raw.c_cc[VMIN] = 0;
        raw.c_cc[VTIME] = 0;

        tcsetattr(STDIN_FILENO,TCSANOW,&raw);
    }
    ~TerminalGuard() {tcsetattr(STDIN_FILENO,TCSANOW,&orig_);}

    TerminalGuard(const TerminalGuard&) = delete;
    TerminalGuard& operator=(const TerminalGuard&) = delete;
};
