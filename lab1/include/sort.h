#include <cmath>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <limits>

#pragma region vector
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
    void PushBack(const T& value) {
        if (capacity == size) {
            const std::size_t newCapacity{NewCapacity(capacity)};
            T* newData{new T[newCapacity]};
            for (std::size_t i = 0; i < capacity; ++i) {
                newData[i] = std::move(data[i]);
            }
            capacity = newCapacity;
            std::swap(newData, data);
            delete[] newData;
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
#pragma endregion vector

#pragma region key-value pair

class TKeyValuePair {
   private:
    double key;
    static inline const int SIZE = 65;
    char value[SIZE]{};

   public:
    TKeyValuePair() {}
    TKeyValuePair(double key, const char* c_str) : key(key) {
        strcpy(value, c_str);
    }
    void Print(FILE* stream) { std::fprintf(stream, "%lf\t%s\n", key, value); }
    bool Scan(FILE* stream) {
        if (fscanf(stream, "%lf ", &key) == 1 &&
            fgets(value, sizeof(value), stream) != NULL) {
            value[strcspn(value, "\n")] = '\0';
            return true;
        }
        return false;
    }
    [[nodiscard]] int Key() const { return key; }
    friend bool operator<(const TKeyValuePair& a, const TKeyValuePair& b) {
        return a.key < b.key;
    }
    friend bool operator>(const TKeyValuePair& a, const TKeyValuePair& b) {
        return b < a;
    }
    friend bool operator<=(const TKeyValuePair& a, const TKeyValuePair& b) {
        return !(b > a);
    }
    friend bool operator>=(const TKeyValuePair& a, const TKeyValuePair& b) {
        return !(b < a);
    }
    friend bool operator==(const TKeyValuePair& a, const TKeyValuePair& b) {
        return a.key == b.key;
    }
    friend bool operator!=(const TKeyValuePair& a, const TKeyValuePair& b) {
        return !(a == b);
    }
};

#pragma endregion key - value pair

#pragma region sort

template <class T>
void InsertionSort(TVector<T>& vector) {
    for (std::size_t i = 1; i < vector.Size(); ++i) {
        T x = vector[i];
        std::size_t j = i;
        while (j > 0 && vector[j - 1] > x) {
            vector[j] = vector[j - 1];
            --j;
        }
        vector[j] = x;
    }
}

template <class T>
void Merge(TVector<T>& arr, TVector<T>& buf, std::size_t left, std::size_t mid,
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
void MergeSort(TVector<T>& arr, TVector<T>& buf) {
    std::size_t count = arr.Size();
    for (size_t i = 1; i < count; i *= 2) {
        for (size_t j = 0; j < count - i; j += 2 * i) {
            Merge(arr, buf, j, j + i, std::min(j + 2 * i, count));
        }
    }
}

void BucketSort(TVector<TKeyValuePair>& vector, size_t numBuckets) {
    const double minElement = -100;
    const double maxElement = 100;
    const double range = maxElement - minElement;
    TVector<TVector<TKeyValuePair>> buckets(numBuckets);
    for (std::size_t i = 0; i < vector.Size(); ++i) {
        std::size_t idx;
            idx =
                (vector[i].Key() - minElement) * numBuckets / range;
        buckets[idx].PushBack(vector[i]);
    }
    TVector<TKeyValuePair> buf(vector.Size());
    std::size_t cnt = 0;
    for (size_t i = 0; i < numBuckets; ++i) {
        MergeSort(buckets[i], buf);
        for (std::size_t j = 0; j < buckets[i].Size(); ++j) {
            vector[cnt] = buckets[i][j];
            ++cnt;
        }
    }
}

#pragma endregion sort