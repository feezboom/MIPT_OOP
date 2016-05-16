// Задача 3. Блокирующий флаг [flag] - 1 балл
// Рассмотрим реализацию булевого флажка, который по задумке позволял бы потокам с блокировкой дождаться (с помощью метода wait) его простановки (методом set).
// Предполагается, что ждать флага могут несколько потоков, а ставить флаг - только один.
// Мьютекс используется для взаимного исключения ждущих потоков, для флага используется атомик, его ставит только один поток. 

#include <mutex>
#include <condition_variable>
#include <atomic>

class blocking_flag {
public:
    blocking_flag()
            : ready_(false)
    {}

    void wait() {
        std::unique_lock<std::mutex> lock(mtx_);
        while (!ready_.load()) {
            ready_cond_.wait(lock);
        }
    }

    void set() {
        ready_.store(true);
        ready_cond_.notify_all();
    }

private:
    std::atomic<bool> ready_;
    std::mutex mtx_;
    std::condition_variable ready_cond_;
};

// Пример использования:

#include <thread>
#include <iostream>
#include <vector>

int main() {
    blocking_flag f;

    auto lambda = [&f]() {
        f.wait();
        std::cout << "ready!" << std::endl;
    };

    std::vector<std::thread> p;
    for (int j = 0; j < 10000; ++j) {
        p.push_back(std::thread(lambda));
    }

    f.set();

    for (int i = 0; i < 10000; ++i) {
        if (p[i].joinable())
            p[i].join();
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));

    return 0;
}

// Оцените коррекность этого кода.
