#include "llm_exerciseMaterial.hpp"

#include <cpr/cpr.h>

#include <nlohmann/json.hpp>

#include "llm_infobody.hpp"

ExerciseMaterial::ExerciseMaterial(struct Llm_infobody& llmin)
    : body_{{"model", llmin.model},
            {"prompt", llmin.prompt},
            {"think", llmin.think},
            {"stream", llmin.stream}},
      url_{llmin.url} {};

std::optional<std::string> ExerciseMaterial::material() const {
  auto res = cpr::Post(cpr::Url{url_},
                       cpr::Header{{"Content-Type", "application/json"}},
                       cpr::Body{body_.dump()});

  if (res.status_code == 200) {
    auto json_res = nlohmann::json::parse(res.text);
    auto raw = json_res["response"].get<std::string>();
    return editedMaterial(raw);
  } else {
    return res.text;
  }
}

std::string ExerciseMaterial::editedMaterial(const std::string& src,
                                             std::size_t witdh) const {
  std::istringstream in{src};
  std::ostringstream out;

  std::string word;
  std::size_t line_len = 0;

  bool start_output = false;

  while (in >> word)  // 1 単語ずつ読み取り
  {
    if (!start_output) {
      if (word == "</think>") {
        start_output = true;  // トリガーを見つけた
      }
      continue;  // まだなら次の単語へ
    }
    // 先頭以外はスペース 1 つ分余計に必要
    const bool first_word_in_line = (line_len == 0);
    std::size_t needed = word.size() + (first_word_in_line ? 0 : 1);

    // 追加したら幅を超えるなら改行
    if (line_len + needed > witdh) {
      out << '\n';
      line_len = 0;
      needed = word.size();  // 行頭なのでスペースは不要
    }

    // スペース挿入（行頭でなければ）
    if (!first_word_in_line && line_len != 0) {
      out << ' ';
    }

    out << word;
    line_len += needed;
  }
  return out.str();
}
