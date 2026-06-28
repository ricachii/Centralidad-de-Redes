#include "ClosenessCentrality.h"
#include "utils/BFS.h"

// Calcula la Centralidad de Cercanía (Closeness Centrality) de cada nodo.
// Responde a la pregunta: "¿Qué tan cerca está este nodo de todos los demás?"
std::unordered_map<int, double> ClosenessCentrality::compute(const Graph& g, ProgressFn progress) const {
    const auto verts = g.vertices();
    const int n = (int)verts.size();
    std::unordered_map<int, double> result;

    int done = 0;
    // Iteramos tomando cada vértice 'v' como el punto de inicio
    for (int v : verts) {
        if (progress) progress(done++, n);
        
        // 1. Ejecutamos un BFS para obtener la distancia más corta desde 'v' a todos los demás
        auto dist = bfsDistances(g, v);
        
        // Descontamos al propio nodo 'v' de la lista de alcanzables
        int reachable = (int)dist.size() - 1;

        long long totalDist = 0;
        // 2. Sumamos todas las distancias para obtener el "costo total" de viaje
        for (const auto& [id, d] : dist)
            if (id != v) totalDist += d;

        // 3. Casos base: Si el nodo está totalmente aislado, su cercanía es 0
        if (reachable == 0 || totalDist == 0) {
            result[v] = 0.0;
        } else {
            // 4. Fórmula de Wasserman y Faust (Súper importante para redes reales)
            // En vez de usar la fórmula clásica (1 / distancia_promedio) que falla en grafos desconectados,
            // multiplicamos por la fracción de la red que realmente pudimos alcanzar.
            
            // Fracción de nodos alcanzados (sobre el total posible N-1)
            double cc = (double)reachable / (n - 1);
            
            // Lo multiplicamos por el inverso de la distancia promedio de esos nodos alcanzados
            cc *= (double)reachable / totalDist;
            
            result[v] = cc;
        }
    }
    if (progress) progress(n, n); // La barra de progreso llega al 100%
    return result;
}