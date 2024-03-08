#include "header.h"

int main() {
    TVector<TKeyValuePair> data;
    std::vector<TKeyValuePair> benchmarkData;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> distr(-100, 100);

    const std::size_t numberOfElements = 100000;

    for (size_t i = 0; i < numberOfElements; ++i) {
        data.PushBack(TKeyValuePair(distr(gen), "test"));
        benchmarkData.emplace_back(distr(gen), "test");

    }
    // Benchmarking bucket sort
    auto start = std::chrono::high_resolution_clock::now();
    BucketSort(data);
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Bucket sort time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " microseconds" << std::endl;




    // Benchmarking std::sort
    start = std::chrono::high_resolution_clock::now();
    std::stable_sort(benchmarkData.begin(), benchmarkData.end());
    end = std::chrono::high_resolution_clock::now();
    std::cout << "std::stable_sort time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " microseconds" << std::endl;



    return 0;
}