#pragma once
#include "IMetric.h"

// Brandes algorithm: O(V * E)
class BetweennessCentrality : public IMetric {
public:
    explicit BetweennessCentrality(bool normalized = true);

    std::unordered_map<int, double> compute(
        const Graph& g, ProgressFn progress = nullptr) const override;
    std::string name() const override { return "Betweenness Centrality"; }

private:
    bool normalized_;
};
