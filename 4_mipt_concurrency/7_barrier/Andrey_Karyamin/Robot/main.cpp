#include "Semaphore.h"
#include <iostream>
#include <thread>

using namespace std;

static Semaphore semLeft(1);
static Semaphore semRight(0);

void walkingLeft(unsigned id) {
    int i = 0;
    while(i++ <= 10) {
        semRight.wait();
        cout << "step " << i << " left" << endl;
        semLeft.signal();
    }
}
void walkingRight(unsigned id) {
    int i = 0;
    while(i++ <= 10) {
        semLeft.wait();
        cout << "step " << i << " right" << endl;
        semRight.signal();
    }
}
void SemaphoreWalking() {
    std::thread thread1(walkingLeft, 1);
    std::thread thread2(walkingRight, 2);

    if (thread1.joinable()) {
        thread1.join();
    }
    if (thread2.joinable()) {
        thread2.join();
    }
}


static std::mutex mtx;
static condition_variable cvLeft;
static condition_variable cvRight;

static int lastStep = 0;

void walkLeft() {
    std::unique_lock<std::mutex> locker(mtx);
    int i = 0;
    while(i++ <= 20) {
        while (lastStep == 0)
            cvRight.wait(locker);
        lastStep = 0;
        cout << "step " << i << " left" << endl;
        cvLeft.notify_one();
    }
}
void walkRight() {
    std::unique_lock<std::mutex> locker(mtx);
    int i = 0;
    while(i++ <= 20) {
        while (lastStep == 1)
            cvLeft.wait(locker);
        lastStep = 1;
        cout << "step " << i << " right" << endl;
        cvRight.notify_one();
    }
}

void cvWalking() {
    std::thread thread1(walkLeft);
    std::thread thread2(walkRight);

    if (thread1.joinable()) {
        thread1.join();
    }
    if (thread2.joinable()) {
        thread2.join();
    }
}

int main() {
    cout << "Hello, World!" << endl;
    SemaphoreWalking();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    cout << endl << endl << endl;

    cvWalking();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    return 0;
}