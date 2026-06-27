#pragma once
#include "IMetric.h"

class PageRank : public IMetric {
public:
    explicit PageRank(double damping = 0.85, int maxIter = 100, double tol = 1e-6);

    std::unordered_map<int, double> compute(
        const Graph& g, ProgressFn progress = nullptr) const override;
    std::string name() const override { return "PageRank"; }

private:
    double damping_;
    int    maxIter_;
    double tol_;
};
