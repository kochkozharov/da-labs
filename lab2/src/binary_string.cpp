#include "binary_string.h"
#include <iostream>
#include <cmath>

int GetBitSize(const std::string& str) {
    return str.size() * BYTE_SIZE;
}

// get first bit of a number which is not 0 (from the left)
int GetBitDifferenceNumber(char a) {
    if (a == 0) {
        return -1;
    }
    int bits = 0;
    while (a > 1) {
        a >>= 1;
        ++bits;
    }
    return BYTE_SIZE - bits - 1;
}

int BitDifference(const std::string& a, const std::string& b) {
    int sizeA = a.size();
    int sizeB = b.size();
    for (int i = 0; i < std::min(sizeA, sizeB); ++i) {
        int differenceIndex = GetBitDifferenceNumber((std::tolower(a[i]) - 'a' + 1) ^ (std::tolower(b[i]) - 'a' + 1));
        if (differenceIndex != -1) {
            return differenceIndex + BYTE_SIZE * i;
        }
    }
    if (sizeA == sizeB) {
        return -1;
    }
    if (sizeA > sizeB) {
        return GetBitDifferenceNumber(std::tolower(a[sizeB]) - 'a' + 1) + BYTE_SIZE * sizeB;
    }
    return GetBitDifferenceNumber(std::tolower(b[sizeA]) - 'a' + 1) + BYTE_SIZE * sizeA;
}

bool GetBitByIndex(const std::string& str, int index) {
    if (index < 0) {
        throw std::out_of_range("index is out of range");
    }
    if (index >= GetBitSize(str)) {
        return 0;
    }
    int letterIndex = index / BYTE_SIZE;
    char letter = std::tolower(str[letterIndex]) - 'a' + 1;
    int bitMask = 1 << (BYTE_SIZE - 1 - index % BYTE_SIZE);
    return (letter & bitMask) >> (BYTE_SIZE - 1 - index % BYTE_SIZE);
}
