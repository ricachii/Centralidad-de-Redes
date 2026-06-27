#pragma once
#include "IMetric.h"

// Power iteration. Converges to the principal eigenvector of the adjacency matrix.
class EigenvectorCentrality : public IMetric {
public:
    explicit EigenvectorCentrality(int maxIter = 100, double tol = 1e-6);

    std::unordered_map<int, double> compute(
        const Graph& g, ProgressFn progress = nullptr) const override;
    std::string name() const override { return "Eigenvector Centrality"; }

private:
    int    maxIter_;
    double tol_;
};
