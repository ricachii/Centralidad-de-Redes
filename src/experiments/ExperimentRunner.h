#pragma once
#include <vector>
#include <string>
#include <memory>
#include "graph/Graph.h"
#include "metrics/IMetric.h"

struct MetricResult {
    std::string metricName;
    double      meanMs;
    double      varianceMs;
    double      minMs;
    double      maxMs;
};

struct GraphResult {
    std::string datasetName;
    int         vertices;
    long long   edges;
    double      buildTimeMs;
    size_t      memoryBytes;
    std::vector<MetricResult> metrics;
};

class ExperimentRunner {
public:
    static const int DEFAULT_RUNS = 10;

    // Runs all metrics N times on g, returns timing stats
    static GraphResult run(
        const std::string& datasetName,
        const Graph& g,
        const std::vector<std::shared_ptr<IMetric>>& metrics,
        int runs = DEFAULT_RUNS
    );

    // Saves results to CSV
    static void saveCSV(const GraphResult& r, const std::string& filepath);
};
