#include <iostream>
#include <memory>
#include <vector>
#include <map>

#include "graph/Graph.h"
#include "parsers/EdgeListParser.h"
#include "parsers/PajekParser.h"
#include "metrics/DegreeCentrality.h"
#include "metrics/BetweennessCentrality.h"
#include "metrics/ClosenessCentrality.h"
#include "metrics/PageRank.h"
#include "metrics/AverageShortestPath.h"
#include "metrics/EigenvectorCentrality.h"
#include "metrics/ClusteringCoefficient.h"
#include "experiments/ExperimentRunner.h"
#include "experiments/EdgeExperiment.h"
#include "utils/Timer.h"

static std::vector<std::shared_ptr<IMetric>> allMetrics() {
    return {
        std::make_shared<DegreeCentrality>(),
        std::make_shared<BetweennessCentrality>(),
        std::make_shared<ClosenessCentrality>(),
        std::make_shared<PageRank>(),
        std::make_shared<AverageShortestPath>(),
        std::make_shared<EigenvectorCentrality>(),
        std::make_shared<ClusteringCoefficient>(),
    };
}

static Graph loadTradeNetwork() {
    const std::vector<std::string> files = {
        "data/dataset2/2000.net",
        "data/dataset2/2005.net",
        "data/dataset2/2010.net",
        "data/dataset2/2015.net",
        "data/dataset2/2018.net",
    };

    std::map<std::pair<int,int>, double> edgeMap;

    PajekParser parser(true);
    for (const auto& f : files) {
        Graph g = parser.parse(f);
        for (int u : g.vertices())
            for (const auto& e : g.neighbors(u)) {
                auto key = std::make_pair(u, e.to);
                auto it = edgeMap.find(key);
                if (it == edgeMap.end() || e.weight > it->second)
                    edgeMap[key] = e.weight;
            }
    }

    Graph result(true);
    for (const auto& [pair, w] : edgeMap)
        result.addEdge(pair.first, pair.second, w);

    std::cout << "[Trade combined] " << result.vertexCount()
              << "V, " << result.edgeCount() << "E\n";
    return result;
}

static void printZoneDeltas(const std::string& label,
    const std::vector<EdgeExperiment::Delta>& deltas)
{
    std::cout << "  [" << label << "]\n";
    for (const auto& d : deltas)
        std::cout << "    " << d.metricName << ": "
                  << std::showpos << d.percentChange << "%\n" << std::noshowpos;
}

static void runExperiment(const std::string& name, Graph& g) {
    std::cout << "\n=== " << name << " | V=" << g.vertexCount()
              << " E=" << g.edgeCount() << " ===\n";

    auto metrics = allMetrics();
    auto result  = ExperimentRunner::run(name, g, metrics);
    ExperimentRunner::saveCSV(result, "results/csv/" + name + ".csv");

    using Z = EdgeExperiment::Zone;
    const std::vector<std::pair<Z, std::string>> zones = {
        {Z::HUB,        "hub (top 10% grado)"},
        {Z::PERIPHERAL, "periferico (bottom 10% grado)"},
        {Z::RANDOM,     "aleatorio"},
    };

    std::cout << "\n-- Impacto de agregar 5 aristas --\n";
    for (const auto& [z, label] : zones)
        printZoneDeltas(label, EdgeExperiment::addEdges(g, metrics, 5, z));

    std::cout << "\n-- Impacto de quitar 5 aristas --\n";
    for (const auto& [z, label] : zones)
        printZoneDeltas(label, EdgeExperiment::removeEdges(g, metrics, 5, z));
}

int main() {
    {
        EdgeListParser parser(false, '\t');
        Timer t; t.start();
        Graph g = parser.parse("data/dataset1/yeast.edgelist");
        double ms = t.elapsedMs();
        std::cout << "Build time: " << ms << " ms | Memory: "
                  << g.memoryBytes() / 1024 << " KB\n";
        runExperiment("yeast", g);
    }

    {
        Timer t; t.start();
        Graph g = loadTradeNetwork();
        double ms = t.elapsedMs();
        std::cout << "Build time: " << ms << " ms | Memory: "
                  << g.memoryBytes() / 1024 << " KB\n";
        runExperiment("trade", g);
    }

    return 0;
}
