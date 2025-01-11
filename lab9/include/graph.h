#include <cstddef>
#include <limits>
#include <optional>
#include <queue>
#include <vector>

class Graph {
private:
    struct adjListElem {
        size_t adjNode;
        long long weight;
        adjListElem(size_t adjNode, size_t weight)
            : adjNode(adjNode), weight(weight) {}
        friend bool operator>(const adjListElem &lhs, const adjListElem &rhs) {
            return lhs.weight > rhs.weight;
        }
    };

    std::vector<std::vector<adjListElem>> adjList;
    std::vector<std::vector<long long>> adjMatrix;

    std::vector<long long> Dijkstra(size_t u) {
        size_t n = adjList.size();
        std::vector<long long> distances(n,
                                         std::numeric_limits<long long>::max());
        distances[u] = 0;
        std::priority_queue<adjListElem, std::vector<adjListElem>,
                            std::greater<adjListElem>>
            minHeap;
        minHeap.emplace(u, 0);
        std::vector<bool> visited(n, false);
        while (!minHeap.empty()) {
            adjListElem cur = minHeap.top();
            minHeap.pop();
            u = cur.adjNode;
            if (visited[u])
                continue;
            visited[u] = true;
            for (adjListElem el : adjList[u]) {
                size_t v = el.adjNode;
                long long w = el.weight;
                if (u == v)
                    continue;
                if (distances[u] < distances[v] - w) {
                    distances[v] = distances[u] + w;
                    minHeap.emplace(v, distances[v]);
                }
            }
        }
        return distances;
    }

    std::optional<std::vector<long long>> BellmanFord(size_t source) {
        size_t n = adjList.size();
        std::vector<long long> distances(n,
                                         std::numeric_limits<long long>::max());
        distances[source] = 0;

        for (size_t i = 0; i < n; ++i) {
            for (size_t u = 0; u < n; ++u) {
                for (const adjListElem &edge : adjList[u]) {
                    size_t v = edge.adjNode;
                    long long weight = edge.weight;
                    if (distances[u] < distances[v] - weight) {
                        if (i == n - 1) {
                            return std::nullopt;
                        }
                        distances[v] = distances[u] + weight;
                    }
                }
            }
        }

        return distances;
    }

public:
    Graph(size_t verticesCount) : adjList(verticesCount + 1), adjMatrix(
            verticesCount+1,
            std::vector<long long>(verticesCount+1, std::numeric_limits<long long>::max())) {
        //prepare adjList
        for (size_t i = 1; i < verticesCount + 1; ++i) {
            adjList[0].emplace_back(i, 0);
        }
        //prepare adjMatrix
        for (size_t i = 1; i < verticesCount+1; ++i) {
            adjMatrix[i][i] = 0;
        }
    }

    void AddEdge(size_t u, size_t v, long long weight) {
        adjList[u].emplace_back(v, weight);
        adjMatrix[u][v] = weight; 
    }

    std::optional<std::vector<std::vector<long long>>> Johnson() {
        auto potentials = BellmanFord(0);
        if (!potentials) {
            return std::nullopt;
        }
        for (size_t u = 1; u < adjList.size(); ++u) {
            for (auto &el : adjList[u]) {
                auto v = el.adjNode;
                if (el.weight != std::numeric_limits<long long>::max())
                    el.weight = el.weight + (*potentials)[u] - (*potentials)[v];
            }
        }
        std::vector<std::vector<long long>> res(
            adjList.size(), std::vector<long long>(adjList.size()));
        for (size_t i = 1; i < adjList.size(); ++i) {
            res[i] = Dijkstra(i);
            for (size_t j = 1; j < res[i].size(); ++j) {
                if (res[i][j] != std::numeric_limits<long long>::max())
                    res[i][j] = res[i][j] + (*potentials)[j] - (*potentials)[i];
            }
        }
        return res;
    }

    std::optional<std::vector<std::vector<long long>>> FloydWarshall() {
        size_t n = adjList.size();
        auto distance = adjMatrix;

        for (size_t k = 1; k < n; ++k) {
            for (size_t i = 1; i < n; ++i) {
                for (size_t j = 1; j < n; ++j) {
                    if (distance[i][k] != std::numeric_limits<long long>::max() &&
                        distance[k][j] != std::numeric_limits<long long>::max() &&
                        distance[i][j] > distance[i][k] + distance[k][j]) {
                        distance[i][j] = distance[i][k] + distance[k][j];
                    }
                }
            }
        }

        for (size_t i = 1; i < n; ++i) {
            if (distance[i][i] < 0) {
                return std::nullopt;
            }
        }

        return distance;
    }

    Graph() = delete;
};
