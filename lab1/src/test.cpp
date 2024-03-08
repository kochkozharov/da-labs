#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>

#include <cassert>

template <class T>
class TVector {
   private:
    T* data{nullptr};
    std::size_t capacity{0}, size{0};

    static constexpr std::size_t NewCapacity(std::size_t capacity) {
        if (capacity == 0) {
            return 1;
        }
        return capacity <= std::numeric_limits<std::size_t>::max() / 2
                   ? capacity * 2
                   : std::numeric_limits<std::size_t>::max();
    }

   public:

    void Reserve(std::size_t newCapacity) {
        if (newCapacity < size) {
            return;
        }
        T* newArray = new T[newCapacity];
        for (size_t k = 0; k < size; ++k) newArray[k] = std::move(data[k]);

        capacity = newCapacity;
        std::swap(data, newArray);
        delete[] newArray;
    }

    void PushBack(const T& value) {
        if (capacity == size) {
            Reserve(NewCapacity(capacity));
        }
        data[size++] = value;
    }
    [[nodiscard]] constexpr std::size_t Size() const { return size; }
    [[nodiscard]] constexpr std::size_t Capacity() const { return size; }

    TVector() = default;

    TVector(std::size_t size) : data{new T[size]}, capacity{size} {}

    TVector(const TVector& other)
        : data(new T[other.capacity]),
          size(other.size),
          capacity(other.capacity) {
        for (std::size_t i = 0; i < size; ++i) {
            data[i] = other.data[i];
        }
    }

    TVector(TVector&& other) noexcept
        : data(other.data), size(other.size), capacity(other.capacity) {
        other.data = nullptr;
        other.size = 0;
        other.capacity = 0;
    }

    TVector& operator=(const TVector& other) {
        if (this == &other) {
            return other;
        }
        if (capacity < other.capacity) {
            data = new T[other.capacity];
            capacity = other.capacity;
        }
        for (std::size_t i = 0; i < other.size; ++i) {
            data[i] = other.data[i];
        }
        size = other.size;
        return *this;
    }

    TVector& operator=(TVector&& other) noexcept {
        delete[] data;
        data = other.data;
        size = other.size;
        capacity = other.capacity;
        other.data = nullptr;
        other.capacity = 0;
        other.size = 0;
        return *this;
    }

    T operator[](std::size_t idx) const { return data[idx]; }
    T& operator[](std::size_t idx) { return data[idx]; }

    virtual ~TVector() noexcept { delete[] data; }
};


template <class T>
void Merge(std::vector<T>& arr, std::vector<T>& buf, std::size_t left, std::size_t mid,
           std::size_t right) {
    std::size_t it1 = 0;
    std::size_t it2 = 0;
    while (left + it1 < mid && mid + it2 < right) {
        if (arr[left + it1] < arr[mid + it2]) {
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
    const double minElement = -100;
    const double maxElement = 100;
    const double range = maxElement - minElement;
    const size_t numBuckets = range;
    std::vector<std::vector<double>> buckets(numBuckets);

    // Distribute input array values into buckets
    for (double val : arr) {
        int bucketIndex = (val - minElement) * (numBuckets-1) / range; // Scale and shift to match bucket index
        buckets[bucketIndex].push_back(val);
    }

    // Sort each bucket and concatenate
    size_t cnt = 0;
    std::vector<double> buf(arr.size());
    for (auto& bucket : buckets) {
        std::stable_sort(bucket.begin(), bucket.end());
        //MergeSort(bucket, buf);
        //result.insert(result.end(), bucket.begin(), bucket.end());
        for (auto &el : bucket) {
            arr[cnt++] = el;
        }

    }

}

int main() {
    std::vector<double> benchmarkData;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> distr(-100, 100);

    const std::size_t numberOfElements = 100000;

    for (size_t i = 0; i < numberOfElements; ++i) {
        benchmarkData.push_back(distr(gen));
    }
    std::vector<double> benchmarkDataCopy = benchmarkData;
    // Benchmarking bucket sort
    auto start = std::chrono::high_resolution_clock::now();
    bucketSort(benchmarkData);
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Bucket sort time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " microseconds" << std::endl;




    // Benchmarking std::sort
    start = std::chrono::high_resolution_clock::now();
    std::stable_sort(benchmarkDataCopy.begin(), benchmarkDataCopy.end());
    end = std::chrono::high_resolution_clock::now();
    std::cout << "std::sort time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " microseconds" << std::endl;

    assert(std::is_sorted(benchmarkDataCopy.begin(), benchmarkDataCopy.end()));
    assert(std::is_sorted(benchmarkData.begin(), benchmarkData.end()));

    return 0;
}