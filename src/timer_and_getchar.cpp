#include <termios.h>
#include <unistd.h>

#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

class Timer {
  int id_;
  //    std::chrono::_V2::steady_clock::time_point last_;
  std::chrono::steady_clock::time_point last_;
  //    Timer(int id):id_{id},last_{std::chrono::steady_clock::now()}{}
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

void setRawMode(bool enable) {
  static termios oldt;
  static bool initialized = false;

  if (!initialized) {
    tcgetattr(STDIN_FILENO, &oldt);
    initialized = true;
  }

  if (enable) {
    termios newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    newt.c_cc[VMIN] = 1;
    newt.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  } else {
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  }
}

int main() {
  setRawMode(true);

  std::vector<char> buffer;
  char c;

  std::cout << "文字を入力してください（Enterで確定）:\n";
  auto t = Timer::create(1);

  while ((c = getchar()) != '\n') {
    std::cout << c;       // 表示だけ（ECHOを無効にしてるため）
    buffer.push_back(c);  // vector に追加
  }
  std::cout << '\n';
  std::cout << "total: " << t->elapsedMilliseconds() << std::flush;

  std::string result(buffer.begin(), buffer.end());  // vector → string 変換

  setRawMode(false);

  std::cout << "\n入力文字列: " << result << '\n';
  return 0;
}
