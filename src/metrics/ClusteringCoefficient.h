#pragma once
#include "IMetric.h"

// Local clustering coefficient: C(v) = triangles(v) / (deg(v) * (deg(v)-1))
class ClusteringCoefficient : public IMetric {
public:
    std::unordered_map<int, double> compute(
        const Graph& g, ProgressFn progress = nullptr) const override;
    std::string name() const override { return "Clustering Coefficient"; }

    static double globalCoefficient(const Graph& g);
};
