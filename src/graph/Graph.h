#pragma once
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <stdexcept>

// Adjacency-list graph. Supports directed/undirected and optional edge weights.
class Graph {
public:
    struct Edge {
        int   to;
        double weight;
    };

    explicit Graph(bool directed = false);

    // Construction
    void addVertex(int id);
    void addEdge(int from, int to, double weight = 1.0);
    void removeEdge(int from, int to);

    // Queries
    bool hasVertex(int id)          const;
    bool hasEdge(int from, int to)  const;
    bool isDirected()               const { return directed_; }

    int    vertexCount() const;
    long long edgeCount()   const;

    const std::vector<Edge>& neighbors(int v) const;
    std::vector<int>         vertices()       const;

    // Memory footprint in bytes (approximate)
    size_t memoryBytes() const;

private:
    bool directed_;
    std::unordered_map<int, std::vector<Edge>> adj_;
    long long edgeCount_ = 0;
};
