#pragma once
#include <cctype>
#include <cstdio>
#include <cstring>

struct TCaseInsensitiveString {
   private:
    static const int KEY_LENGTH = 256;
    char str[KEY_LENGTH + 1];
    int sz = 0;

   public:
    TCaseInsensitiveString() = default;
    TCaseInsensitiveString(const char* value) {
        for (int i = 0; i < KEY_LENGTH && value[i] != '\0'; ++i) {
            str[i] = std::tolower(value[i]);
            sz++;
        }
        str[sz] = '\0';
    }
    const char& operator[](int idx) const { return str[idx]; }
    int size() const { return sz; }
    bool Scan(FILE* stream = stdin) {
        int res = fscanf(stream, "%256s", str);
        if (res == EOF) {
            return false;
        }
        sz = strlen(str);
        for (int i = 0; i < sz; ++i) {
            str[i] = std::tolower(str[i]);
        }
        return true;
    }
    const char* CStr() const { return str; }
    friend bool operator==(const TCaseInsensitiveString& a,
                           const TCaseInsensitiveString& b) {
        return strcmp(a.str, b.str) == 0;
    }
};

int GetBitSize(const TCaseInsensitiveString& str);

int GetBitDifference(const TCaseInsensitiveString& a,
                  const TCaseInsensitiveString& b);

bool GetBitByIndex(const TCaseInsensitiveString& str, int index);
