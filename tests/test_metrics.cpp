#include <cassert>
#include <iostream>
#include <cmath>
#include "graph/Graph.h"
#include "metrics/DegreeCentrality.h"
#include "metrics/BetweennessCentrality.h"
#include "metrics/ClosenessCentrality.h"
#include "metrics/PageRank.h"
#include "metrics/ClusteringCoefficient.h"

// Simple path: 1 -- 2 -- 3 -- 4
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
    // Middle nodes have higher betweenness
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

void testClusteringTriangle() {
    // Complete graph K3: every vertex should have CC = 1
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
    testPageRankSums();
    testClusteringTriangle();
    std::cout << "All metric tests passed.\n";
    return 0;
}
