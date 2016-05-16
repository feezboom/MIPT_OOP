//
// Created by avk on 28.03.16.
//

#include "CyclicBarrier.h"

CyclicBarrier::CyclicBarrier(unsigned int limit_)
        : limit_(limit_), era_(0)
{
    if (limit_ == 0) {
        throw std::invalid_argument("There must be at least one thread.");
    }
}

void CyclicBarrier::enter() {
    // Хватаем мьютекс.
    std::unique_lock<std::mutex> locker(enterGate_);
    // Потоков стало больше.
    ++currentThreadsNumber_;
    // Если уже можно пройти, то скажем об этом всем.
    if (currentThreadsNumber_ == limit_) {
        currentThreadsNumber_ = 0;
        era_++;
        timeToPass_.notify_all();
        // Если нет - то подождём пока будет можно. Кто-нибудь нам скажет.
    } else {
        unsigned myEra = era_;
        timeToPass_.wait(locker, [this, &myEra] () -> bool { return myEra == era_;});
    }
}