#include <iostream>
#include <chrono>
#include <thread>
#include <vector>

class Timer{
    int id_;
    std::chrono::steady_clock::time_point last_;
    explicit Timer(int id):id_{id},last_{std::chrono::steady_clock::now()}{}
public:
    static std::unique_ptr<Timer> create(int id){
        return std::unique_ptr<Timer>(new Timer{id});
    }
//    int getDiffMSeconds(){
    int elapsedMilliseconds() const {
        auto now = std::chrono::steady_clock::now();
        return std::chrono::duration_cast<std::chrono::milliseconds>(now-last_).count();
    }
};

int main(){

    auto t = Timer::create(1);

    for (int i = 0; i < 10000; ++i) {

//       std::this_thread::sleep_for(std::chrono::microseconds(1));

//        std::cout << "\r" << t->getDiffMSeconds() <<std::flush;
//        std::cout << "\r" << t->elapsedMilliseconds() <<std::flush;
    }
    std::cout << '\n';
    std::cout << "total: " << t->elapsedMilliseconds() <<std::flush;

}
