#pragma once
#include <cctype>
#include <iostream>
#include <cstring>

const int BYTE_SIZE = 5;

struct CaseInsensitiveString {
   private:
    static const int KEY_LENGTH = 256;
    char str[KEY_LENGTH + 1];
    int sz = 0;

   public:
    CaseInsensitiveString() = default;
    
    CaseInsensitiveString(const char* value) {
        for (int i = 0; i < KEY_LENGTH && value[i] != '\0'; i++) {
            str[i] = std::tolower(value[i]);
            sz++;
        }
        str[sz] = '\0';
    }
    const char& operator[](int idx) const { return str[idx]; }
    int size() const { return sz; }
    bool Scan(FILE *stream) {
        return fscanf(stream,"%256s", str) != EOF;
    }
    const char *CStr() const{
        return str;
    }
    friend bool operator==(const CaseInsensitiveString& a, const CaseInsensitiveString& b) {
        return strcmp(a.str, b.str) == 0;
    }
};

int GetBitSize(const CaseInsensitiveString& str);

int BitDifference(const CaseInsensitiveString& a,
                  const CaseInsensitiveString& b);

bool GetBitByIndex(const CaseInsensitiveString& str, int index);
