#include <chrono>
#include <iostream>
#include <random>

#include "header.h"

int main() {
    TVector<TKeyValuePair> data;
    TVector<TKeyValuePair> data2;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> distr(-100, 100);
    for (size_t i = 100000; i <= 1000000; i+=100000) {
        const size_t numberOfElements = i;
        std::cout << numberOfElements << '\n';
        for (size_t i = 0; i < numberOfElements; ++i) {
            data.PushBack(TKeyValuePair(distr(gen), "test"));
            data2.PushBack(TKeyValuePair(distr(gen), "test"));
        }

        auto start = std::chrono::high_resolution_clock::now();
        BucketSort(data);
        auto end = std::chrono::high_resolution_clock::now();
        std::cout << "Bucket sort time: "
                  << std::chrono::duration_cast<std::chrono::microseconds>(
                         end - start)
                         .count()
                  << " microseconds" << std::endl;

        // Benchmarking std::sort
        TVector<TKeyValuePair> buf(data2.Size());
        start = std::chrono::high_resolution_clock::now();
        MergeSort(data2, buf);
        end = std::chrono::high_resolution_clock::now();
        std::cout << "merge sort: "
                  << std::chrono::duration_cast<std::chrono::microseconds>(
                         end - start)
                         .count()
                  << " microseconds" << std::endl;
    }

    return 0;
}