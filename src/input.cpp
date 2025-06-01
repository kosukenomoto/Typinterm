#pragma once
#include "key_event.hpp"
#include <optional>
#include <unistd.h>
#include <iostream>

class InputLoop{
    int fd_;
//    std::vector<char> buf_;
//    std::vector<char> ring_;          // 入力リングバッファ
//    mbstate_t st_{};                  // マルチバイト状態
//    explicit InputLoop():fd_{STDIN_FILENO},buf_{std::vector<char>(4096)}{};
    explicit InputLoop():fd_{STDIN_FILENO}{};
public:
    static std::unique_ptr<InputLoop> create(){
//        setlocale(LC_CTYPE, "");         // 現在のロケール (UTF-8 前提)
        return std::unique_ptr<InputLoop> {new InputLoop()};
    }
    std::optional<KeyEvent> poll()
    {
        char c;
        ssize_t n = ::read(fd_,&c,1);
        if(n==1){
            return KeyEvent{c, std::chrono::steady_clock::now()};
        }
        return std::nullopt;
    }
};
//        char c;
//        ssize_t n = read(fd_, buf_.data(), buf_.size());
//        // 変換できるぶんだけループ
//        const char* p = ring_.data();
//        size_t remaining = ring_.size();
//
//        while(remaining){
//            wchar_t wc;
//            ssize_t used = mbrtowc(&wc,p,remaining,&st_);
//            if (used==(ssize_t)-2) break;
//            if (used==(ssize_t)-1) {
//                std::cerr << "invalid sequence\n";
//                used = 1;
//                std::memset(&st_, 0, sizeof st_);
//            }else if(used==0){
//                used = 1;
//            }else{
//                if (wc == L'q') return std::nullopt;   // Enter で終了
//                std::wcout << L"[" << wc << L"]" << std::flush;
//            }
