//
// Created by avk on 24.02.16.
//

#ifndef PETERSON_MUTEX_MUTEX_H
#define PETERSON_MUTEX_MUTEX_H


#include <atomic>

// Mutex Петерсона
class PetersonMutex
{
public:
    // Конструктор.
    PetersonMutex()
    {
        // Опускаем флажки.
        locked[0].store(0);
        locked[1].store(0);
    };
    // Конструктор копирования.
    PetersonMutex(const PetersonMutex &instance)
    {
        *this = instance;
    }

    PetersonMutex(const PetersonMutex &&instance) {
        *this = instance;
    }

    PetersonMutex operator=(const PetersonMutex &instance) {
        this->locked[0].store(instance.locked[0].load());
        this->locked[1].store(instance.locked[1].load());
        this->last.store(instance.last.load());
    }
    // Захватить mutex.
    void lock(int threadId)
    {
        int otherThreadId = 1 - threadId;
        // Doorway секция.
        locked[threadId].store(1);
        last.store(threadId);

        // Ожидание освобождения пруда.
        while(locked[otherThreadId].load() && last.load() == threadId);
    }
    // Отпустить mutex.
    void unlock(int threadId) {
        locked[threadId].store(0);
    }
    ~PetersonMutex() {};
private:
    // Массив занятости пруда. Флажки.
    std::atomic<bool> locked[2];
    // Обозначение того, кто последний вышел из doorway секции
    std::atomic<int> last;
};


#endif //PETERSON_MUTEX_MUTEX_H
