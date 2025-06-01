#include <catch2/catch_test_macros.hpp>
#include "session.hpp"

TEST_CASE("生成直後はREADY"){
    TypingSession session{};
    REQUIRE(session.phase()==TypingSession::Phase::Ready);
}
