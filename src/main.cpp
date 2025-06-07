#include <ncpp/NotCurses.hh>

#include "Contents.cpp"
#include "Inputs.cpp"
#include "Timer.cpp"
#include "input.cpp"
#include "key_event.hpp"
#include "renderer.hpp"
#include "session.hpp"
#include "terminal.hpp"

int main() {
  //    setBufferedInput(false);  // バッファリング無効
  TerminalGuard term;
  // ncpp::NotCurses nc;
  // auto* stdp = nc.get_stdplane();
  // stdp->putstr(0, 0, "hello");
  // nc.render();

  // create test contents**********************
  std::unique_ptr conts = Contents::create();
  conts->impContentsLine("test11 test12");
  conts->impContentsLine("test21 test22 test23");
  conts->impContentsLine("test31 test32 test33 test34");
  //*******************************************

  // show message
  std::cout << "1文字ずつ入力を検出します（qで終了）:\n";

  // input char
  char c;

  // session creaste
  TypingSession session("This test was very cool.\nand Line2");

  // create lineCtler
  std::unique_ptr lineCtl = LineCtler::create();

  // create newline(0 line)
  Aline* curline = &(lineCtl->newline());
  //  aline& curline = lines->newline(); ←参照で受け取るとcurlienは更新できない

  // create timer
  // TODO Timer created for LineNumber. but not to be used
  auto t = Timer::create(lineCtl->cur_linenum());

  // show first line contents
  // std::cout << conts->contentsLine(lineCtl->cur_linenum()-1) << std::endl;
  std::unique_ptr<InputLoop> inputloop = InputLoop::create();
  Renderer render{};
  //    render.clr();
  //    render.hidecursor();
  // TODO evをキャッチしてステータスをupdateする構造にする
  while (session.phase() != TypingSession::Phase::Finished) {
    //        c = getchar();
    //        ssize_t n = ::read(STDIN_FILENO,&c,1);
    if (std::optional<KeyEvent> ev = inputloop->poll()) {
      if (session.update(*ev)) {
        render.draw(session);
      }
      //            std::cout << (*ev).c <<std::flush;
      //            if ((*ev).c == '\n') {
      //                std::cout << curline->elaplsedLineInput() << std::endl;
      //
      //                //create newline(n line)
      //                curline = &(lineCtl->newline());
      //                //curline = lines->newline();
      //                //info↑ これはNG。NG理由は以下の通り
      //                //参照ではなくコピーになってる。
      //                //curlineはvectorが再生成される前のnewlineを指しているが、
      //                //newlineが動くことで再生成されて参照先が古くなりリンクが壊れる。。。
      //                //そして、c++では参照は一度設定すると更新できない。
      //
      //                //show line contents by linenumber
      //                std::cout <<
      //                conts->contentsLine(lineCtl->cur_linenum()-1) <<
      //                std::endl; continue;
      //            }
      //            if ((*ev).c == 'q') break;
      //
      //            //append cahr with elapsed ms.
      //            //curline->(c,t->elapsedMilliseconds());
      //            curline->appendTimeChar(*ev);
    }

    // if (!ev.has_value()){continue;}
  }
  //    setBufferedInput(true);   // back termios
  //    render.showcursor();
  // debuging for show all typed lines
  std::cout << "#######################\n" << std::endl;
  return 0;
}
