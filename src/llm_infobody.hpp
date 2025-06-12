#pragma once
#include <string>

// ex1)
// url "http://localhost:11434/api/generate"
// model "deepseek-r1:7b"
// prompt "タイピング練習の素材として100
// wordで構成された英語の文章を作成してください。
// 英字記号や数字もできる限り含むものとしてください。"

struct Llm_infobody {
  std::string url;
  std::string model;
  std::string prompt;
  bool think{false};
  bool stream{false};
};
