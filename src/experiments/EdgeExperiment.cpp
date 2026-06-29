#include "EdgeExperiment.h"
#include <algorithm>
#include <random>
#include <cmath>

static double meanScore(const std::unordered_map<int, double>& scores) {
    double sum = 0.0;
    for (const auto& [id, v] : scores) sum += v;
    return scores.empty() ? 0.0 : sum / scores.size();
}

static std::vector<EdgeExperiment::Delta> computeDeltas(
    const Graph& before,
    const Graph& after,
    const std::vector<std::shared_ptr<IMetric>>& metrics)
{
    std::vector<EdgeExperiment::Delta> deltas;
    for (const auto& m : metrics) {
        double b = meanScore(m->compute(before));
        double a = meanScore(m->compute(after));
        double pct = (b != 0.0) ? (a - b) / std::abs(b) * 100.0 : 0.0;
        deltas.push_back({m->name(), b, a, pct});
    }
    return deltas;
}

// Devuelve los vertices ordenados por grado; toma el 10% superior o inferior segun la zona.
static std::vector<int> zoneVertices(const Graph& g, EdgeExperiment::Zone zone) {
    auto verts = g.vertices();
    std::sort(verts.begin(), verts.end(), [&g](int a, int b) {
        return g.neighbors(a).size() > g.neighbors(b).size(); // grado descendente
    });

    size_t cut = std::max<size_t>(1, verts.size() / 10);
    if (zone == EdgeExperiment::Zone::HUB)
        return {verts.begin(), verts.begin() + cut};
    if (zone == EdgeExperiment::Zone::PERIPHERAL)
        return {verts.end() - cut, verts.end()};
    return verts; // RANDOM: todo el grafo
}

std::vector<EdgeExperiment::Delta> EdgeExperiment::addEdges(
    Graph g,
    const std::vector<std::shared_ptr<IMetric>>& metrics,
    int  k,
    Zone zone)
{
    Graph original = g;
    const auto allVerts = g.vertices();
    const auto srcVerts = zoneVertices(g, zone);

    std::mt19937 rng(42);
    std::uniform_int_distribution<int> srcDist(0, (int)srcVerts.size() - 1);
    std::uniform_int_distribution<int> dstDist(0, (int)allVerts.size() - 1);

    int added = 0, attempts = 0;
    while (added < k && attempts < k * 1000) {
        ++attempts;
        int u = srcVerts[srcDist(rng)];
        int v = allVerts[dstDist(rng)];
        if (u != v && !g.hasEdge(u, v)) {
            g.addEdge(u, v);
            ++added;
        }
    }
    return computeDeltas(original, g, metrics);
}

std::vector<EdgeExperiment::Delta> EdgeExperiment::removeEdges(
    Graph g,
    const std::vector<std::shared_ptr<IMetric>>& metrics,
    int  k,
    Zone zone)
{
    Graph original = g;
    const auto srcVerts = zoneVertices(g, zone);

    std::mt19937 rng(42);
    std::uniform_int_distribution<int> srcDist(0, (int)srcVerts.size() - 1);

    int removed = 0, attempts = 0;
    while (removed < k && attempts < k * 100) {
        ++attempts;
        int u = srcVerts[srcDist(rng)];
        const auto& nb = g.neighbors(u);
        if (nb.empty()) continue;
        std::uniform_int_distribution<int> nd(0, (int)nb.size() - 1);
        g.removeEdge(u, nb[nd(rng)].to);
        ++removed;
    }
    return computeDeltas(original, g, metrics);
}
