#include <cstring>
#include <utility>
#include <cstdio>
#include <limits>
#include <algorithm>

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

    void PushBack(T&& value) {
        if (capacity == size) {
            Reserve(NewCapacity(capacity));
        }
        data[size++] = std::move(value);
    }

    template <typename... Args>
    void EmplaceBack(Args&&... args) {
        if (size == capacity) Reserve(NewCapacity(capacity));
        new (data + size++) T(std::forward<Args>(args)...);
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
void Merge(TVector<T>& arr, TVector<T>& buf, std::size_t left, std::size_t mid,
           std::size_t right) {
    std::size_t it1 = 0;
    std::size_t it2 = 0;
    while (left + it1 < mid && mid + it2 < right) {
        if (arr[left + it1] <= arr[mid + it2]) {
            buf[it1 + it2] = std::move(arr[left + it1]);
            it1 += 1;
        } else {
            buf[it1 + it2] = std::move(arr[mid + it2]);
            it2 += 1;
        }
    }
    while (left + it1 < mid) {
        buf[it1 + it2] = std::move(arr[left + it1]);
        it1 += 1;
    }

    while (mid + it2 < right) {
        buf[it1 + it2] = std::move(arr[mid + it2]);
        it2 += 1;
    }
    for (std::size_t i = 0; i < it1 + it2; ++i) {
        arr[left + i] = std::move(buf[i]);
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

struct TFixedSizeLine {
    inline static const int SIZE = 64;
    char str[SIZE + 1];
    TFixedSizeLine() = default;
    TFixedSizeLine(const char* a) { strcpy(str, a); }
    operator const char*() { return str; }
};

class TKeyValuePair {
   public:
    double key;
    TFixedSizeLine* value;
    TKeyValuePair() : value(nullptr) {}
    TKeyValuePair(int key, const char* value)
        : key(key), value(new TFixedSizeLine(value)) {}
    TKeyValuePair(const TKeyValuePair& other) : key(other.key) {
        value = new TFixedSizeLine();
        *value = *other.value;
    }
    TKeyValuePair& operator=(const TKeyValuePair& other) {
        delete value;
        value = new TFixedSizeLine();
        *value = *other.value;
        key = other.key;
        return *this;
    }
    TKeyValuePair& operator=(TKeyValuePair&& other) {
        delete value;
        value = other.value;
        other.value = nullptr;
        key = other.key;
        return *this;
    }
    TKeyValuePair(TKeyValuePair&& other) : key(other.key) {
        value = other.value;
        other.value = nullptr;
    }

    ~TKeyValuePair() { delete value; }
    void Print(FILE* stream) {
        std::fprintf(stream, "%lf\t%s\n", key, value->str);
    }
    bool Scan(FILE* stream) {
        if (!value) value = new TFixedSizeLine();
        if (fscanf(stream, "%lf ", &key) == 1 &&
            fgets(value->str, sizeof(value->str), stream) != nullptr) {
            value->str[strcspn(value->str, "\n")] = '\0';
            return true;
        }
        return false;
    }
};

bool operator<(const TKeyValuePair& a, const TKeyValuePair& b) {
    return a.key < b.key;
}
bool operator>=(const TKeyValuePair& a, const TKeyValuePair& b) {
    return !(a < b);
}

bool operator>(const TKeyValuePair& a, const TKeyValuePair& b) { return b < a; }

bool operator<=(const TKeyValuePair& a, const TKeyValuePair& b) {
    return !(a > b);
}

void BucketSort(TVector<TKeyValuePair>& arr) {
    const int minElement = -100;
    const int maxElement = 100;
    const int range = maxElement - minElement;
    const size_t numBuckets = range;
    TVector<TVector<TKeyValuePair>> buckets(numBuckets);
    for (size_t i = 0; i < arr.Size(); ++i) {
        int bucketIndex = (arr[i].key - minElement) * (numBuckets - 1) / range;
        buckets[bucketIndex].PushBack(std::move(arr[i]));
    }

    size_t cnt = 0;
    TVector<TKeyValuePair> buf(arr.Size());
    for (size_t i = 0; i < numBuckets; ++i) {
        MergeSort(buckets[i], buf);
        for (size_t j = 0; j < buckets[i].Size(); ++j) {
            arr[cnt++] = std::move(buckets[i][j]);
        }
    }
}