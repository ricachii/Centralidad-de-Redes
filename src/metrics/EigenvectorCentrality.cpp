#include "EigenvectorCentrality.h"
#include <cmath>
#include <algorithm>

EigenvectorCentrality::EigenvectorCentrality(int maxIter, double tol)
    : maxIter_(maxIter), tol_(tol) {}

std::unordered_map<int, double> EigenvectorCentrality::compute(const Graph& g, ProgressFn progress) const {
    const auto verts = g.vertices();
    std::unordered_map<int, double> x, next;

    double init = 1.0 / verts.size();
    for (int v : verts) x[v] = init;

    for (int iter = 0; iter < maxIter_; iter++) {
        if (progress) progress(iter, maxIter_);
        for (int v : verts) next[v] = 0.0;

        for (int v : verts)
            for (const auto& e : g.neighbors(v))
                next[e.to] += x[v];

        double maxVal = 0.0;
        for (const auto& [id, val] : next)
            maxVal = std::max(maxVal, val);

        if (maxVal < 1e-12) break;
        for (auto& [id, val] : next) val /= maxVal;

        double diff = 0.0;
        for (int v : verts)
            diff += std::abs(next[v] - x[v]);

        x.swap(next);
        if (diff < tol_) break;
    }
    return x;
}
