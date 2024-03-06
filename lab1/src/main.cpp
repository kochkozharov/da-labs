#include "sort.h"

#pragma region main
int main() {
    TVector<TKeyValuePair> arr;
    TKeyValuePair pair;
    while (pair.Scan(stdin)) {
        arr.PushBack(pair);
    }
    const int numBuckets = 2000;
    BucketSort(arr, numBuckets);
    for (std::size_t i = 0; i < arr.Size(); ++i) {
        arr[i].Print(stdout);
    }
}
#pragma endregion main