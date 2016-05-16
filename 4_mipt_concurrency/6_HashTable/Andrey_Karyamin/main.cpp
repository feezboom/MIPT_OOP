#include "ThreadSafeHashSet.h"
#include <random>
#include <thread>
#include <fstream>

using namespace std;

void print(ThreadSafeHashSet<double>& h) {
    std::cout << "Blam" << std::endl;
}

void space() {
    for (int i = 0; i < 3; ++i) {
        std::cout << std::endl;


    }
}

template<class T>
class HashFoo {
public:
    long long operator()(double num) {
        long long toReturn = static_cast<long long>(num*(INT64_MAX / 101) * 39);
        return toReturn;
    }
};

int main() {
    ThreadSafeHashSet<double> h(100, 10);

    h.insert(24.4);
    h.printTable();
    std::cout << "elements == " << h.size() << std::endl;
    space();

    h.insert(2.1);
    h.printTable();
    std::cout << "elements == " << h.size() << std::endl;
    space();

    h.insert(3.1);
    h.printTable();
    std::cout << "elements == " << h.size() << std::endl;
    space();

    h.insert(4.1);
    h.printTable();
    std::cout << "elements == " << h.size() << std::endl;
    space();

    h.insert(987.45);
    h.printTable();
    std::cout << "elements == " << h.size() << std::endl;
    space();

    h.insert(5.1);
    h.printTable();
    std::cout << "elements == " << h.size() << std::endl;
    space();


    h.remove(2.1);
    h.printTable();
    std::cout << "elements == " << h.size() << std::endl;
    space();

    h.remove(3.1);
    h.printTable();
    std::cout << "elements == " << h.size() << std::endl;
    space();

    h.remove(4.1);
    h.printTable();
    std::cout << "elements == " << h.size() << std::endl;
    space();

    h.remove(24.4);
    h.printTable();
    std::cout << "elements == " << h.size() << std::endl;
    space();

    h.insert(24.4);
    h.printTable();
    std::cout << "elements == " << h.size() << std::endl;
    space();

    h.insert(2.1);
    h.printTable();
    std::cout << "elements == " << h.size() << std::endl;
    space();

    h.insert(3.1);
    h.printTable();
    std::cout << "elements == " << h.size() << std::endl;
    space();

    h.insert(4.1);
    h.printTable();
    std::cout << "elements == " << h.size() << std::endl;
    space();

    h.insert(987.45);
    h.printTable();
    std::cout << "elements == " << h.size() << std::endl;
    space();

    h.insert(5.1);
    h.printTable();
    std::cout << "elements == " << h.size() << std::endl;
    space();


    std::cout << "contains 10 = " << h.contains(10) << std::endl;
    std::cout << "contains 987.45 = " << h.contains(987.45) << std::endl;
    std::cout << "contains 5.1 = " << h.contains(5.1) << std::endl;
    std::cout << "contains 2.1 = " << h.contains(2.1) << std::endl;
    std::cout << "contains 7.7 = " << h.contains(7.7) << std::endl;

    mutex write;
    int counter = 0;
    int limit = 100000;
    auto threadLambda = [&h, &write, &counter, limit] (int id) {
        for (int i = 0; i < limit; ++i) {
            random_device rd;   // non-deterministic generator
            mt19937 gen(rd());
            int task = static_cast<unsigned int>(abs(gen())) % 3;
            // Случайные двузначные числа.
            double number = ((int)abs(gen()) % 10000) / 100.0;
            number = max(number, -number);
            write.lock();
            std::cout << "[" << id << "] " << "ITERATION : " << counter++ << "||" << "i == " << i << std::endl;
            write.unlock();
            //std::cout << "task = " << task << "; number = " << number << std::endl;
            //std::cout << "thread id == " << this_thread::get_id() << std::endl;
            //##########################################################################################################
            //##########################################################################################################
            //##########################################################################################################
            // taks == 0 <==> вставить случайно сгенерированный номер
            // taks == 1 <==> удалить случайно сгенерированный номер
            // taks == 2 <==> проверить на присутствие случайно сгенерированный номер
            //##########################################################################################################
            //##########################################################################################################
            //##########################################################################################################
            switch (task) {
                case 0 :
                    //write.lock();
                    //std::cout << "||" << "[" << id << "] " << "inserting " << number << std::endl;
                    //write.unlock();
                    //std::cout << "before:" << std::endl;
                    //std::cout << "count == " << h.countElements() << std::endl;
                    //std::cout << "elementsnumber == " << h.currentElementsNumber_ << std::endl;
                    //h.printTable();
                    h.insert(number);
                    //std::cout << "after" << std::endl;
                    //std::cout << "count == " << h.countElements() << std::endl;
                    //std::cout << "elementsnumber == " << h.currentElementsNumber_ << std::endl;
                    //space();
                    //if (h.countElements() != h.currentElementsNumber_) {
                    //    assert(h.size() == h.currentElementsNumber_);
                    //}
                    
                    //assert(h.size() == h.currentElementsNumber_);
                    //bool beforeInsert;
                    //beforeInsert = (h.size() == h.currentElementsNumber_);
                    //h.printTable();
                    //##################################################################################################
                    h.insert(number);
                    //##################################################################################################
                    //write.lock();
                    //std::cout << counter++ << "||" << "[" << id << "] " << "inserted" << std::endl;
                    //write.unlock();
                    //h.printTable();
                    //space();
                    //bool afterInsert;
                    //afterInsert = (h.size() == h.currentElementsNumber_);
                    //assert(h.size() == h.currentElementsNumber_);
                    break;
                case 1:
                    //write.lock();
                    //std::cout << "||" << "[" << id << "] " << "removing number " << number << std::endl;
                    //write.unlock();
                    //std::cout << "before:" << std::endl;
                    //std::cout << "count == " << h.size() << std::endl;
                    //std::cout << "elementsnumber == " << h.currentElementsNumber_ << std::endl;
                    //h.printTable();
                    h.remove(number);
                    //std::cout << "after" << std::endl;
                    //std::cout << "count == " << h.countElements() << std::endl;
                    //std::cout << "elementsnumber == " << h.currentElementsNumber_ << std::endl;
                    //space();
                    
                    //assert(h.size() == h.currentElementsNumber_);
                    //bool beforeRemove;
                    //beforeRemove = (h.size() == h.currentElementsNumber_);
                    //h.printTable();
                    //##################################################################################################
                    h.remove(number);
                    //##################################################################################################
                    //write.lock();
                    //std::cout << counter++ << "||" << "[" << id << "] " << "removed" << std::endl;
                    //write.unlock();
                    //h.printTable();
                    //space();
                    // afterRemove;
                    //afterRemove = (h.size() == h.currentElementsNumber_);
                    //assert(h.size() == h.currentElementsNumber_);
                    break;
                case 2:
                    write.lock();
                    std::cout << "[" << id << "] " << "contains " << number << " == " << h.contains(number) << std::endl;
                    write.unlock();
                    //std::cout << h.contains(number);
                    //##################################################################################################
                    h.contains(number);
                    //##################################################################################################
                    //assert(h.size() == h.currentElementsNumber_);
                    //if (i % 100 == 0)
                    //    std::cout << std::endl;
                    break;
                default:
                    break;
            }
        }
    };

    //threadLambda(7);

    h.insert(4.8);
    //assert(h.size() == h.currentElementsNumber_);
    h.insert(4.8);
    //assert(h.size() == h.currentElementsNumber_);
    h.insert(4.8);
    //assert(h.size() == h.currentElementsNumber_);
    h.remove(4.8);


   std::vector<thread> threads;
    for (int i = 0; i < 10; ++i) {
        threads.push_back(thread(threadLambda, i));
    }

    for (int i = 0; i < threads.size(); ++i) {
        if (threads[i].joinable())
            threads[i].join();
    }

    h.printTable();

    //std::cout << "elementsCurrent == " << h.currentElementsNumber_ << std::endl;
    std::cout << "elementsCounted == " << h.size() << std::endl;


    std::cout << "Hello, World!" << std::endl;

    this_thread::sleep_for(chrono::seconds(1));
    return 0;
}