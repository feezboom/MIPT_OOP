//
// Created by avk on 28.03.16.
//

#pragma once

#include <algorithm>
#include "ThreadPool.h"

template<typename _InputIterator, typename _OutputIterator>
void merge(_InputIterator leftFirst, _InputIterator leftLast,
           _InputIterator rightFirst, _InputIterator rightLast,
           _OutputIterator result) {
    auto left = leftFirst;
    auto right = rightFirst;
    while (left != leftLast || right != rightLast) {
        // Крайний случай, когда дошли до конца одного из кусков
        if (left == leftLast) {
            *result = *right;
            right++;
        } else if (right == rightLast) {
            *result = *left;
            left++;
        } else if (*left < *right) {
            // Не дошли до конца ни одного из кусков.
            *result = *left;
            left++;
        } else {
            *result = *right;
            right++;
        }
        result++;
    }
}

template<typename _RandomAccessIterator, typename _OutputIterator>
void __MergeSort(_RandomAccessIterator first,
                 _RandomAccessIterator last,
                 _OutputIterator outputIterator,
                 size_t distance,
                 ThreadPool<bool>& threadPool) {

    size_t leftDistance = distance / 2;
    size_t rightDistance = distance - leftDistance;

    _RandomAccessIterator
            mid = first + leftDistance;

    _RandomAccessIterator
            leftFirst = first,
            leftLast = mid,
            rightFirst = mid,
            rightLast = last;

    _OutputIterator outputLeft = outputIterator;
    _OutputIterator outputRight = outputLeft + leftDistance;

    if (leftDistance > 1) {
        __MergeSort(leftFirst, leftLast, outputLeft, leftDistance, threadPool);
        auto out = outputLeft;
        for (auto it = leftFirst; it != leftLast; it++, out++) {
            *it = *out;
        }
    }
    if (rightDistance > 1) {
        __MergeSort(rightFirst, rightLast, outputRight, rightDistance, threadPool);
        auto out = outputRight;
        for (auto it = rightFirst; it != rightLast; it++, out++) {
            *it = *out;
        }
    }
    merge(leftFirst, leftLast, rightFirst, rightLast, outputIterator);

}

template<typename _RandomAccessIterator>
void MergeSort(_RandomAccessIterator first, _RandomAccessIterator last) {
    typedef typename std::iterator_traits<_RandomAccessIterator>::value_type _ValueType;

    std::vector<_ValueType> tempCopy(first, last);
    size_t distance(tempCopy.size());

    ThreadPool<bool> threadPool;
    __MergeSort(tempCopy.begin(), tempCopy.end(), first, distance, threadPool);

}

