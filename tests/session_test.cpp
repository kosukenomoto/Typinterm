#define CATCH_CONFIG_MAIN
#include <catch2/catch_ll.hpp>
#include "../temp_working/session.hpp"

TypingSession session{};

TEST_CASE("生成直後はREADY"){
    REQUIRE(session.phase()==Phase.);
}
