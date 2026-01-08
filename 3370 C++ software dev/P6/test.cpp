#include <iostream>
#include <thread>
#include <stop_token>
#include <vector>

using namespace std::literals::chrono_literals;

void threadFunction(std::stop_token st) {
        while (!st.stop_requested()) {
            std::cout << "Running..." << std::endl;
        }
    }

class Test {
public:
    void threadFunction(std::stop_token st) {
        while (!st.stop_requested()) {
            std::cout << "Running..." << std::endl;
        }
    }

    /*
    void start() {
        std::vector<std::jthread> threads;
        for (int i = 0; i < 4; i++) {
            threads.emplace_back(&Test::threadFunction, this);
        }
    }
        */
};

int main() {
    std::jthread j(threadFunction);
    std::this_thread::sleep_for(0.01s);
    /*
    Test t;
    std::vector<std::jthread> threads;
    for (int i = 0; i < 4; i++) {
        threads.emplace_back(&Test::threadFunction, &t);
    }
    */

    return 0;
}