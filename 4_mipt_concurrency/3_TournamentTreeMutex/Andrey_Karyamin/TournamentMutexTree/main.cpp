#include "tree_mutex.h"
#include <iostream>
#include <thread>


static const int threadsNumber = 100;
static tree_mutex mutexTree(threadsNumber);

void ourThreadFunc(int id) {
    for (int i = 0; i < 10; ++i) {
        mutexTree.lock(id);
        std::cout << "Hello!" << " I'm some thread; id == " << id << "; critical section started; ";
        std::cout << "critical section finished" << std::endl;
        mutexTree.unlock(id);
    }
}

int main() {
    std::vector <std::thread> threads;

    for (int i = 0; i < threadsNumber; ++i) {
        threads.push_back(std::thread(ourThreadFunc, i));
    }
    for (int j = 0; j < threadsNumber; ++j) {
        if (threads[j].joinable())
            threads[j].join();
    }

    return 0;
}