#include <iostream>
#include "Mutex.h"
#include <thread>
#include <vector>

using namespace std;

PetersonMutex petersonMutex;

void ourThreadFunction(int id) {
    petersonMutex.lock(id);
    cout << "Here is our critical section, id = " << id << endl;
    petersonMutex.unlock(id);
}

int main() {
    PetersonMutex petersonMutex;

    thread thread0(ourThreadFunction, 0), thread1(ourThreadFunction, 1);
    if (thread0.joinable())
        thread0.join();

    if (thread1.joinable())
        thread1.join();
    return 0;
}