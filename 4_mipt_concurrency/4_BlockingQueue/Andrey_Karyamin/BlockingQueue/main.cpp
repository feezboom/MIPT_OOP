#include <iostream>
#include <random>
#include <thread>
#include "BlockingQueue.h"

// std::packaged_task
// std::async элитнааа

// Проверка числа на простоту.
bool prime(long long n){
    for(long long i=2;i<=sqrt(n);i++)
        if(n%i==0)
            return false;
    return true;
}
// Количество активных потоков, которые будут выполнять задания - проверять на простоту.
static const short threadsNumber = 100;
// Размер нашей будущей очереди.
static const int queueSize = 10000;
// Сама себе блокирующая очередь.
static BlockingQueue<long long> ourQueue(queueSize);
// Мютекс, нужный для синхронизированного вывода результатов потоков по выполнению заданий.
static std::mutex writeMutex;

// Функция, на которой запускаются все наши потоки.
void threadFunc(int myId) {
    while (true) {
        // То, что мы будем доставать из очереди.
        long long item;
        // Достаём.
        ourQueue.dequeue(item);
        // А вдруг яд? Нужно проверить.
        if (item == -1)
            break;
        // Проверим число на простоту, и уведомим о результате.
        writeMutex.lock();
        std::cout << "THREADID == " << myId << "; item " << item << " prime == " <<
                (prime(item) ? "true" : "false") << std::endl;
        writeMutex.unlock();
    }
}

// MAIN

int main() {
    // Это вектор потоков.
    std::vector<std::thread> threads;


    // Запускаем все наши потоки
    for (int i = 0; i < threadsNumber; ++i) {
        threads.push_back(std::thread(threadFunc, i));
    }


    // Этот блок кладёт задания в нашу очередь.
    unsigned i = 1;
    while (i++ < 1000) {

        // Сгенерируем два случайных числа.
        std::random_device rd;   // non-deterministic generator
        std::mt19937 gen(rd());
        long long k =  static_cast<long long>(abs(gen())) % 102;
        long long k1 =  static_cast<long long>(abs(gen())) % 102;

        // Сунем эти два числа в блокирующую очередь.
        writeMutex.lock();
        std::cout << "k == " << k  << " given " << std::endl;
        std::cout << "k1 == " << k1  << " given " << std::endl;
        writeMutex.unlock();

        ourQueue.enqueue(k);
        ourQueue.enqueue(k1);
    }
/*
    // Запускаем все наши потоки
    for (int i = 0; i < threadsNumber; ++i) {
        threads.push_back(std::thread(threadFunc, i));
    }
*/
    // Бросим ядовитые пилюли, чтобы убить потоки.
    for (int i = 0; i < threadsNumber; ++i)
        ourQueue.enqueue(-1);


    // Ждём пока все потоки завершатся.
    for (int j = 0; j < threadsNumber; ++j) {
        if (threads[j].joinable())
            threads[j].join();
    }

    return 0;
}