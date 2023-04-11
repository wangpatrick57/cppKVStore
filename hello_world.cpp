#include <iostream>
#include <thread>

class hello {
private:
    int i;
public:
    hello(int i_):i(i_){}
    void operator()() const {
        for (int j = 0; j < 100000; j++) {
            std::cout << "hello world " << i << std::endl;
        }
    }
};

int main() {
    std::thread t((hello(5)));
    t.join();
    std::cout << "end\n";
}