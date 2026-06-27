#pragma once
#include "IMetric.h"

class AverageShortestPath : public IMetric {
public:
    std::unordered_map<int, double> compute(
        const Graph& g, ProgressFn progress = nullptr) const override;
    std::string name() const override { return "Average Shortest Path"; }

    static double graphAverage(const Graph& g);
};
