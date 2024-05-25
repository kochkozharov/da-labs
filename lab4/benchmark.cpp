#include "zfunc.h"
#include <chrono>
#include <cstddef>
#include <iostream>
#include <random>
#include <vector>
#include <cassert>
#include <climits>

using duration_t = std::chrono::microseconds;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "lab2_benchmark [NUM_OF_KEYS]");
        exit(-1);
    }
    char *p;
    size_t textLen = strtoul(argv[1], &p, 10);
    if (errno != 0 || *p != '\0' || textLen < 10) {
        fprintf(stderr, "Wrong arg");
        exit(-1);
    }
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<unsigned char> dis(
        0, std::numeric_limits<unsigned char>::max());
    const size_t patternLen = 2;     // Number of elements in the vector
    
    std::vector<long long> pattern(patternLen);
    for (size_t i = 0; i < patternLen; ++i) {
        pattern[i] = dis(gen);
    }
    std::vector<long long> text(textLen);
    for (size_t i = 0; i < textLen; ++i) {
        text[i] = dis(gen);
    }
    std::vector<size_t> res1;
    std::vector<size_t> res2;
    long t1, t2;
    {
        auto start = std::chrono::system_clock::now();
        auto it = std::search(text.begin(), text.end(),
                              pattern.begin(), pattern.end());
        while (it != text.end()) {
            res1.push_back(std::distance(text.begin(), it));
            it = std::search(std::next(it), text.end(), pattern.begin(),
                             pattern.end());
        }
        auto end = std::chrono::system_clock::now();
        t1 = std::chrono::duration_cast<duration_t>(end - start).count();
    }
    {
        std::vector<long long> preparedText;
        preparedText.reserve(patternLen + 1 + textLen);
        preparedText.insert(preparedText.end(), pattern.begin(), pattern.end());
        preparedText.push_back(-1);
        preparedText.insert(preparedText.end(), text.begin(), text.end());
        auto start = std::chrono::system_clock::now();
        std::vector<std::size_t> zFun = ZFunction(preparedText);
        for (std::size_t i = patternLen + 1; i < zFun.size(); ++i) {
            if (zFun[i] == patternLen) {
                res2.push_back(i -patternLen - 1);
            }
        }
        auto end = std::chrono::system_clock::now();
        t2 = std::chrono::duration_cast<duration_t>(end - start).count();
    }
    std::cout << "matches: " << res2.size() << '\n';
    assert(res1 == res2);
    std::cout << "std: " << t1 << '\n';
    std::cout << "z-function: " << t2 << '\n';
    return 0;
}