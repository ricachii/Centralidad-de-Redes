#include "ClusteringCoefficient.h"
#include "utils/Stats.h"
#include <unordered_set>

std::unordered_map<int, double> ClusteringCoefficient::compute(const Graph& g, ProgressFn progress) const {
    const auto verts = g.vertices();
    const int n = (int)verts.size();
    std::unordered_map<int, double> result;

    int done = 0;
    for (int v : verts) {
        if (progress) progress(done++, n);
        const auto& nb = g.neighbors(v);
        int deg = (int)nb.size();

        if (deg < 2) {
            result[v] = 0.0;
            continue;
        }

        std::unordered_set<int> nbSet;
        nbSet.reserve(deg);
        for (const auto& e : nb) nbSet.insert(e.to);

        int triangles = 0;
        for (const auto& e : nb)
            for (const auto& f : g.neighbors(e.to))
                if (nbSet.count(f.to)) triangles++;

        // undirected: each triangle appears twice in the count
        result[v] = (double)triangles / ((double)deg * (deg - 1));
    }
    if (progress) progress(n, n);
    return result;
}

double ClusteringCoefficient::globalCoefficient(const Graph& g) {
    ClusteringCoefficient cc;
    return meanScores(cc.compute(g));
}
