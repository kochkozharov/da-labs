#include "binary_string.h"

#include <climits>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))

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
    for (int i = 0; i < MAX(GetBitSize(a), GetBitSize(b)); ++i) {
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