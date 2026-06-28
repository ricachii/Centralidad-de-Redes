#pragma once
#include <queue>
#include <unordered_map>
#include "graph/Graph.h"

// BFS distances from src to every reachable vertex (src itself not included in result).
inline std::unordered_map<int, int> bfsDistances(const Graph& g, int src) {
    std::unordered_map<int, int> dist;
    dist.reserve(g.vertexCount());
    dist[src] = 0;
    std::queue<int> Q;
    Q.push(src);
    while (!Q.empty()) {
        int u = Q.front(); Q.pop();
        for (const auto& e : g.neighbors(u)) {
            if (!dist.count(e.to)) {
                dist[e.to] = dist[u] + 1;
                Q.push(e.to);
            }
        }
    }
    return dist;
}
