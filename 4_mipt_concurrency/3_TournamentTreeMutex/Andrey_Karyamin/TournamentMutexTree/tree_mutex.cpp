//
// Created by avk on 26.02.16.
//
#include "tree_mutex.h"
#include <thread>
#include <cmath>

PetersonMutex::PetersonMutex() {
    // Опускаем флажки.
    locked[0].store(0);
    locked[1].store(0);
}


void PetersonMutex::lock(int threadId) {
    int otherThreadId = 1 - threadId;
    // Doorway секция.
    locked[threadId].store(1);
    last.store(threadId);

    // Ожидание освобождения пруда.
    while(locked[otherThreadId].load() && last.load() == threadId) {
        std::this_thread::yield();
    }
}

void PetersonMutex::unlock(int threadId) {
    locked[threadId].store(0);
}

tree_mutex::tree_mutex(unsigned int threadsNumber) {
    // Инициализация числа потоков
    this->threadsNumber = threadsNumber;

    if (threadsNumber <= 1) return;
    // Вычисление количества нужных мютексов в дереве
    unsigned int mtxNum; // Количество мютексов в дереве.
    unsigned int power = (unsigned int)log2(threadsNumber - 1) + 1;
    mtxNum = pow(2, power) - 1; // Количество мютексов - количество элементов в дереве.
    treeSize = mtxNum;
    // Посчитаем индекс вершины, с которой, условно, начинаются наши потоки - листья.
    threadStartIndex = mtxNum;

    // Создадим мютексы.
    ourTournamentTree = std::vector<node>(mtxNum);
    // Обозначим владельцев, как никого.
    for(auto &it : ourTournamentTree)
        it.owner = static_cast<unsigned int>(-1);
}

void tree_mutex::lock(unsigned int threadId) {
    if (threadsNumber <= 1)
        return;

    // Считаем индекс вершины, в которой находится поток в дереве.
    unsigned int threadCurrentVertexIndex = threadId + treeSize;
    // И поднимаемся по дереву вверх.
    while (threadCurrentVertexIndex != 0) {
        // Сохраняем ребёнка, из которого мы поднимаемся.
        unsigned int childMovedFrom = threadCurrentVertexIndex;

        // Считаем родителя, куда будем подниматься.
        threadCurrentVertexIndex = (threadCurrentVertexIndex - 1) / 2;

        // Посчитаем локальный id потока нужный для мютекса Петерсена.
        unsigned int myLocalId = childMovedFrom % 2;

        // Пытаемся захватить мютекс родителя.
        ourTournamentTree[threadCurrentVertexIndex].mtx.lock(myLocalId);
        // И когда вдруг захватили, устанавливаем этому мютексу владельца - текущий поток (threadId).
        ourTournamentTree[threadCurrentVertexIndex].owner = threadId;
    }
}

void tree_mutex::unlock(unsigned int threadId) {
    if (threadsNumber <= 1)
        return;
    // Выходим мы из корня, поэтому стартовая вершина имеет индекс ноль.
    unsigned int threadCurrentVertexIndex = 0;
    // Будем хранить индексы детей вершины, в которой сейчас находится поток.
    unsigned int leftSon, rightSon;
    // Собственно говоря, сам спуск по дереву, "отпускание" мютексов.
    while (threadCurrentVertexIndex < threadStartIndex) {
        // Посчитаем детей.
        leftSon = threadCurrentVertexIndex * 2 + 1;
        rightSon = leftSon + 1;

        // Теперь нужно бы узнать, в какого ребёнка стоит опускаться. Естественно из которого мы поднялись.
        // Распознаем это с помощью владельца.

        unsigned int sonToMove;

        if (leftSon >= threadStartIndex) {
            // Если уровень ниже уже не содержит мютексов, то мы уже на дне.
            sonToMove = threadId + treeSize;
        } else {
            // Иначе узнаем куда нужно спускаться.
            unsigned int leftSonOwner = ourTournamentTree[leftSon].owner;
            // Либо в левого сына.
            if (leftSonOwner == threadId) {
                sonToMove = leftSon;
            } else {
                // А иначе в правого, и никак по-другому.
                sonToMove = rightSon;
            }
        }

        // Показываем, что владельца у мютекса нет.
        ourTournamentTree[threadCurrentVertexIndex].owner = static_cast<unsigned int>(-1);
        // И отпускаем сам мютекс.
        ourTournamentTree[threadCurrentVertexIndex].mtx.unlock(sonToMove % 2);

        // Далее, уже законно, спускаемся вниз по дереву.
        threadCurrentVertexIndex = sonToMove;
    }
}