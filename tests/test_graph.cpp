#include <cassert>
#include <iostream>
#include <cmath>
#include "graph/Graph.h"

void testBasicOperations() {
    // Grafo camino: 1 -- 2 -- 3 (sin arista entre 1 y 3)
    Graph g(false);
    g.addEdge(1, 2);
    g.addEdge(2, 3);

    assert(g.vertexCount() == 3);
    assert(g.edgeCount()   == 2);
    assert(g.hasEdge(1, 2));
    assert(g.hasEdge(2, 1)); // no dirigido
    assert(!g.hasEdge(1, 3));

    g.removeEdge(1, 2);
    assert(!g.hasEdge(1, 2));
    assert(g.edgeCount() == 1);

    std::cout << "testBasicOperations PASSED\n";
}

void testDirected() {
    Graph g(true);
    g.addEdge(1, 2);
    assert( g.hasEdge(1, 2));
    assert(!g.hasEdge(2, 1)); // dirigido
    assert(g.isDirected());
    std::cout << "testDirected PASSED\n";
}

void testVertexOperations() {
    Graph g(false);
    g.addVertex(5);   // vertice aislado, sin aristas
    g.addEdge(1, 2);

    assert(g.hasVertex(5));
    assert(g.hasVertex(1));
    assert(!g.hasVertex(99));
    assert(!g.isDirected());

    // vertices() debe listar los 3 ids: 5, 1, 2
    assert(g.vertexCount() == 3);
    assert(g.vertices().size() == 3);
    std::cout << "testVertexOperations PASSED\n";
}

void testNeighborsAndWeights() {
    Graph g(false);
    g.addEdge(1, 2, 7.5);
    g.addEdge(1, 3);   // peso por defecto 1.0

    // El nodo 1 tiene 2 vecinos
    const auto& nb = g.neighbors(1);
    assert(nb.size() == 2);

    // La arista 1-2 conserva su peso, y al ser no dirigida tambien en sentido inverso
    bool found = false;
    for (const auto& e : nb)
        if (e.to == 2) { assert(std::abs(e.weight - 7.5) < 1e-9); found = true; }
    assert(found);
    assert(std::abs(g.neighbors(2)[0].weight - 7.5) < 1e-9);
    std::cout << "testNeighborsAndWeights PASSED\n";
}

void testMemory() {
    Graph g(false);
    g.addEdge(1, 2);
    assert(g.memoryBytes() > 0);
    std::cout << "testMemory PASSED\n";
}

int main() {
    testBasicOperations();
    testDirected();
    testVertexOperations();
    testNeighborsAndWeights();
    testMemory();
    std::cout << "All graph tests passed.\n";
    return 0;
}
