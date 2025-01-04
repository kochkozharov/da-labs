#include <cstddef>
#include <iostream>
#include "graph.h"

int main() {
    size_t n, m;
    std::cin >> n >> m;
    Graph graph(n);
    for (size_t i = 0; i < m; ++i) {
        size_t u,v;
        long long w;
        std::cin >> u >> v >> w;
        graph.AddEdge(u, v, w);
    }
    auto allDistances = graph.Johnson();
    if (!allDistances) {
        std::cout << "Negative cycle\n";
        return 0;
    }
    for (size_t i = 1; i < n+1; ++i) {
        for (size_t j = 1; j < n+1; ++j) {
            auto x =(*allDistances)[i][j];
            if (x == std::numeric_limits<long long>::max()) {
                std::cout << "inf ";
            }
            else {
                std::cout << x << ' ';
            }
        }
        std::cout << '\n';
    }
}