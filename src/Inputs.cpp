#include <iostream>
#include <memory>
#include <numeric>
#include <ranges>
#include <string>
#include <vector>

#include "key_event.hpp"

class Aline {
  std::vector<KeyEvent> aline_;
  int aline_num_;

 public:
  explicit Aline(int n) : aline_num_{n} {}
  void appendTimeChar(KeyEvent ev) { aline_.push_back(ev); }
  void deleteLastTimeChar(void) { aline_.pop_back(); }
  std::string showLine(void) const {
    std::string result;
    result.reserve(aline_.size());
    for (const KeyEvent& c : aline_) {
      result += c.c;
    }
    return result;
  }
  // 先頭と最後のTimeCharのmilsec差 //TODO １文字の場合差異が0
  std::chrono::milliseconds elaplsedLineInput() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        aline_.back().ts - aline_.front().ts);
  }
  // TODO in_milliseconds_をキーにしてcharをゲットするメソッドを設ける
};

class LineCtler {
  std::vector<Aline> lines_past_;
  int cur_linenum_;
  explicit LineCtler() : cur_linenum_{0} {}

 public:
  static std::unique_ptr<LineCtler> create(void) {
    return std::unique_ptr<LineCtler>(new LineCtler());
  }
  Aline& newline() {
    lines_past_.emplace_back(cur_linenum_++);
    return lines_past_.back();
  }
  std::string showline0() { return lines_past_[0].showLine(); }
  std::string alllines() {
    auto view = lines_past_ | std::views::transform(
                                  [](const Aline& a) { return a.showLine(); });
    return std::accumulate(
        view.begin(), view.end(), std::string{},
        [](std::string acc, const std::string& s) { return acc + s + '\n'; });
  }
  int cur_linenum(void) { return cur_linenum_; }
};
