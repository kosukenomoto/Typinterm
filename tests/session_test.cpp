#include <catch2/catch_test_macros.hpp>
#include "session.hpp"
#include "key_event.hpp"
#include <chrono>
#include <catch2/catch_approx.hpp>

TEST_CASE("生成直後はREADY"){
    TypingSession session{};
    REQUIRE(session.phase()==TypingSession::Phase::Ready);
}

TEST_CASE("update は正常にキーを記録する"){
    TypingSession session{};
    char in = 'a';
    std::chrono::steady_clock::time_point t0 = std::chrono::steady_clock::now();
    KeyEvent inkey{in,t0};

    SECTION("戻り値") { REQUIRE(session.update(inkey)); }
    SECTION("保存内容"){
        session.update(inkey);
        REQUIRE(session.typed_key().size() == 1);
        REQUIRE(session.typed_key().back().c == inkey.c);
        REQUIRE(session.typed_key().back().ts == inkey.ts);
    }
}

TEST_CASE("tick で wpm を計算する"){
    TypingSession session{};
    auto t0 = std::chrono::steady_clock::now();
    session.update(KeyEvent{'a', t0});
    session.update(KeyEvent{'b', t0 + std::chrono::seconds(1)});
    session.tick(t0 + std::chrono::minutes(1));
    REQUIRE(session.wpm() == Catch::Approx(0.4).margin(0.01));
}
