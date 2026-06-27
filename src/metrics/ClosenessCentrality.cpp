#include "ClosenessCentrality.h"
#include "utils/BFS.h"

std::unordered_map<int, double> ClosenessCentrality::compute(const Graph& g, ProgressFn progress) const {
    const auto verts = g.vertices();
    const int n = (int)verts.size();
    std::unordered_map<int, double> result;

    int done = 0;
    for (int v : verts) {
        if (progress) progress(done++, n);
        auto dist = bfsDistances(g, v);
        int reachable = (int)dist.size() - 1;

        long long totalDist = 0;
        for (const auto& [id, d] : dist)
            if (id != v) totalDist += d;

        if (reachable == 0 || totalDist == 0) {
            result[v] = 0.0;
        } else {
            // Wasserman-Faust: normalizes for disconnected graphs
            double cc = (double)reachable / (n - 1);
            cc *= (double)reachable / totalDist;
            result[v] = cc;
        }
    }
    if (progress) progress(n, n);
    return result;
}
