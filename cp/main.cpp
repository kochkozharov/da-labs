#include "iostream"
#include "map"
#include "pers_segtree.h"
#include "set"
#include <algorithm>
#include <iterator>
#include <vector>

struct TPoint {
    int x, y, id;
    TPoint(int x, int y) : x(x), y(y) {}
    friend bool operator<(const TPoint &lhs, const TPoint &rhs) {
        return lhs.id < rhs.id;
    }
};

struct TSegmentPoint : TPoint {
    bool isBegin;
    TSegmentPoint(int x, int y, bool isBegin)
        : TPoint(x, y), isBegin(isBegin) {}
};

int main() {
    size_t n, m;
    std::cin >> n >> m;

    std::vector<TSegmentPoint> segment;
    std::vector<TPoint> point;
    std::set<long long> uniqH;
    for (size_t i = 0; i < n; ++i) {
        long long l, r, y;
        std::cin >> l >> r >> y;
        segment.emplace_back(l, y, 1);
        segment.emplace_back(r, y, -1);
        uniqH.insert(y);
    }

    std::map<const long long, long long> compressY;
    int coord = 0;
    for (long long y : uniqH) {
        compressY[y] = coord++;
    }

    size_t maxY = uniqH.size();
    TPersistentSegtree tree(maxY);

    std::sort(segment.begin(), segment.end());

    for (auto &seg : segment) {
        tree.Add(compressY[seg.y], static_cast<long long>(seg.isBegin), seg.x);
    }

    for (int i = 0; i < m; ++i) {
        long long x, y;
        std::cin >> x >> y;
        point.emplace_back(x, y);
    }

    for (const auto &pt : point) {
        auto it = compressY.upper_bound(pt.y);
        size_t version = std::distance(std::lower_bound(segment.begin(), segment.end(), pt), segment.begin());
        size_t res;
        if (it != compressY.end()) {
            res = tree.Sum(it->second, maxY, version);
        }
        else {
            res = 0;
        }
        std::cout << res << "\n";
    }

    return 0;
}