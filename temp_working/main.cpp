#include "Inputs.cpp"
#include "Timer.cpp"
#include "Contents.cpp"
#include "terminal.hpp"

//void setBufferedInput(bool enable) {
//    static termios oldt;
//    static bool initialized = false;
//
//    if (!initialized) {
//        tcgetattr(STDIN_FILENO, &oldt); // 現在の端末設定を取得
//        initialized = true;
//    }
//
//    termios newt = oldt;
//    if (!enable) {
//        newt.c_lflag &= ~(ICANON | ECHO); // カノニカル・エコー無効
//    } else {
//        newt.c_lflag |= (ICANON | ECHO);
//    }
//    tcsetattr(STDIN_FILENO, TCSANOW, &newt); // 設定を即時適用
//}

int main() {
//    setBufferedInput(false);  // バッファリング無効
    TerminalGuard term;

    //create test contents**********************
    std::unique_ptr conts = Contents::create();
    conts->impContentsLine("test11 test12");
    conts->impContentsLine("test21 test22 test23");
    conts->impContentsLine("test31 test32 test33 test34");
    //*******************************************

    //show message
    std::cout << "1文字ずつ入力を検出します（qで終了）:\n";

    //input char
    char c;

    //create lineCtler
    std::unique_ptr lineCtl = LineCtler::create();

    //create newline(0 line)
    Aline* curline = &(lineCtl->newline());
//  aline& curline = lines->newline(); ←参照で受け取るとcurlienは更新できない

    //create timer
    //TODO Timer created for LineNumber. but not to be used
    auto t = Timer::create(lineCtl->cur_linenum());

    //show first line contents
    std::cout << conts->contentsLine(lineCtl->cur_linenum()-1) << std::endl;

    while (true) {

//        c = getchar();
        ssize_t n = ::read(STDIN_FILENO,&c,1);

        if (n!=1){continue;}
        std::cout << c <<std::flush;

        if (c == '\n') {
            std::cout << curline->elaplsedLineInput() << std::endl;

            //create newline(n line)
            curline = &(lineCtl->newline());
            //curline = lines->newline();
            //info↑ これはNG。NG理由は以下の通り
            //参照ではなくコピーになってる。
            //curlineはvectorが再生成される前のnewlineを指しているが、
            //newlineが動くことで再生成されて参照先が古くなりリンクが壊れる。。。
            //そして、c++では参照は一度設定すると更新できない。

            //show line contents by linenumber
            std::cout << conts->contentsLine(lineCtl->cur_linenum()-1) << std::endl;
            continue;
        }
        if (c == 'q') break;

        //append cahr with elapsed ms.
        curline->appendChar(c,t->elapsedMilliseconds());

    }
//    setBufferedInput(true);   // back termios

    //debuging for show all typed lines
    std::cout<<"#######################\n"<<std::endl;
    std::cout<<lineCtl->alllines()<<std::endl;
    return 0;
}
