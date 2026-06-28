#pragma once
#include "IMetric.h"

// Clase para calcular la Centralidad de Cercanía (Closeness).
// Hereda de la interfaz base IMetric para integrarse automáticamente 
// con el sistema de experimentos (ExperimentRunner).
class ClosenessCentrality : public IMetric {
public:
    // Función principal obligatoria (sobrescrita con 'override').
    // Recibe el grafo y la función de progreso, y devuelve el diccionario 
    // con los puntajes de cercanía de cada nodo.
    std::unordered_map<int, double> compute(
        const Graph& g, ProgressFn progress = nullptr) const override;
        
    // Etiqueta de texto para identificar la métrica en la consola 
    // y en los encabezados de los archivos .csv generados.
    std::string name() const override { return "Closeness Centrality"; }
    
    // Nota: A diferencia de Degree o Betweenness, esta clase no necesita 
    // variables privadas (flags de normalización), ya que la fórmula de 
    // Wasserman-Faust que usamos en el .cpp ya incluye su propia normalización.
};