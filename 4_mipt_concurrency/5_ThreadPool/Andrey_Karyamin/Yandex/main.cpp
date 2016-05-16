#include <iostream>
#include <vector>
#include <future>
#include "thread_pool.h"

using std::vector;
using std::shared_ptr;
using std::future;

long test(long limit)
{
    long long a = 0;
    for (int i = 0; i < limit; ++i) {
        a++;
        if (i % 10000000000 == 0)
            std::cout << "I'm counting. ID ==" << std::this_thread::get_id() << std::endl;
    }
    return a;
}
int main() {
    ThreadPool<long> gogoLovers(10);

    vector<shared_ptr<future<long>>> answer;

    shared_ptr<std::future<long>> myFuture;
    for (double i = 1; i < 85899345; i*=1.5) {
        myFuture = gogoLovers.submitTask(std::bind(test, i-0));
    }

    std::cout << "Got result : " << myFuture->get() << std:: endl;
    for (int j = 10 - 1; j >= 0; ++j) {

    }

    return 0;
}