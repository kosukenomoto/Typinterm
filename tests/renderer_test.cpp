#include "renderer.hpp"

#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <sstream>

#include "session.hpp"

TEST_CASE("renderer drawはsessionのkeyeventをstring出力") {
  TypingSession session{"test"};
  Renderer render{session};
  char in = 'a';
  std::chrono::steady_clock::time_point t0 = std::chrono::steady_clock::now();
  KeyEvent inkey{in, t0};
  std::ostringstream buffer;
  std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

  SECTION("表示") {}
}
