#include "AverageShortestPath.h"
#include "utils/BFS.h"
#include "utils/Stats.h"

// Calcula el "Average Shortest Path" (Camino Más Corto Promedio) LOCAL para cada nodo.
// Es decir, para el nodo A, ¿cuántos saltos en promedio tiene que dar para llegar al resto?
std::unordered_map<int, double> AverageShortestPath::compute(const Graph& g, ProgressFn progress) const {
    const auto verts = g.vertices();
    const int n = (int)verts.size();
    std::unordered_map<int, double> result;

    int done = 0;
    // Recorremos todos los nodos del grafo, usándolos de origen (src) uno por uno
    for (int src : verts) {
        if (progress) progress(done++, n);
        
        // Usamos la utilidad BFS que armamos para obtener las distancias desde 'src' 
        // hacia todos los nodos que puede alcanzar.
        auto dist = bfsDistances(g, src);
        
        // Descontamos 1 porque no nos interesa la distancia del nodo hacia sí mismo (que es 0)
        int reachable = (int)dist.size() - 1;

        long long total = 0;
        // Sumamos todas las distancias obtenidas por el BFS
        for (const auto& [id, d] : dist)
            if (id != src) total += d;

        // Si el nodo puede alcanzar a otros (reachable > 0), sacamos su promedio.
        // Si es un nodo aislado (reachable == 0), su promedio es 0.0 para evitar 
        // un error matemático de división por cero.
        result[src] = reachable > 0 ? (double)total / reachable : 0.0;
    }
    if (progress) progress(n, n);
    return result;
}

// Esta es la función que realmente cumple con la definición global pedida por el profesor.
// Calcula el promedio de toda la red sumando los promedios locales de cada nodo.
double AverageShortestPath::graphAverage(const Graph& g) {
    AverageShortestPath m;
    // Llama a la función 'compute' de arriba y luego usa 'meanScores' (de Stats.h) 
    // para promediar todos los resultados en un solo gran número global.
    return meanScores(m.compute(g));
}