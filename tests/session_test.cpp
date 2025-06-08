#include "session.hpp"

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <chrono>

#include "key_event.hpp"

TEST_CASE("生成直後はREADY") {
  TypingSession session{"test"};
  REQUIRE(session.phase() == TypingSession::Phase::Ready);
}

TEST_CASE("update は正常にキーを記録する") {
  TypingSession session{"test"};
  char in = 'a';
  std::chrono::steady_clock::time_point t0 = std::chrono::steady_clock::now();
  KeyEvent inkey{in, t0};

  SECTION("戻り値") { REQUIRE(session.update(inkey)); }
  SECTION("保存内容") {
    session.update(inkey);
    REQUIRE(session.typed_key().size() == 1);
    REQUIRE(session.typed_key().back().ev.c == inkey.c);
    REQUIRE(session.typed_key().back().ev.ts == inkey.ts);
  }
}

TEST_CASE("tick で wpm を計算する") {
  TypingSession session{"test"};
  auto t0 = std::chrono::steady_clock::now();
  session.update(KeyEvent{'t', t0});
  session.update(KeyEvent{'e', t0 + std::chrono::seconds(1)});
  session.tick(t0 + std::chrono::minutes(1));
  REQUIRE(session.wpm() == Catch::Approx(0.4).margin(0.01));
}

TEST_CASE("改行後にカーソルが正しく初期化される") {
  TypingSession session{"ab\nbcd"};
  auto t0 = std::chrono::steady_clock::now();

  session.update(KeyEvent{'a', t0});
  REQUIRE(session.cursor() == 1);
  REQUIRE(session.typed_line() == 0);
  // cmake -Vで表示
  std::cout << session.lesson_line_str().size() << std::endl;
  std::cout << session.lesson_line_str()[session.cursor()] << std::endl;

  session.update(KeyEvent{'b', t0});
  REQUIRE(session.cursor() == 2);
  REQUIRE(session.typed_line() == 0);
  std::cout << session.lesson_line_str().size() << std::endl;
  std::cout << session.lesson_line_str()[session.cursor()] << std::endl;

  session.update(KeyEvent{'\n', t0});
  REQUIRE(session.cursor() == 0);
  REQUIRE(session.typed_line() == 1);
  std::cout << session.lesson_line_str().size() << std::endl;
  std::cout << session.lesson_line_str()[session.cursor()] << std::endl;

  session.update(KeyEvent{'b', t0});
  REQUIRE(session.cursor() == 1);
  REQUIRE(session.typed_line() == 1);
  std::cout << session.lesson_line_str().size() << std::endl;
  std::cout << session.lesson_line_str()[session.cursor()] << std::endl;

  session.update(KeyEvent{'c', t0});
  REQUIRE(session.cursor() == 2);
  REQUIRE(session.typed_line() == 1);
  std::cout << session.lesson_line_str().size() << std::endl;
  std::cout << session.lesson_line_str()[session.cursor()] << std::endl;
}
