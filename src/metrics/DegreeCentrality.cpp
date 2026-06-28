#include "DegreeCentrality.h"

// Constructor que simplemente guarda la configuración que elegimos.
DegreeCentrality::DegreeCentrality(bool normalized, bool in_degree)
    : normalized_(normalized), in_degree_(in_degree) {}

// Función principal que cuenta las conexiones.
std::unordered_map<int, double> DegreeCentrality::compute(const Graph& g, ProgressFn) const {
    std::unordered_map<int, double> result;
    const auto verts = g.vertices();
    const int n = g.vertexCount();

    // CASO 1: Red Dirigida y queremos saber cuántos apuntan HACIA el nodo (In-Degree).
    if (in_degree_ && g.isDirected()) {
        // Inicializamos todos los nodos en 0.
        for (int id : verts) result[id] = 0.0;
        
        // Truco: Como la Lista de Adyacencia solo sabe de dónde SALEN las flechas, 
        // tenemos que recorrer todos los vértices y sus vecinos, y sumarle +1 
        // al nodo de DESTINO (e.to).
        for (int u : verts)
            for (const auto& e : g.neighbors(u))
                result[e.to]++;
    } 
    // CASO 2: Red No Dirigida o solo queremos saber de dónde SALEN las flechas (Out-Degree).
    else {
        // Esto es rapidísimo (O(1) por nodo). Simplemente le preguntamos al grafo
        // de qué tamaño es la lista de vecinos de este nodo.
        for (int id : verts)
            result[id] = (double)g.neighbors(id).size();
    }

    // NORMALIZACIÓN
    // Para poder comparar el nodo de una red de 100 personas con el de una red de 1000,
    // dividimos sus conexiones reales por las conexiones MÁXIMAS que podría llegar a tener (n - 1).
    // Así, si se conecta con todos, su puntaje es 1.0 (100%). Si no se conecta con nadie, es 0.0.
    if (normalized_ && n > 1) {
        double denom = (double)(n - 1);
        for (auto& [id, val] : result)
            val /= denom;
    }
    
    return result;
}