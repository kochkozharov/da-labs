#include <algorithm>
#include <iostream>
#include <vector>

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

int main() {
    int n, m;
    std::cin >> m >> n;
    std::vector<int> res;
    std::vector<Addition> additions(m, Addition(n));
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            std::cin >> additions[i].ratios[j];
        }
        std::cin >> additions[i].price;
        additions[i].index = i;
    }
    for (int i = 0; i < n; ++i) {
        int index = FindLowestPriceRow(additions, i);
        if (index == -1) {
            std::cout << "-1" << std::endl;
            return 0;
        }

        std::swap(additions[i], additions[index]);
        res.push_back(additions[i].index);
        SubtractRows(additions, i);
    }

    std::sort(res.begin(), res.end());
    for (auto r : res) {
        std::cout << r + 1 << ' ';
    }
    std::cout << '\n';
    return 0;
}