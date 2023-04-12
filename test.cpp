#include <thread>
#include <condition_variable>
#include <iostream>
#include <vector>
#include <queue>
#include <mutex>

int main() {
    std::mutex mut;
    int a = 0;
    
    {
        std::unique_lock<std::mutex> lk{mut};
        a += 3;
        lk.unlock();
    }

    std::cout << a << std::endl;
}