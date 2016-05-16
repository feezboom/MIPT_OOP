//
// Created by avk on 03.03.16.
//
#pragma once

#include <mutex>
#include <condition_variable>
#include <queue>


template <class T>
class BlockingQueue {
public:
    // Обычные конструкторы.
    BlockingQueue();
    BlockingQueue(unsigned capacity);
    // Запрещённый конструктор копирования.
    BlockingQueue(const BlockingQueue<T>&) = delete;
    // Засунуть элемент item в очередь. Засыпает пока очередь полна.
    void enqueue(const T& item);
    // Вынуть элемент из очереди. Засыпает пока очередь пуста.
    void dequeue(T& item);
    T dequeue();

    ~BlockingQueue();
private:
    // Размер нашей очереди. Обусловимся, что если capacity == -1, то размер очереди неограничен.
    // Ну почти.
    unsigned capacity;
    // Собственно, очередь, для помещения в неё данных для обмена между потоками.
    std::queue<T> ourQueue;
    // Мютекс для безопасных операций с очередью.
    std::mutex ourMutex;
    // Условная переменная нужная для ожидания момента, когда очередь перестанет быть полной.
    std::condition_variable someonePopped;
    // Условная переменная нужная для ожидания момента, когда очередь перестанет быть пустой.
    std::condition_variable someonePushed;
};


#include "BlockingQueue.hpp"
