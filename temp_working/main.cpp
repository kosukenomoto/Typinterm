#include "Inputs.cpp"
#include "Timer.cpp"
#include <termios.h>

void setBufferedInput(bool enable) {
    static termios oldt;
    static bool initialized = false;

    if (!initialized) {
        tcgetattr(STDIN_FILENO, &oldt); // 現在の端末設定を取得
        initialized = true;
    }

    termios newt = oldt;
    if (!enable) {
        newt.c_lflag &= ~(ICANON | ECHO); // カノニカル・エコー無効
    } else {
        newt.c_lflag |= (ICANON | ECHO);
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &newt); // 設定を即時適用
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
        curline->appendChar(c,t->elapsedMilliseconds());
//      curline.appendChar(c);
    }
    setBufferedInput(true);   // 元に戻す
    std::cout<<"#######################\n"<<std::endl;
    std::cout<<lines->alllines()<<std::endl;
    return 0;
}
