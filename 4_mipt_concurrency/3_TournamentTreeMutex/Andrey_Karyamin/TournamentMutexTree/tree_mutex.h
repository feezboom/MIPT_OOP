//
// Created by avk on 26.02.16.
//

#ifndef TOURNAMENTMUTEXTREE_MUTEXTREE_H
#define TOURNAMENTMUTEXTREE_MUTEXTREE_H


#include <atomic>
#include <vector>

// Mutex Петерсона
class PetersonMutex
{
public:
    // Конструктор.
    PetersonMutex();
    // Конструктор копирования.
    PetersonMutex(const PetersonMutex &instance) = delete;

    PetersonMutex(const PetersonMutex &&instance) = delete;

    PetersonMutex operator=(const PetersonMutex &instance) = delete;
    // Захватить mutex. threadId принимает значения из множества {0, 1}
    void lock(int threadId);
    // Отпустить mutex. threadId принимает значения из множества {0, 1}
    void unlock(int threadId);

private:
    // Массив занятости пруда. Флажки.
    std::atomic<bool> locked[2];
    // Обозначение того, кто последний вышел из doorway секции
    std::atomic<int> last;
};


// Турнирное дерево, на основе алгоритма Петерсона
// Дерево имеет всеми внутренними вершинами мютексы Петерсона, а листьями - потоки
// Если быть точнее, то их идентификаторы
class tree_mutex {
public:
    // Конструктор
    tree_mutex(unsigned int threadsNumber);

    // Захватить мютекс потоком threadId.
    // По факту threadId это номер вершины дерева, в которой как бы "находится" поток.
    // Просто в мютексе Петерсона это число будет браться по модулю 2,
    // а значит всё отработает корректно.
    void lock(unsigned int threadId);
    // Отпустить мютекс, захваченный потоком threadId.
    void unlock(unsigned int threadId);
private:
    struct node {
        PetersonMutex mtx;
        unsigned int owner;
    };
    unsigned int threadsNumber;
    unsigned int treeSize; // Количество мютексов.
    unsigned int threadStartIndex;
    std::vector<node> ourTournamentTree;
};


#endif //TOURNAMENTMUTEXTREE_MUTEXTREE_H
