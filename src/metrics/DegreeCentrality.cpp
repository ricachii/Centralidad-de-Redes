#include "DegreeCentrality.h"

DegreeCentrality::DegreeCentrality(bool normalized, bool in_degree)
    : normalized_(normalized), in_degree_(in_degree) {}

std::unordered_map<int, double> DegreeCentrality::compute(const Graph& g, ProgressFn) const {
    std::unordered_map<int, double> result;
    const auto verts = g.vertices();
    const int n = g.vertexCount();

    if (in_degree_ && g.isDirected()) {
        // Count how many times each vertex appears as destination
        for (int id : verts) result[id] = 0.0;
        for (int u : verts)
            for (const auto& e : g.neighbors(u))
                result[e.to]++;
    } else {
        for (int id : verts)
            result[id] = (double)g.neighbors(id).size();
    }

    if (normalized_ && n > 1) {
        double denom = (double)(n - 1);
        for (auto& [id, val] : result)
            val /= denom;
    }
    return result;
}
