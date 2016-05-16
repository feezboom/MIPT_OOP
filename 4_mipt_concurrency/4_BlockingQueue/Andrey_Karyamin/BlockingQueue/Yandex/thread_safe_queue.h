//
// Created by avk on 24.04.16.
//

#pragma once

#include <mutex>
#include <condition_variable>
#include <queue>
#include <deque>
#include <exception>


template <class T, class Container = std::deque<T>>
class thread_safe_queue {
public:
    // Обычные конструкторы.
    explicit thread_safe_queue();
    explicit thread_safe_queue(size_t capacity);
    // Запрещённый конструктор копирования.
    thread_safe_queue(const thread_safe_queue<T>&) = delete;
    // Засунуть элемент item в очередь. Засыпает пока очередь полна.
    void enqueue(const T& item);
    void enqueue(T&& item);
    // Вынуть элемент из очереди. Засыпает пока очередь пуста.
    void pop(T &item);

    void shutdown();

    ~thread_safe_queue();
private:
    void enqueuePrepare();
    // Размер нашей очереди. Обусловимся, что если capacity == -1, то размер очереди неограничен.
    // Ну почти.
    size_t capacity;
    // Собственно, очередь, для помещения в неё данных для обмена между потоками.
    Container ourQueue;
    // Мютекс для безопасных операций с очередью.
    std::mutex ourMutex;
    // Условная переменная нужная для ожидания момента, когда очередь перестанет быть полной.
    std::condition_variable someonePopped;
    // Условная переменная нужная для ожидания момента, когда очередь перестанет быть пустой.
    std::condition_variable someonePushed;
    // Флаг завершения работы
    bool isShutdown;
};

template <class T, class C>
void thread_safe_queue<T, C>::shutdown() {
    isShutdown = true;
    someonePopped.notify_all();
}


// Обычный конструктор. Задаём вместимость очереди.
template <class T, class C>
thread_safe_queue<T, C>::thread_safe_queue(size_t capacity): capacity(capacity), isShutdown(false)
{}

template<class T, class C>
thread_safe_queue<T, C>::thread_safe_queue() : thread_safe_queue(static_cast<size_t>(-1))
{}


// Помещение элемента в очередь.
template<class T, class C>
void thread_safe_queue<T, C>::enqueue(const T& item) {
    T temp = item;
    enqueue(std::move(temp));
}
template <class T, class C>
void thread_safe_queue<T, C>::enqueue(T&& item) {
    if (isShutdown) {
        throw std::exception();
    }

    // Захватим мютекс, чтобы попробовать положить элемент в очередь.
    std::unique_lock<std::mutex> ourLock(ourMutex);

    // Стоит подождать, пока какой-нибудь другой поток
    // не вытащит по крайней мере один элемент из очереди, чтобы освободилось место.
    while(ourQueue.size() == capacity && !isShutdown) {
        someonePopped.wait(ourLock);
    }
    if (isShutdown) {
        throw std::exception();
    }
    // И теперь уже кладём в очередь.
    ourQueue.push_back(std::move(item));
    // Скажем  кому-нибудь, что мы засунули элемент в очередь.
    someonePushed.notify_one();

    // Отпускаем мютекс
}
template <class T, class C>
void thread_safe_queue<T, C>::enqueuePrepare() {
    if (isShutdown) {
        throw std::exception();
    }

    // Захватим мютекс, чтобы попробовать положить элемент в очередь.
    std::unique_lock<std::mutex> ourLock(ourMutex);

    // Стоит подождать, пока какой-нибудь другой поток
    // не вытащит по крайней мере один элемент из очереди, чтобы освободилось место.
    while(ourQueue.size() == capacity && !isShutdown) {
        someonePopped.wait(ourLock);
    }
    if (isShutdown) {
        throw std::exception();
    }
}

// Извлечение элемента из очереди.
template<class T, class C> void thread_safe_queue<T, C>::pop(T &item) {
    if (isShutdown && (ourQueue.size() == 0)) {
        throw std::exception();
    }
    // Блокируем мютекс, чтобы безопасно извлечь элемент из очереди.
    std::unique_lock<std::mutex> ourLock(ourMutex);
    // Если вдруг очередь пуста, то придётся подождать.
    while(ourQueue.size() == 0) {
        someonePushed.wait(ourLock);
    }
    // Всё, теперь очередь не пуста, извлечём долгожданный элемент.
    item = std::move(ourQueue.front());
    ourQueue.pop_front();
    // И уведомим кого-нибудь о том, что из очереди был извлечён элемент.
    someonePopped.notify_one();
}


template<class T, class C>
thread_safe_queue<T, C>::~thread_safe_queue() {
    std::unique_lock<std::mutex> ourLock(ourMutex);
    someonePopped.notify_all();
    someonePushed.notify_all();
}



