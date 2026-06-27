#include "PageRank.h"
#include <cmath>

PageRank::PageRank(double damping, int maxIter, double tol)
    : damping_(damping), maxIter_(maxIter), tol_(tol) {}

std::unordered_map<int, double> PageRank::compute(const Graph& g, ProgressFn progress) const {
    const auto verts = g.vertices();
    const int  n     = (int)verts.size();

    std::unordered_map<int, double> rank, next;
    double init = 1.0 / n;
    for (int v : verts) rank[v] = init;

    for (int iter = 0; iter < maxIter_; iter++) {
        if (progress) progress(iter, maxIter_);
        double dangling = 0.0;
        for (int v : verts)
            if (g.neighbors(v).empty())
                dangling += rank[v];

        double base = (1.0 - damping_) / n + damping_ * dangling / n;
        for (int v : verts) next[v] = base;

        for (int u : verts) {
            const auto& nb = g.neighbors(u);
            if (nb.empty()) continue;
            double share = damping_ * rank[u] / nb.size();
            for (const auto& e : nb)
                next[e.to] += share;
        }

        double diff = 0.0;
        for (int v : verts)
            diff += std::abs(next[v] - rank[v]);

        rank.swap(next);
        if (diff < tol_) break;
    }
    return rank;
}
