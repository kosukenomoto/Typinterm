#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <ranges>
#include <numeric>
class TimeChar{
    char ch_;
    int in_milliseconds_;
public:
    explicit TimeChar(char x,int in_mil):ch_{x},in_milliseconds_{in_mil} {}
    char getChar() const { return ch_; }
    int getTime() const {return in_milliseconds_;}
};

class Aline{
    std::vector<TimeChar> aline_;
    int aline_num_;
public:
    explicit Aline(int n):aline_num_{n}{}
    void appendChar(char c){
//        aline_.push_back(new TimeChar{c,0});
//      上記ではnewしたポインタを返すので型不一致。下記と書く。
        aline_.emplace_back(c,0);
    }
    void appendChar(char c,int t){
        aline_.emplace_back(c,t);
    }
    void appendTimeChar(TimeChar tm){ aline_.push_back(tm); }
    void deleteLastTimeChar(void){aline_.pop_back();}
    std::string showLine(void) const {
        std::string result;
        result.reserve(aline_.size());
        for(const TimeChar& c : aline_){
            result += c.getChar();
        }
        return result;
    }
    //先頭と最後のTimeCharのmilsec差 //TODO １文字の場合差異が0
    int elaplsedLineInput(){
        return aline_.back().getTime() - aline_.front().getTime();
    }
    //TODO in_milliseconds_をキーにしてcharをゲットするメソッドを設ける
};

class LineCtler{
    std::vector<Aline> lines_past_;
    int cur_linenum_;
    explicit LineCtler():cur_linenum_{0}{}
public:
    static std::unique_ptr<LineCtler> create(void) {
        return std::unique_ptr<LineCtler>(new LineCtler());
    }
    Aline& newline() {
        lines_past_.emplace_back(cur_linenum_++);
        return lines_past_.back();
    }
    std::string showline0() {
        return lines_past_[0].showLine();
    }
    std::string alllines() {
        auto view =  lines_past_
            | std::views::transform([](const Aline& a){
                return a.showLine();
            });
        return std::accumulate(view.begin(),view.end(),std::string{},
            [](std::string acc,const std::string& s){
                return acc + s + '\n';
            });
    }
    int cur_linenum(void){
        return cur_linenum_;
    }
};
