#pragma once
#include "IMetric.h"

//Esta metrica mide que tanto se "condensan" los nodos alrededor de otros, esta es la version local del algoritmo, que mide la cantidad de triangulos que pasan por el nodo.
class ClusteringCoefficient : public IMetric {
public:
    std::unordered_map<int, double> compute(
        const Graph& g, ProgressFn progress = nullptr) const override;
    std::string name() const override { return "Clustering Coefficient"; }

    static double globalCoefficient(const Graph& g);
};
