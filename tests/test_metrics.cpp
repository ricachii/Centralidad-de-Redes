#include <cassert>
#include <iostream>
#include <cmath>
#include "graph/Graph.h"
#include "metrics/DegreeCentrality.h"
#include "metrics/BetweennessCentrality.h"
#include "metrics/ClosenessCentrality.h"
#include "metrics/PageRank.h"
#include "metrics/AverageShortestPath.h"
#include "metrics/EigenvectorCentrality.h"
#include "metrics/ClusteringCoefficient.h"

// Grafo camino simple: 1 -- 2 -- 3 -- 4
static Graph pathGraph() {
    Graph g(false);
    g.addEdge(1, 2);
    g.addEdge(2, 3);
    g.addEdge(3, 4);
    return g;
}

void testDegree() {
    auto g = pathGraph();
    DegreeCentrality dc(false);
    auto scores = dc.compute(g);
    assert(scores[1] == 1);
    assert(scores[2] == 2);
    std::cout << "testDegree PASSED\n";
}

void testBetweenness() {
    auto g = pathGraph();
    BetweennessCentrality bc(false);
    auto scores = bc.compute(g);
    // Los nodos centrales tienen mayor betweenness
    assert(scores[2] > scores[1]);
    assert(scores[3] > scores[4]);
    std::cout << "testBetweenness PASSED\n";
}

void testPageRankSums() {
    auto g = pathGraph();
    PageRank pr;
    auto scores = pr.compute(g);
    double sum = 0.0;
    for (const auto& [id, v] : scores) sum += v;
    assert(std::abs(sum - 1.0) < 1e-4);
    std::cout << "testPageRankSums PASSED (sum=" << sum << ")\n";
}

void testCloseness() {
    auto g = pathGraph();
    ClosenessCentrality cc;
    auto scores = cc.compute(g);
    // Los nodos centrales estan mas cerca de todos que los extremos
    assert(scores[2] > scores[1]);
    assert(scores[3] > scores[4]);
    std::cout << "testCloseness PASSED\n";
}

void testAverageShortestPath() {
    auto g = pathGraph();
    AverageShortestPath asp;
    auto scores = asp.compute(g);
    // El nodo 1 alcanza {2,3,4} a distancias 1,2,3 -> promedio = 2.0
    assert(std::abs(scores[1] - 2.0) < 1e-9);
    // El nodo central 2 tiene un camino promedio menor que el extremo 1
    assert(scores[2] < scores[1]);
    // Promedio global sobre todos los nodos: (2 + 4/3 + 4/3 + 2) / 4 = 1.6667
    assert(std::abs(AverageShortestPath::graphAverage(g) - 1.66667) < 1e-3);
    std::cout << "testAverageShortestPath PASSED\n";
}

void testEigenvector() {
    auto g = pathGraph();
    EigenvectorCentrality ec;
    auto scores = ec.compute(g);
    // Los nodos centrales acumulan mas influencia que los extremos
    assert(scores[2] > scores[1]);
    assert(scores[3] > scores[4]);
    std::cout << "testEigenvector PASSED\n";
}

void testClusteringTriangle() {
    // Grafo completo K3: cada vertice debe tener CC = 1
    Graph g(false);
    g.addEdge(1, 2);
    g.addEdge(2, 3);
    g.addEdge(1, 3);
    ClusteringCoefficient cc;
    auto scores = cc.compute(g);
    for (const auto& [id, v] : scores)
        assert(std::abs(v - 1.0) < 1e-9);
    std::cout << "testClusteringTriangle PASSED\n";
}

int main() {
    testDegree();
    testBetweenness();
    testCloseness();
    testPageRankSums();
    testAverageShortestPath();
    testEigenvector();
    testClusteringTriangle();
    std::cout << "All metric tests passed.\n";
    return 0;
}
