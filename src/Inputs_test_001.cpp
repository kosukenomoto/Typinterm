#include <termios.h>
#include <unistd.h>

#include <chrono>
#include <iostream>
#include <memory>
#include <numeric>
#include <ranges>
#include <string>
#include <thread>
#include <vector>

class Timer {
  int id_;
  std::chrono::steady_clock::time_point last_;
  explicit Timer(int id) : id_{id}, last_{std::chrono::steady_clock::now()} {}

 public:
  static std::unique_ptr<Timer> create(int id) {
    return std::unique_ptr<Timer>(new Timer{id});
  }
  //    int getDiffMSeconds(){
  int elapsedMilliseconds() const {
    auto now = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(now - last_)
        .count();
  }
};

class TimeChar {
  char ch_;
  int in_milliseconds_;

 public:
  explicit TimeChar(char x, int in_mil) : ch_{x}, in_milliseconds_{in_mil} {}
  char getChar() const { return ch_; }
  int getTime() const { return in_milliseconds_; }
};

class Aline {
  std::vector<TimeChar> aline_;
  int aline_num_;

 public:
  explicit Aline(int n) : aline_num_{n} {}
  void appendChar(char c) {
    //        aline_.push_back(new TimeChar{c,0});
    //      上記ではnewしたポインタを返すので型不一致。下記と書く。
    aline_.emplace_back(c, 0);
  }
  void appendChar(char c, int t) { aline_.emplace_back(c, t); }
  void appendTimeChar(TimeChar tm) { aline_.push_back(tm); }
  void deleteLastTimeChar(void) { aline_.pop_back(); }
  std::string showLine(void) const {
    std::string result;
    result.reserve(aline_.size());
    for (const TimeChar& c : aline_) {
      result += c.getChar();
    }
    return result;
  }
  // TODO 先頭と最後のTimeCharのmilsec差
  int elaplsedLineInput() {
    return aline_.back().getTime() - aline_.front().getTime();
  }
  // TODO in_milliseconds_をキーにしてcharをゲットするメソッドを設ける
};

// class Aline{
//     std::string aline_;
//     int aline_num_;
// public:
//     explicit Aline(int n):aline_num_{n},aline_{""}{};
//     void appendChar(char x){aline_.push_back(x);}
//     void deleteLastChar(void){aline_.pop_back();}
//     std::string showLine(void) const {return aline_;}
// };

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
  std::unique_ptr lines = LineCtler::create();
  Aline* curline = &(lines->newline());
  //  aline& curline = lines->newline();
  //  ここは参照になっているが、、、

  auto t = Timer::create(1);
  while (true) {
    c = getchar();  // エンターなしで即取得
    std::cout << c;
    if (c == '\n') {
      std::cout << curline->elaplsedLineInput() << std::endl;
      curline = &(lines->newline());
      //          curline = lines->newline();
      //          個々は参照ではなくコピー。curlineはvectorが再生成される前のnewlineを指しているが、
      //          newlineが動くことで再生成されて参照先が古くなりリンクが壊れる。。。
      //          c++では参照は一度設定すると更新できない。
      continue;
    }
    if (c == 'q') break;
    curline->appendChar(c, t->elapsedMilliseconds());
    //      curline.appendChar(c);
  }
  setBufferedInput(true);  // 元に戻す
  std::cout << "#######################\n" << std::endl;
  std::cout << lines->alllines() << std::endl;
  return 0;
}
