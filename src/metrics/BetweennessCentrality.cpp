#include "BetweennessCentrality.h"
#include <queue>
#include <stack>
#include <vector>
#include <unordered_map>

BetweennessCentrality::BetweennessCentrality(bool normalized)
    : normalized_(normalized) {}

std::unordered_map<int, double> BetweennessCentrality::compute(
    const Graph& g, ProgressFn progress) const
{
    const auto verts = g.vertices();
    const int n = (int)verts.size();

    // 1. Truco de optimización (Mapeo): 
    // Los IDs de los nodos pueden ser números como 10, 55, 900.
    // Creamos un diccionario para mapearlos a índices ordenados (0, 1, 2...) 
    // para poder usar std::vector en lugar de mapas más adelante. ¡Esto ahorra mucha RAM y tiempo!
    std::unordered_map<int, int> idx;
    for (int i = 0; i < n; i++) idx[verts[i]] = i;

    // Aquí guardaremos el puntaje final de Betweenness de cada nodo
    std::vector<double> betweenness(n, 0.0);

    // Iteramos tomando cada nodo 's' como el punto de inicio
    for (int s = 0; s < n; s++) {
        if (progress) progress(s, n);

        // Estructuras de datos clave para el Algoritmo de Brandes:
        std::vector<std::vector<int>> pred(n); // pred[w] = lista de nodos antes de 'w' en el camino más corto
        std::vector<double> sigma(n, 0.0);     // sigma[w] = cantidad de caminos más cortos desde 's' hasta 'w'
        std::vector<int>    dist(n, -1);       // dist[w] = distancia desde 's' hasta 'w'
        
        sigma[s] = 1.0;
        dist[s]  = 0;

        std::queue<int> Q; // Cola para el BFS (Búsqueda en Anchura)
        std::stack<int> S; // Pila para guardar el orden y luego ir "hacia atrás"
        Q.push(s);

        // --- FASE 1: Búsqueda hacia adelante (BFS Modificado) ---
        // Descubrimos las distancias y contamos cuántos caminos más cortos hay
        while (!Q.empty()) {
            int v = Q.front(); Q.pop();
            S.push(v); // Guardamos el nodo en la pila para la fase 2
            
            for (const auto& e : g.neighbors(verts[v])) {
                int w = idx[e.to];
                
                // Si es la primera vez que vemos el nodo 'w'
                if (dist[w] < 0) {
                    Q.push(w);
                    dist[w] = dist[v] + 1;
                }
                // Si encontramos otro camino igual de corto para llegar a 'w'
                if (dist[w] == dist[v] + 1) {
                    sigma[w] += sigma[v]; // Sumamos las formas de llegar
                    pred[w].push_back(v); // Anotamos a 'v' como predecesor de 'w'
                }
            }
        }

        // --- FASE 2: Propagación hacia atrás (Back-propagation) ---
        // Sacamos los nodos de la pila (desde el más lejano al más cercano)
        // y calculamos cuánta "dependencia" tienen de los nodos puente.
        std::vector<double> delta(n, 0.0);
        while (!S.empty()) {
            int w = S.top(); S.pop();
            for (int v : pred[w]) {
                // Fórmula de Brandes: sumamos la fracción de caminos que pasan por 'v'
                delta[v] += (sigma[v] / sigma[w]) * (1.0 + delta[w]);
            }
            // Si 'w' no es el nodo de origen, le sumamos la dependencia acumulada a su puntaje final
            if (w != s) {
                betweenness[w] += delta[w];
            }
        }
    }
    if (progress) progress(n, n);

    // 3. Ajustes finales
    // En grafos no dirigidos, cada camino se contó dos veces (A->B y B->A), así que dividimos por 2.
    if (!g.isDirected())
        for (auto& b : betweenness) b /= 2.0;

    std::unordered_map<int, double> result;
    for (int i = 0; i < n; i++) {
        double val = betweenness[i];
        
        // Normalización matemática para que el puntaje quede entre 0 y 1.
        // Se divide por la cantidad máxima de pares de nodos posibles.
        if (normalized_ && n > 2) {
            double denom = g.isDirected()
                ? (double)(n - 1) * (n - 2)
                : (double)(n - 1) * (n - 2) / 2.0;
            val /= denom;
        }
        result[verts[i]] = val; // Restauramos los IDs originales y guardamos
    }
    return result;
}