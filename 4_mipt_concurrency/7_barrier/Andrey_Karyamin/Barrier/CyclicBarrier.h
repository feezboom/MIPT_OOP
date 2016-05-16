//
// Created by avk on 28.03.16.
//

#pragma once

#include <mutex>
#include <condition_variable>

class CyclicBarrier {
public:
    // Барьер обязательно параметризован количеством потоков, которые должны через него пройти.
    CyclicBarrier(unsigned limit_);
    CyclicBarrier(const CyclicBarrier&) = delete;
    // Пройти через барьер. Поток засыпает если ещё не время.
    void enter();

private:
    // Сколько потоков нужно для прохождения барьера
    unsigned limit_;
    // Сколько уже есть потоков, ждущих.
    unsigned currentThreadsNumber_;
    // Синхронизация с помощью эпох.
    unsigned era_;
    // Мьютекс для атомарного изменения переменных
    std::mutex enterGate_;
    // Условная переменная для ожидания остальных потоков.
    std::condition_variable timeToPass_;
};

