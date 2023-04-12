#include <iostream>
#include <thread>
#include <mutex>
#include <unistd.h>

std::mutex cout_mutex;

class hello {
private:
    int &i;
public:
    hello(int &i_):i(i_){}
    void operator()() const {
        for (int j = 0; j < 10; j++) {
            usleep(100000 + i);
            cout_mutex.lock();
            std::cout << j << " hello world " << i << std::endl;
            cout_mutex.unlock();
        }
    }
};

void except() {
    int tmp = 3;
    std::thread t{hello(tmp)};
    t.detach();
    cout_mutex.lock();
    std::cout << "exiting oops" << std::endl;
    cout_mutex.unlock();
}

int main() {
    except();
    sleep(2);
    cout_mutex.lock();
    std::cout << "exiting main" << std::endl;
    cout_mutex.unlock();
}