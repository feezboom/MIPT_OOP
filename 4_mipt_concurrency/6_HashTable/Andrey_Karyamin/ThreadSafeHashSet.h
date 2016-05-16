#pragma once
//
// Created by avk on 20.03.16.
//

#include <mutex>
#include <vector>
#include <forward_list>

// Потоко-безопасное множество, основанное на хэш-таблице.
// Потоко-безопасность реализована с помощью stripe lock метода - мьютекс на несколько корзин.
// Расширяется при превышении коэффициента заполнения.
// Сужается если коэффициент заполнения слишком низкий.
template<class T, class H = std::hash<T>>
class ThreadSafeHashSet {
public:
    // Конструкторы...
    ThreadSafeHashSet();
    ThreadSafeHashSet(unsigned int mutexNumber);
    ThreadSafeHashSet(unsigned int mutexNumber, double extremeLoadFactor);
    ThreadSafeHashSet(unsigned int mutexNumber, double extremeLoadFactor, unsigned int growthFactor);
    // Вставка элемента в таблицу.
    void insert(const T& element);
    // Удаление элемента из таблицы.
    void remove(const T& element);
    // Проверка на принадлежность таблице элемента.
    bool contains(const T& element);
    // Вывести всю таблицу в std::cout.
    void printTable() const;
    // Возвращает количество элементов в хэш-таблице.
    unsigned int size();

private:
    // Предельный load factor для таблицы перед расширением.
    // Также используется для сужения таблицы.
    double                              extremeLoadFactor_;
    // Во сколько раз каждый раз увеличивается/уменьшается таблица.
    // Должен быть целым числом, чтобы сохранялась корректность работы -
    // число корзин было кратно число мьютексов.
    unsigned int                        growthFactor_;
    // Количество мьютексов, защищающих страйпы.
    unsigned int                        mutexNumber_;
    // Текущее количество элементов в таблице.
    unsigned int                        currentElementsNumber_;
    std::mutex                          elementsCounterLock_;
    // Те самые мьютексы..
    class HonestRwMutex;
    std::vector<HonestRwMutex>          stripesLocks_;
    // Односвязный список в каждой корзине.
    std::vector<std::forward_list<T>>   buckets_;

    // Посчитать хэш от аргумента.
    unsigned int getHash(T element) const;
    // Вернуть текущее количество корзин - списков.
    unsigned int getCurrentBucketsNumber() const;
    // Вернуть текущую заполенность таблицы.
    double getCurrentLoadFactor() const;
    // Эта функция расширяет контейнер, если это необходимо -
    // заполненность превысила пороговую заполненность.
    void expandIfNeeded();
    // Будет сужать контейнер, если надо -
    // если новый load factor после сужения будет меньше extreme load factor.
    void condenseIfNeeded();
    // Перехеширование всей нашей таблицы.
    // Параметр обозначает что новые хэши элементов будут браться
    // по модулю этого параметра -  количества корзин.
    void reHash(unsigned int bucketsNumber);
    // Умная функция захвата всех мьютексов.
    // Хватает один, и, если кто-то уже успел поменять размер
    // контейнера, то он отпускает его и не хватает остальные.
    // Как результат возвращает true, если расширять нужно
    // и false иначе.
    bool smartlyGrabAllMutexes();
    // Отпустить все захваченные мьютексы из контейнера stripesLocks_.
    // Работает по-тупому.
    void releaseAllMutexes();


    // Константы по умолчанию.
    static constexpr int    DEFAULT_MUTEX_NUMBER        = 10;
    static constexpr int    DEFAULT_GROWTH_FACTOR       =  2;
    static constexpr double DEFAULT_EXTREME_LOAD_FACTOR =  2;
};

#include "ThreadSafeHashSet.hpp"

