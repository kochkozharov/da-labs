#include <cstddef>
#include <iostream>
#include <sstream>
#include <vector>

using ll = long long;

std::size_t RelativeToAbsolute(const std::pair<size_t, size_t> &relativeIndex,
                               const std::vector<std::size_t> &prefixSums) {
    return prefixSums[relativeIndex.first] + relativeIndex.second;
}

std::pair<size_t, size_t>
AbsoluteToRelative(std::size_t absoluteIndex,
                   const std::vector<std::size_t> &prefixSums) {
    size_t lineIndex = std::lower_bound(prefixSums.begin(), prefixSums.end(),
                                        absoluteIndex + 1) -
                       prefixSums.begin();
    size_t numberIndex =
        absoluteIndex - (lineIndex > 0 ? prefixSums[lineIndex - 1] : 0) + 1;
    return std::make_pair(lineIndex, numberIndex);
}

std::vector<std::size_t> ZFunction(const std::vector<ll> &arr) {
    std::size_t n = arr.size();
    std::vector<std::size_t> z(arr.size());
    for (std::size_t i = 1, l = 0, r = 0; i < arr.size(); ++i) {
        if (i <= r) {
            z[i] = std::min(r - i + 1, z[i - l]);
        }
        while (i + z[i] < n && arr[z[i]] == arr[i + z[i]]) {
            ++z[i];
        }
        if (i + z[i] - 1 > r) {
            l = i;
            r = i + z[i] - 1;
        }
    }
    return z;
}

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
