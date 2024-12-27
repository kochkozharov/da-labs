#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <set>
#include <pers_segtree.h>

struct Segments {
    int x, h, type;
    Segments(int x, int h, int type) : x(x), h(h), type(type) {}
    friend bool operator<(const Segments& a, const Segments& b) {
        return a.x < b.x;
    }
};

struct Points {
    int x, y, id;
    Points(int x, int y, int id) : x(x), y(y), id(id) {}
};


int main() {
    int n, m;
    std::cin >> n >> m;

    std::vector<Segments> segment;
    std::vector<Points> point;
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
        tree.update(seg.x, compressY[seg.h], seg.type);
    }

    for (int i = 0; i < m; ++i) {
        int x, y;
        std::cin >> x >> y;
        point.emplace_back(x, y, i);
    }

    for (const auto& pt : point) {
        auto it = compressY.upper_bound(pt.y);
        if (it != compressY.end()) {

            auto segIt = std::upper_bound(
                segment.begin(), segment.end(), pt.x,
                [](int x, const auto& seg) {
                    return x < seg.x;
                }
            );

            int low = std::distance(segment.begin(), segIt) - 1;

            results[pt.id] = tree.query(low, it->second, maxY - 1);
        } else {
            results[pt.id] = 0;
        }
    }

    for (const auto& res : results) {
        std::cout << res << "\n";
    }

    return 0;
}