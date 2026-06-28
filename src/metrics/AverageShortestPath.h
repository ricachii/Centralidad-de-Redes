#pragma once
#include "IMetric.h"

// Clase para calcular el "Average Shortest Path" (Camino Más Corto Promedio).
// Hereda de IMetric para asegurar que nuestro main.cpp pueda ejecutarla 
// de la misma forma que ejecuta el PageRank o el Degree.
class AverageShortestPath : public IMetric {
public:
    // Función principal obligatoria (sobrescrita con 'override').
    // Calcula el promedio LOCAL de cada nodo (cuántos saltos en promedio le toma 
    // a un nodo específico llegar a todos los demás). Devuelve el diccionario de resultados.
    std::unordered_map<int, double> compute(
        const Graph& g, ProgressFn progress = nullptr) const override;
        
    // Nombre oficial de la métrica para que los reportes en consola y 
    // los archivos .csv se generen con el título correcto.
    std::string name() const override { return "Average Shortest Path"; }

    // ¡Ojo con esta función! Es 'static' (se puede llamar sin crear el objeto).
    // La agregamos como un extra a esta clase porque el enunciado del profe pide 
    // que el ASP sea una "medida de toda la red". Esta función envuelve a 'compute' 
    // y saca el gran promedio GLOBAL de todo el grafo.
    static double graphAverage(const Graph& g);
};