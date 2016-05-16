#include "CyclicBarrier.h"
#include <iostream>
#include <thread>
#include <vector>
#include <future>
#include <cmath>

using namespace std;

static CyclicBarrier cyclicBarrier(5);
static std::mutex locker;

int F() {
    cyclicBarrier.enter();
    std::cout << "HELLO IM SOME THREAD" << std::endl;
    unsigned i = 0;
    unsigned barrier = 10000;
    while (i++ < barrier) {
        cyclicBarrier.enter();
        locker.lock();
        std::cout << "MY ID == " << std::this_thread::get_id() << std:: endl;
        locker.unlock();
    }
}


int main() {
    cout << "Hello, World!" << endl;
    std::vector<std::thread> threads;

    const unsigned threadsNumber = 30;

    for (int i = 1; i <= threadsNumber; ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        cout << "progress " << i*10 << "%" << std::endl;
        threads.push_back(std::thread(F));
    }

    for (int i = 0; i < threadsNumber; ++i) {
        threads[i].join();
    }

    return 0;
}