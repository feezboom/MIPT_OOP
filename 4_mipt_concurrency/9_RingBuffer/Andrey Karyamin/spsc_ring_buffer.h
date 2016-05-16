//
// Created by avk on 01.05.16.
//

#pragma once

#include <boost/atomic.hpp>
#include <thread>
#include <iostream>

template<class T>
class spsc_ring_buffer {
public:
    spsc_ring_buffer();
    spsc_ring_buffer(size_t capacity);
    bool enqueue(T e);
    bool dequeue(T& e);
    void invoke(int& i) {
        i = 10;
    }
    void call() {
        int a = 1;
        auto thread1 = std::thread(&spsc_ring_buffer::invoke, this, std::ref(a));
        std::cout << a;
    }
private:
    size_t capacity;
    static const size_t DEFAULT_CAPACITY = 1024;
};

template<class T>
spsc_ring_buffer<T>::spsc_ring_buffer() : capacity(DEFAULT_CAPACITY) {}

template<class T>
spsc_ring_buffer<T>::spsc_ring_buffer(size_t capacity) : capacity(capacity) {}

