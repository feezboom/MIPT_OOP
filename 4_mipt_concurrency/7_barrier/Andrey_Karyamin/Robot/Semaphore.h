//
// Created by avk on 07.04.16.
//

#pragma once


#include <condition_variable>

class Semaphore {
public:
    Semaphore();
    Semaphore(unsigned);
    void wait();
    void signal();
private:
    unsigned counter;
    std::mutex mtx;
    std::condition_variable cv;
};
