#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>


template <class T>
void Merge(std::vector<T>& arr, std::vector<T>& buf, std::size_t left, std::size_t mid,
           std::size_t right) {
    std::size_t it1 = 0;
    std::size_t it2 = 0;
    while (left + it1 < mid && mid + it2 < right) {
        if (arr[left + it1] >= arr[mid + it2]) {
            buf[it1 + it2] = arr[left + it1];
            it1 += 1;
        } else {
            buf[it1 + it2] = arr[mid + it2];
            it2 += 1;
        }
    }
    while (left + it1 < mid) {
        buf[it1 + it2] = arr[left + it1];
        it1 += 1;
    }

    while (mid + it2 < right) {
        buf[it1 + it2] = arr[mid + it2];
        it2 += 1;
    }
    for (std::size_t i = 0; i < it1 + it2; ++i) {
        arr[left + i] = buf[i];
    }
}

template <class T>
void MergeSort(std::vector<T>& arr, std::vector<T>& buf) {
    std::size_t count = arr.size();
    for (size_t i = 1; i < count; i *= 2) {
        for (size_t j = 0; j < count - i; j += 2 * i) {
            Merge(arr, buf, j, j + i, std::min(j + 2 * i, count));
        }
    }
}

// Function to perform bucket sort
void bucketSort(std::vector<double>& arr) {
    int numBuckets = 200; // Adjust based on granularity
    std::vector<std::vector<double>> buckets(numBuckets);

    // Distribute input array values into buckets
    for (double val : arr) {
        int bucketIndex = (val + 100) * numBuckets / 200; // Scale and shift to match bucket index
        buckets[bucketIndex].push_back(val);
    }

    // Sort each bucket and concatenate
    size_t cnt = 0;
    std::vector<double> buf(arr.size());
    for (auto& bucket : buckets) {
        //std::stable_sort(bucket.begin(), bucket.end());
        MergeSort(bucket, buf);
        //result.insert(result.end(), bucket.begin(), bucket.end());
        for (auto &el : bucket) {
            arr[cnt] = el;
        }

    }

}

int main() {
    std::vector<double> benchmarkData;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> distr(-100, 100);

    const std::size_t numberOfElements = 10000;

    for (size_t i = 0; i < numberOfElements; ++i) {
        benchmarkData.push_back(distr(gen));
    }

    // Benchmarking bucket sort
    auto start = std::chrono::high_resolution_clock::now();
   bucketSort(benchmarkData);
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Bucket sort time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " microseconds" << std::endl;

    // Benchmarking std::sort
    start = std::chrono::high_resolution_clock::now();
    std::sort(benchmarkData.begin(), benchmarkData.end());
    end = std::chrono::high_resolution_clock::now();
    std::cout << "std::sort time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " microseconds" << std::endl;
    return 0;
}