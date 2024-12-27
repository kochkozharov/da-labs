#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <set>
#include <pers_segtree.h>

struct TPoint {
    int x, y;
    TPoint(int x, int y) : x(x), y(y) {}
    friend bool operator<(const TPoint &lhs, const TPoint &rhs) {
        return lhs.x < rhs.x;
    }
};
struct TSegmentPoint : TPoint {
    bool isBegin;
    TSegmentPoint(int x, int y, bool isBegin)
        : TPoint(x, y), isBegin(isBegin) {}
};


int main() {
    int n, m;
    std::cin >> n >> m;

    std::vector<TSegmentPoint> segment;
    std::vector<TPoint> point;
    std::vector<int> results(m);

    std::set<int> uniqH;

    for (int i = 0; i < n; ++i) {
        int l, r, h;
        std::cin >> l >> r >> h;
        segment.emplace_back(l, h, 1);
        segment.emplace_back(r + 1, h, -1);
        uniqH.insert(h);
    }

    std::map<int, int> compressY;
    int coord = 0;
    for (int y : uniqH) {
        compressY[y] = coord++;
    }

    int maxY = uniqH.size();
    PersistentSegmentTree tree(maxY);

    std::sort(segment.begin(), segment.end());

    for (auto& seg : segment) {
        tree.update(seg.x, compressY[seg.y], static_cast<int>(seg.isBegin));
    }

    for (int i = 0; i < m; ++i) {
        int x, y;
        std::cin >> x >> y;
        point.emplace_back(x, y);
    }

    for (const auto& pt : point) {
        auto it = compressY.upper_bound(pt.y);
        int res;
        if (it != compressY.end()) {

            auto segIt = std::upper_bound(
                segment.begin(), segment.end(), pt.x,
                [](int x, const auto& seg) {
                    return x < seg.x;
                }
            );

            int low = std::distance(segment.begin(), segIt) - 1;

            res = tree.query(low, it->second, maxY - 1);
        } else {
            res =  0;
        }
        std::cout << res << "\n";
        
    }

    return 0;
}