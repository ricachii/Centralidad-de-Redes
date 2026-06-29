#pragma once
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <stdexcept>

// Grafo con lista de adyacencia. Soporta dirigido/no dirigido y pesos opcionales.
class Graph {
public:
    struct Edge {
        int   to;
        double weight;
    };

    explicit Graph(bool directed = false);

    // Construccion
    void addVertex(int id);
    void addEdge(int from, int to, double weight = 1.0);
    void removeEdge(int from, int to);

    // Consultas
    bool hasVertex(int id)          const;
    bool hasEdge(int from, int to)  const;
    bool isDirected()               const { return directed_; }

    int    vertexCount() const;
    long long edgeCount()   const;

    const std::vector<Edge>& neighbors(int v) const;
    std::vector<int>         vertices()       const;

    // Uso de memoria en bytes (aproximado)
    size_t memoryBytes() const;

private:
    bool directed_;
    std::unordered_map<int, std::vector<Edge>> adj_;
    long long edgeCount_ = 0;
};
