#pragma once
#include <unistd.h>

#include <iostream>
#include <optional>

#include "key_event.hpp"

class InputLoop {
  int fd_;
  explicit InputLoop() : fd_{STDIN_FILENO} {};

 public:
  static std::unique_ptr<InputLoop> create() {
    return std::unique_ptr<InputLoop>{new InputLoop()};
  }
  std::optional<KeyEvent> poll() {
    char c;
    ssize_t n = ::read(fd_, &c, 1);
    //    if (n == 1 && (c != '\t' && c != '\b')) {
    if (n == 1 && ((c == '\n') || (c == '\r') ||
                   !std::iscntrl(static_cast<unsigned char>(c)))) {
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
