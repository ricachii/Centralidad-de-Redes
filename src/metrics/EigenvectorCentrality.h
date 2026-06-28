#pragma once
#include "IMetric.h"

//Obtiene el vector propio dominante de la matriz de adjacencia del grafo con un metodo iterativo
//Este vector contiene la centralidad de todos los nodos del grafo, esta metrica representa la "influencia" de un nodo en la red y es la base para el PageRank
class EigenvectorCentrality : public IMetric {
public:
    
    // Definimos el numero maximo de iteraciones del algoritmo y el error maximo antes de arrojar un valor
    explicit EigenvectorCentrality(int maxIter = 100, double tol = 1e-6);

    std::unordered_map<int, double> compute(
        const Graph& g, ProgressFn progress = nullptr) const override;
    std::string name() const override { return "Eigenvector Centrality"; }

private:
    int    maxIter_;
    double tol_;
};
