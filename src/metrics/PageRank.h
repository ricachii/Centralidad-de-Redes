#pragma once
#include "IMetric.h"

// Clase para calcular la métrica de PageRank.
// Hereda de IMetric para encajar perfectamente en el sistema de experimentos.
class PageRank : public IMetric {
public:
    // Constructor con parámetros por defecto (súper importante):
    // - damping (0.85): Es el estándar matemático propuesto por los creadores de Google. 
    //   Asume que el 85% de las veces seguimos un link, y el 15% saltamos a otro lado.
    // - maxIter (100): Pone un "techo" de seguridad para que el ciclo no sea infinito.
    // - tol (1e-6): Tolerancia de 0.000001. Exige una precisión altísima para considerar 
    //   que el algoritmo ya "convergió" (se estabilizó).
    explicit PageRank(double damping = 0.85, int maxIter = 100, double tol = 1e-6);

    // Función obligatoria que recibe el Grafo y ejecuta el cálculo iterativo.
    std::unordered_map<int, double> compute(
        const Graph& g, ProgressFn progress = nullptr) const override;
        
    // Nombre oficial para los .csv y los prints de la consola.
    std::string name() const override { return "PageRank"; }

private:
    // Variables privadas para guardar la configuración elegida al crear el objeto.
    // Al hacerlas privadas, garantizamos el Encapsulamiento (nadie puede alterarlas 
    // a mitad del cálculo y arruinar la matemática).
    double damping_;
    int    maxIter_;
    double tol_;
};