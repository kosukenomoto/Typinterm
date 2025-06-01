#include <vector>
#include <string>
#include <memory>

class Contents{
    std::vector<std::string> con_;
    explicit Contents(){};
public:
    static std::unique_ptr<Contents> create(void){
        return std::unique_ptr<Contents>(new Contents());
    }
    void impContentsLine(std::string line){
        con_.push_back(line);
    }
    std::string contentsLine(int linenum){
        return con_.at(linenum);
    }
    int totalLineNumber(void){
        return con_.size();
    }
};
