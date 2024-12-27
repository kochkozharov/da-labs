#include <vector>
#include <algorithm>
#include <map>
#include <set>
#include <ctime>
#include <iostream>
#include <chrono>
#include "pers_segtree.h"


void runPersistentTree(int n, int m) {
    auto start = std::chrono::high_resolution_clock::now(); 
    std::srand(std::time(nullptr));

    std::vector<Segment> segments;
    std::vector<Point> points;
    std::vector<int> results(m);
    std::set<int> uniqueHeights;
    std::map<int, int> versionMap;

    for (int i = 0; i < n; ++i) {
        int left = std::rand() % 100;
        int right = left + (std::rand() % 50);
        int height = std::rand() % 100;
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

    for (int i = 0; i < m; ++i) {
        int x = std::rand() % 100;
        int y = std::rand() % 100;
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
    }
    auto end = std::chrono::high_resolution_clock::now(); 
    std::chrono::duration<double> duration = end - start; 
    std::cout << "Time taken for n = " << n << " and m = " << m << ": " << duration.count() << " seconds\n";
}

int main() {
    runPersistentTree(1000, 1000);
    runPersistentTree(5000, 5000);
    runPersistentTree(10000, 10000);
    runPersistentTree(30000, 30000);
    runPersistentTree(50000, 50000);
    runPersistentTree(70000, 70000);
    runPersistentTree(100000, 100000);
    return 0;
}