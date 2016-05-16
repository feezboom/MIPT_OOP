#include <iostream>
#include "TicketSpinLock.hpp"
#include <thread>

static TicketSpinLock ticketSpinLock;

using namespace std;

// Объявление
// Создать метод void ourThreadFunc(int id)
static void ourThreadFunc(int id) {
    ticketSpinLock.lock();
    cout << "Our critical section started; id == " << id << "; ";
    cout << "Our critical section finished" << endl;
    ticketSpinLock.unlock();
}

int main() {
    thread thread1(ourThreadFunc, 1),
            thread2(ourThreadFunc, 2),
            thread3(ourThreadFunc, 3),
            thread4(ourThreadFunc, 4),
            thread5(ourThreadFunc, 5),
            thread6(ourThreadFunc, 6);

    if (thread1.joinable())
        thread1.join();
    if (thread2.joinable())
        thread2.join();
    if (thread3.joinable())
        thread3.join();
    if (thread4.joinable())
        thread4.join();
    if (thread5.joinable())
        thread5.join();
    if (thread6.joinable())
        thread6.join();
    return 0;
}