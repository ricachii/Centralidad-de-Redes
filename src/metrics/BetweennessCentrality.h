#pragma once
#include "IMetric.h"

// Clase para calcular la Centralidad de Intermediación (Betweenness).
// El comentario original dice O(V * E), que es la forma resumida de escribir 
// la complejidad del Algoritmo de Brandes para grafos esparcidos (redes reales).
class BetweennessCentrality : public IMetric {
public:
    // Constructor de la métrica. La bandera 'normalized' viene en 'true' por defecto.
    // Esto es importantísimo porque los puntajes crudos de Betweenness cuentan la cantidad 
    // total de caminos, lo que da números gigantes (cientos de miles). Normalizarlo 
    // divide todo por el máximo posible, dejándolo entre 0.0 y 1.0 para que sea legible.
    explicit BetweennessCentrality(bool normalized = true);

    // La función principal que sobrescribe el molde de IMetric. 
    // Aquí es donde el main.cpp entra a ejecutar el Algoritmo de Brandes.
    std::unordered_map<int, double> compute(
        const Graph& g, ProgressFn progress = nullptr) const override;
        
    // Etiqueta de texto para que los reportes se generen de forma automática y ordenada.
    std::string name() const override { return "Betweenness Centrality"; }

private:
    // Variable privada que recuerda si le pedimos al programa normalizar los datos o no.
    bool normalized_;
};