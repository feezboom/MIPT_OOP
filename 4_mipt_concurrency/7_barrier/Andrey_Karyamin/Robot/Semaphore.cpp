//
// Created by avk on 07.04.16.
//

#include "Semaphore.h"

Semaphore::Semaphore() : Semaphore(0) {}

Semaphore::Semaphore(unsigned counter) : counter(counter) { }

void Semaphore::wait() {
    std::unique_lock<std::mutex> locker(mtx);
    while (counter == 0) {
        cv.wait(locker);
    }
    counter--;
}

void Semaphore::signal() {
    std::unique_lock<std::mutex> locker(mtx);
    counter++;
    if (counter > 0) {
        cv.notify_one();
    }
}



