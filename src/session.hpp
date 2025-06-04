#pragma once
#include "key_event.hpp"
#include <vector>
#include <string>
#include <chrono>

class TypingSession{
public:
    enum class Phase{ Ready,Running,Finished};
    explicit TypingSession(std::string lesson = "");
    void set_lesson(std::string lesson){ lesson_ = std::move(lesson); }

    // input  event
    bool update(const KeyEvent& ev); //true: to render

    // time base update
    void tick(std::chrono::steady_clock::time_point now);

    // status for render
    Phase phase()   const noexcept { return phase_; }
    const std::vector<KeyEvent>& typed_key() const noexcept{return typed_keys_;}

    //---------------------------------------
    size_t cursor() const noexcept { return cursor_; }
    size_t errors() const noexcept { return errors_; }
    double wpm() const noexcept { return wpm_cached_; }

private:
    //status
    Phase phase_ {Phase::Ready};
    std::vector<KeyEvent> typed_keys_;
    //----------------------------------------
    std::string lesson_;
    size_t cursor_{0};
    size_t errors_{0};

    //total sum
    std::chrono::steady_clock::time_point t0_;
    size_t typed_chars_ {0};
    double wpm_cached_ {0.0};

};
