#include <iostream>
#include <vector>
#include <future>
#include "ThreadPool.h"
#include "ConcurrentMergeSort.h"

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
/*
    vector<shared_ptr<future<long>>> answer;

    shared_ptr<std::future<long>> myFuture;
    for (double i = 1; i < 85899345; i*=1.5) {
        myFuture = gogoLovers.submitTask(std::bind(test, i-0));
    }

    std::cout << "Got result : " << myFuture->get() << std:: endl;
    for (int j = 10 - 1; j >= 0; ++j) {

    }
*/
    std::vector<int> part1 = {3, 6, 7, 9, 15};
    std::vector<int> part2 = {1, 2, 5, 7, 9, 13, 17, 19};
    std::vector<int> result(13);

    merge(part1.begin(), part1.end(), part2.begin(), part2.end(), result.begin());

    std::cout << std::endl;
    //                         1  2   3   4   5  6  7   8   9   10 11 12 13 14 15 16
    std::vector<int> toSort = {5, 4, 15, 12, 35, 7, 0, -5, 45, 666, 7, 1, 0, 2, 4, 3};
    MergeSort(toSort.begin(), toSort.end());
/*
    toSort = {1, 0, 2, -1};
    MergeSort(toSort.begin(), toSort.end());
*/
    for_each(toSort.begin(), toSort.end(), [] (int i) { std::cout << i << " ";});
    return 0;
}
