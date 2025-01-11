#include <chrono>
#include <random>
#include <iostream>
#include "graph.h"

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <number_of_nodes> <number_of_edges>\n";
        return 1;
    }

    size_t n = std::strtoull(argv[1], nullptr, 10);
    size_t m = std::strtoull(argv[2], nullptr, 10);


    Graph graph(n);

    std::mt19937 rng(42);
    std::uniform_int_distribution<size_t> nodeDist(1, n);
    std::uniform_int_distribution<long long> weightDist(0, 1000);

    for (size_t i = 0; i < m; ++i) {
        size_t u = nodeDist(rng);
        size_t v = nodeDist(rng);
        long long w = weightDist(rng);
        graph.AddEdge(u, v, w);
    }

    auto start = std::chrono::high_resolution_clock::now();
    auto johnsonResult = graph.Johnson();
    auto end = std::chrono::high_resolution_clock::now();
    auto johnsonDuration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    if (!johnsonResult) {
        std::cout << "Negative cycle detected in Johnson algorithm\n";
    } else {
        std::cout << "Johnson algorithm completed in " << johnsonDuration << " ms\n";
    }

    start = std::chrono::high_resolution_clock::now();
    auto floydWarshallResult = graph.FloydWarshall();
    end = std::chrono::high_resolution_clock::now();
    auto floydDuration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "Floyd-Warshall algorithm completed in " << floydDuration << " ms\n";

    return 0;
}