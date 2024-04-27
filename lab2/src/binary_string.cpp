#include "binary_string.h"

#include <climits>
#include <cmath>
#include <iostream>

/*
int GetBitSize(const CaseInsensitiveString& str) {
    return str.size() * BYTE_SIZE;
}

// get first bit of a number which is not 0 (from the left)
static int GetBitDifferenceNumber(char a) {
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

int BitDifference(const CaseInsensitiveString& a, const CaseInsensitiveString&
b) { int sizeA = a.size(); int sizeB = b.size(); for (int i = 0; i <
std::min(sizeA, sizeB); ++i) { int differenceIndex =
GetBitDifferenceNumber((a[i] - 'a' + 1) ^ (b[i] - 'a' + 1)); if (differenceIndex
!= -1) { return differenceIndex + BYTE_SIZE * i;
        }
    }
    if (sizeA == sizeB) {
        return -1;
    }
    if (sizeA > sizeB) {
        return GetBitDifferenceNumber(a[sizeB] - 'a' + 1) + BYTE_SIZE * sizeB;
    }
    return GetBitDifferenceNumber(b[sizeA] - 'a' + 1) + BYTE_SIZE * sizeA;
}

bool GetBitByIndex(const CaseInsensitiveString& str, int index) {
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

*/

int GetBitSize(const CaseInsensitiveString& str) {
    return str.size() * CHAR_BIT;
}

bool GetBitByIndex(const CaseInsensitiveString& str, int index) {
    const char* ptr = str.CStr();
    int byteIndex = index / CHAR_BIT;
    int bitIndex = index % CHAR_BIT;
    return ptr[byteIndex] & (1 << bitIndex);
}

int BitDifference(const CaseInsensitiveString& a,
                  const CaseInsensitiveString& b) {
    int n = 0;
    for (int i = 0; i < std::max(GetBitSize(a), GetBitSize(b)); ++i) {
        if (GetBitByIndex(a, i) != GetBitByIndex(b, i)) {
            break;
        }
        n++;
    }
    if (a.size() == b.size() && n == GetBitSize(a)) {
        return -1;
    }
    return n;
}