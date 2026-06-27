#include "AverageShortestPath.h"
#include "utils/BFS.h"
#include "utils/Stats.h"

std::unordered_map<int, double> AverageShortestPath::compute(const Graph& g, ProgressFn progress) const {
    const auto verts = g.vertices();
    const int n = (int)verts.size();
    std::unordered_map<int, double> result;

    int done = 0;
    for (int src : verts) {
        if (progress) progress(done++, n);
        auto dist = bfsDistances(g, src);
        int reachable = (int)dist.size() - 1;

        long long total = 0;
        for (const auto& [id, d] : dist)
            if (id != src) total += d;

        result[src] = reachable > 0 ? (double)total / reachable : 0.0;
    }
    if (progress) progress(n, n);
    return result;
}

double AverageShortestPath::graphAverage(const Graph& g) {
    AverageShortestPath m;
    return meanScores(m.compute(g));
}
