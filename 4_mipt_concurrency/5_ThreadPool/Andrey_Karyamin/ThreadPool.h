//
// Created by avk on 13.03.16.
//

#pragma once

#include "../../4_BlockingQueue/Andrey_Karyamin/BlockingQueue/BlockingQueue.h"

#include <thread>
#include <future>
#include <memory>

// Класс пула потоков для параллельного
// выполнения заданий
template<class T>
class ThreadPool {
public:
    typedef std::shared_ptr<std::promise<T>> SmartPointerToPromise;
    typedef std::shared_ptr<std::future<T>> SmartPointerToFuture;

    // Конструкторы
    ThreadPool();
    explicit ThreadPool(unsigned threadsNumber);
    // Конструктор копирования и '=' запрещены в параллельном мире
    ThreadPool(const ThreadPool<T>& threadPool) = delete;
    ThreadPool<T>& operator=(const ThreadPool<T>& threadPool) = delete;
    // Отправка задания на исполнение потребителем.
    SmartPointerToFuture submitTask(std::function<T()> functionToPerform);
    // Деструктор. Кидает всем потокам отраву и ждёт пока все умрут.
    // Таким образом невыполненных заданий оставаться не будет.
    ~ThreadPool();
private:
    // Количество потоков-исполнителей в пуле.
    unsigned threadsNumber_;
    // Вектор потоков-работников-исполнителей.
    std::vector<std::thread> ourWorkers_;

    // Вспомогательный класс упакованного задания для потоков-исполнителей.
    // Используется как шаблонный параметр блокирующей очереди.
    // Имеет всего лишь два поля:   job - функтор для исполнения
    //                              promise - умный указатель на promise,
    //                              для возвращения результата работы.
    // Описание в ThreadPool.hpp
    class ThreadTask;
    // Блокирующая очередь для передачи заданий исполнителям. Потокобезопасна.
    // Состоит из запакованных заданий ThreadTasks.
    BlockingQueue<ThreadPool<T>::ThreadTask> blockingQueue_;

// В паре с конструктором.
    // Возвращает количество ядер.
    // Число в кандидаты количества воркеров. Если что-то идёт не так - то четыре.
    unsigned getCoresNumber() const;
    // Возвращает рабочую функцию потоков-исполнителей.
    auto getWorkerFunction();

// Упаковка.
    // Вспомогательная функция для упаковки задания
    // в класс ThreadTask. Заголовок пару строчками выше.
    ThreadPool<T>::ThreadTask packThreadTask(std::function<T()> job,
                               SmartPointerToPromise promise);

// В паре с деструктором.
    // Отправляет в очередь ядовитые задания для воркеров, чтобы их убить.
    void sendPoisonousPills();
    // Работает в паре с деструктором. Ждёт завершения всех потоков.
    void joinAllWorkers();
};

#include "ThreadPool.hpp"
