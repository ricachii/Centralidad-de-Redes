#pragma once
#include "graph/Graph.h"
#include "metrics/IMetric.h"
#include <memory>
#include <vector>
#include <string>

// Measures impact on metrics when adding or removing edges
class EdgeExperiment {
public:
    // Where in the network to place/remove the edge
    enum class Zone {
        RANDOM,      // uniformly random vertices
        HUB,         // top 10% by degree (highly connected)
        PERIPHERAL   // bottom 10% by degree (poorly connected)
    };

    struct Delta {
        std::string metricName;
        double      beforeMean;
        double      afterMean;
        double      percentChange;
    };

    // Add k edges sourced from the given zone and report metric deltas
    static std::vector<Delta> addEdges(
        Graph g,
        const std::vector<std::shared_ptr<IMetric>>& metrics,
        int  k    = 5,
        Zone zone = Zone::RANDOM
    );

    // Remove k edges sourced from the given zone and report metric deltas
    static std::vector<Delta> removeEdges(
        Graph g,
        const std::vector<std::shared_ptr<IMetric>>& metrics,
        int  k    = 5,
        Zone zone = Zone::RANDOM
    );
};
