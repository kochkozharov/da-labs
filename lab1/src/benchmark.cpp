#include <algorithm>
#include <chrono>
#include <iostream>
#include <memory>
#include <random>
#include <vector>

#include "sort.h"


int main() {
    TVector<TKeyValuePair> data;
    std::vector<TKeyValuePair> benchmarkData;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> distr(-100, 100);

    std::size_t numberOfElements = 10000;
    for (size_t i = 0; i < numberOfElements; ++i) {
        auto pair = TKeyValuePair(distr(gen), "test");
        data.PushBack(pair);
        benchmarkData.push_back(pair);
    }
    const size_t numBuckets = 10000;
    auto start1 = std::chrono::high_resolution_clock::now();
    BucketSort(data, numBuckets);
    auto finish1 = std::chrono::high_resolution_clock::now();

    auto cmp = [](const TKeyValuePair& a, const TKeyValuePair& b) {
        return a.Key() < b.Key();
    };

    auto start3 = std::chrono::high_resolution_clock::now();
    std::stable_sort(benchmarkData.begin(), benchmarkData.end(), cmp);
    auto finish3 = std::chrono::high_resolution_clock::now();

    auto duration1 =
        std::chrono::duration_cast<std::chrono::microseconds>(finish1 - start1);
    auto duration3 =
        std::chrono::duration_cast<std::chrono::microseconds>(finish3 - start3);
    std::cout << "my sort duration: " << duration1.count() << std::endl;
    std::cout << "std stable_sort duration: " << duration3.count() << std::endl;

    return 0;
}

