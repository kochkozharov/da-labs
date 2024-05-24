#include <cstddef>
#include <iostream>
#include <sstream>
#include <vector>

#include "zfunc.h"

int main() {
    std::vector<ll> preparedText;
    std::string line;
    std::getline(std::cin, line);
    uint num;
    std::istringstream iss{line};
    std::size_t patternSize = 0;
    while (iss >> num) {
        patternSize++;
        preparedText.push_back(num);
    }
    preparedText.push_back(-1);
    std::vector<std::size_t> prefixSums(1);
    while (std::getline(std::cin, line)) {
        std::istringstream iss{line};
        std::size_t cnt = 0;
        while (iss >> num) {
            preparedText.push_back(num);
            cnt++;
        }
        prefixSums.push_back(prefixSums.back()+cnt);
    }

    std::vector<std::size_t> zFun = ZFunction(preparedText);
    for (std::size_t i = patternSize+1; i < zFun.size(); ++i) {
        if (zFun[i] == patternSize) {
            std::pair<size_t, size_t> match = AbsoluteToRelative(i-patternSize-1, prefixSums);
            std::cout << match.first << ", " << match.second;
            std::cout << '\n';
        }
    }
}
