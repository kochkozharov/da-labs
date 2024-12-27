#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <set>
#include "pers_segtree.h"

int main() {
    int segmentCount, pointCount;
    std::cin >> segmentCount >> pointCount;

    std::vector<Segment> segments;
    std::vector<Point> points;
    std::vector<int> results(pointCount);
    std::set<int> uniqueHeights;
    std::map<int, int> versionMap;

    for (int i = 0; i < segmentCount; ++i) {
        int left, right, height;
        std::cin >> left >> right >> height;
        segments.emplace_back(left, height, 1);
        segments.emplace_back(right + 1, height, -1);
        uniqueHeights.insert(height);
    }

    std::map<int, int> heightCompression;
    int compressedIndex = 0;
    for (int height : uniqueHeights) {
        heightCompression[height] = compressedIndex++;
    }

    PersistentSegmentTree tree(compressedIndex);
    std::sort(segments.begin(), segments.end(), [](const Segment& a, const Segment& b) { return a.x < b.x; });

    for (const auto& segment : segments) {
        tree.Set(tree.Versions() - 1, segment.x, heightCompression[segment.height], segment.type);
        versionMap[segment.x] = tree.Versions() - 1;
    }

    for (int i = 0; i < pointCount; ++i) {
        int x, y;
        std::cin >> x >> y;
        points.emplace_back(x, y);
    }

    for (const auto& point : points) {
        auto itY = heightCompression.upper_bound(point.y);
        auto itX = versionMap.upper_bound(point.x);
        int versionIndex = (itX == versionMap.begin()) ? 0 : std::prev(itX)->second;
        int res;
        if (itY != heightCompression.end()) {
            res = tree.Sum(versionIndex, itY->second, compressedIndex - 1);
        } else {
            res = 0;
        }
        std::cout << res << "\n";
    }

    return 0;
}
