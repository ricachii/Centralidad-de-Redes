#include <cassert>
#include <iostream>
#include "graph/Graph.h"

void testBasicOperations() {
    // Path graph: 1 -- 2 -- 3 (no edge between 1 and 3)
    Graph g(false);
    g.addEdge(1, 2);
    g.addEdge(2, 3);

    assert(g.vertexCount() == 3);
    assert(g.edgeCount()   == 2);
    assert(g.hasEdge(1, 2));
    assert(g.hasEdge(2, 1)); // undirected
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
    assert(!g.hasEdge(2, 1)); // directed
    std::cout << "testDirected PASSED\n";
}

int main() {
    testBasicOperations();
    testDirected();
    std::cout << "All graph tests passed.\n";
    return 0;
}
