#include "BetweennessCentrality.h"
#include <queue>
#include <stack>
#include <vector>
#include <unordered_map>

BetweennessCentrality::BetweennessCentrality(bool normalized)
    : normalized_(normalized) {}

std::unordered_map<int, double> BetweennessCentrality::compute(
    const Graph& g, ProgressFn progress) const
{
    const auto verts = g.vertices();
    const int n = (int)verts.size();

    std::unordered_map<int, int> idx;
    for (int i = 0; i < n; i++) idx[verts[i]] = i;

    std::vector<double> betweenness(n, 0.0);

    for (int s = 0; s < n; s++) {
        if (progress) progress(s, n);

        std::vector<std::vector<int>> pred(n);
        std::vector<double> sigma(n, 0.0);
        std::vector<int>    dist(n, -1);
        sigma[s] = 1.0;
        dist[s]  = 0;

        std::queue<int> Q;
        std::stack<int> S;
        Q.push(s);

        while (!Q.empty()) {
            int v = Q.front(); Q.pop();
            S.push(v);
            for (const auto& e : g.neighbors(verts[v])) {
                int w = idx[e.to];
                if (dist[w] < 0) {
                    Q.push(w);
                    dist[w] = dist[v] + 1;
                }
                if (dist[w] == dist[v] + 1) {
                    sigma[w] += sigma[v];
                    pred[w].push_back(v);
                }
            }
        }

        std::vector<double> delta(n, 0.0);
        while (!S.empty()) {
            int w = S.top(); S.pop();
            for (int v : pred[w])
                delta[v] += (sigma[v] / sigma[w]) * (1.0 + delta[w]);
            if (w != s)
                betweenness[w] += delta[w];
        }
    }
    if (progress) progress(n, n);

    if (!g.isDirected())
        for (auto& b : betweenness) b /= 2.0;

    std::unordered_map<int, double> result;
    for (int i = 0; i < n; i++) {
        double val = betweenness[i];
        if (normalized_ && n > 2) {
            double denom = g.isDirected()
                ? (double)(n - 1) * (n - 2)
                : (double)(n - 1) * (n - 2) / 2.0;
            val /= denom;
        }
        result[verts[i]] = val;
    }
    return result;
}
