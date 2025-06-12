
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <chrono>

#include "llm_exerciseMaterial.hpp"

TEST_CASE("llmtest001") {
  Llm_infobody llmin{"http://localhost:11434/api/generate", "deepseek-r1:7b",
                     "タイピング練習の素材として100wordで\
      構成された英語の文章を作成してください。\
      英字記号や数字もできる限り含むものとしてください。"};
  ExerciseMaterial exmat{llmin};
}
