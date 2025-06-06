#include <termios.h>
#include <unistd.h>

#include <iostream>
#include <memory>
#include <numeric>
#include <ranges>
#include <string>
#include <vector>

class aline {
  std::string aline_;
  int aline_num_;

 public:
  explicit aline(int n) : aline_num_{n}, aline_{""} {};
  void appendChar(char x) { aline_.push_back(x); }
  void deleteLastChar(void) { aline_.pop_back(); }
  std::string showLine(void) const { return aline_; }
};

class linectler {
  std::vector<aline> lines_past_;
  int cur_linenum_;
  explicit linectler() : cur_linenum_{0} {}

 public:
  static std::unique_ptr<linectler> create(void) {
    return std::unique_ptr<linectler>(new linectler());
  }
  aline& newline() {
    lines_past_.emplace_back(cur_linenum_++);
    return lines_past_.back();
  }
  std::string showline0() { return lines_past_[0].showLine(); }
  std::string alllines() {
    auto view = lines_past_ | std::views::transform(
                                  [](const aline& a) { return a.showLine(); });
    return std::accumulate(
        view.begin(), view.end(), std::string{},
        [](std::string acc, const std::string& s) { return acc + s + '\n'; });
  }
};

void setBufferedInput(bool enable) {
  static termios oldt;
  static bool initialized = false;

  if (!initialized) {
    tcgetattr(STDIN_FILENO, &oldt);  // 現在の端末設定を取得
    initialized = true;
  }

  termios newt = oldt;
  if (!enable) {
    newt.c_lflag &= ~(ICANON | ECHO);  // カノニカル・エコー無効
  } else {
    newt.c_lflag |= (ICANON | ECHO);
  }
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);  // 設定を即時適用
}

int main() {
  setBufferedInput(false);  // バッファリング無効

  std::cout << "1文字ずつ入力を検出します（qで終了）:\n";
  char c;
  std::unique_ptr lines = linectler::create();
  aline* curline = &(lines->newline());
  //  aline& curline = lines->newline();
  //  ここは参照になっているが、、、
  while (true) {
    c = getchar();  // エンターなしで即取得
    std::cout << c;
    if (c == '\n') {
      curline = &(lines->newline());
      //          curline = lines->newline();
      //          個々は参照ではなくコピー。curlineはvectorが再生成される前のnewlineを指しているが、
      //          newlineが動くことで再生成されて参照先が古くなりリンクが壊れる。。。
      //          c++では参照は一度設定すると更新できない。
      continue;
    }
    if (c == 'q') break;
    curline->appendChar(c);
    //      curline.appendChar(c);
  }
  setBufferedInput(true);  // 元に戻す
  std::cout << "#######################\n" << std::endl;
  std::cout << lines->alllines() << std::endl;
  return 0;
}
