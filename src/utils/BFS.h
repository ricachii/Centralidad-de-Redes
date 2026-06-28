#pragma once
#include <queue>
#include <unordered_map>
#include "graph/Graph.h"

// Calcula la distancia más corta (en cantidad de saltos/aristas) desde un nodo de origen (src)
// hacia todos los demás nodos que estén conectados en el grafo.
// Devuelve un diccionario donde la clave es el ID del nodo y el valor es la distancia.
inline std::unordered_map<int, int> bfsDistances(const Graph& g, int src) {
    std::unordered_map<int, int> dist;
    
    // Optimización: reservamos espacio inicial según la cantidad de vértices. 
    // Así evitamos que el mapa tenga que reasignar memoria a cada rato mientras crece.
    dist.reserve(g.vertexCount());
    
    // La distancia del nodo origen a sí mismo siempre es 0
    dist[src] = 0;
    
    // Cola clásica para ir procesando los nodos nivel por nivel (BFS)
    std::queue<int> Q;
    Q.push(src);
    
    while (!Q.empty()) {
        // Sacamos el primer nodo de la cola para procesarlo
        int u = Q.front(); 
        Q.pop();
        
        // Recorremos todos los vecinos directos del nodo actual 'u'
        for (const auto& e : g.neighbors(u)) {
            // count() busca si el vecino ya está en el mapa.
            // Si da 0 (false), significa que es la primera vez que lo vemos.
            if (!dist.count(e.to)) {
                // Su distancia será la distancia del nodo actual + 1 salto
                dist[e.to] = dist[u] + 1;
                
                // Lo metemos al final de la cola para revisar sus vecinos más adelante
                Q.push(e.to);
            }
        }
    }
    
    return dist;
}
