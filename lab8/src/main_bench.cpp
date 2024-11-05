#include <algorithm>
#include <iostream>
#include <vector>
#include <chrono>

using duration_t = std::chrono::milliseconds;

constexpr const int MAX_NUM = 50;

struct Addition {
    Addition(size_t n) : ratios(n) {}
    std::vector<double> ratios;
    int price;
    int index;
};

int FindLowestPriceRow(std::vector<Addition> &v, int t) {
    size_t m = v.size();
    size_t n = v[0].ratios.size();
    int minPrice = MAX_NUM + 1;
    int index = -1;
    for (int i = t; i < m; ++i) {
        if ((v[i].ratios[t] != 0.0) && (v[i].price < minPrice)) {
            index = i;
            minPrice = v[i].price;
        }
    }
    return index;
}

void SubtractRows(std::vector<Addition> &v, int t) {
    size_t m = v.size();
    size_t n = v[0].ratios.size();
    for (int i = t + 1; i < m; ++i) {
        double coeff = v[i].ratios[t] / v[t].ratios[t];
        for (int j = t; j < n; ++j) {
            v[i].ratios[j] -= v[t].ratios[j] * coeff;
        }
    }
}

std::vector<int> Solve(std::vector<Addition> &additions) {
    size_t m = additions.size();
    size_t n = additions[0].ratios.size();
    std::vector<int> res;

    for (int i = 0; i < n; ++i) {
        int index = FindLowestPriceRow(additions, i);
        if (index == -1) {
            return {};
        }

        std::swap(additions[i], additions[index]);
        res.push_back(additions[i].index);
        SubtractRows(additions, i);
    }

    std::sort(res.begin(), res.end());
    return res;
}

int main() {
    int n, m;
    std::cin >> m >> n;
    std::vector<Addition> additions(m, Addition(n));

    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            std::cin >> additions[i].ratios[j];
        }
        std::cin >> additions[i].price;
        additions[i].index = i;
    }

    auto start = std::chrono::system_clock::now();

    std::vector<int> res = Solve(additions);

    auto end = std::chrono::system_clock::now();
    auto t1 = std::chrono::duration_cast<duration_t>(end - start).count();

    if (res.empty()) {
        std::cout << "-1\n";
        return 0;
    }

    for (auto r : res) {
        std::cout << r + 1 << ' ';
    }
    std::cout << '\n';
    std::cout << t1 << "ms\n";
    return 0;
}