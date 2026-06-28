#include "PageRank.h"
#include <cmath>

// Constructor:
// - damping (factor de amortiguación): Generalmente 0.85. Es la probabilidad de que 
//   un "surfista web" siga haciendo clic en los links en lugar de saltar a una página al azar.
// - maxIter: Límite de seguridad para que el ciclo no corra infinitamente.
// - tol: Tolerancia. Si los puntajes cambian menos que esto, asumimos que ya terminamos.
PageRank::PageRank(double damping, int maxIter, double tol)
    : damping_(damping), maxIter_(maxIter), tol_(tol) {}

std::unordered_map<int, double> PageRank::compute(const Graph& g, ProgressFn progress) const {
    const auto verts = g.vertices();
    const int  n     = (int)verts.size();

    std::unordered_map<int, double> rank, next;
    
    // 1. Inicialización: Le damos a cada nodo exactamente la misma probabilidad inicial (1 / N).
    double init = 1.0 / n;
    for (int v : verts) rank[v] = init;

    // 2. Iteraciones de propagación de influencia
    for (int iter = 0; iter < maxIter_; iter++) {
        if (progress) progress(iter, maxIter_);
        
        // --- EL TRUCO DE LOS DANGLING NODES (Nodos sin salida) ---
        // Si un nodo no tiene a quién apuntar, su influencia se pierde y la suma total 
        // del sistema deja de ser 1.0. Para arreglarlo, sumamos el puntaje de todos estos 
        // nodos aislados para redistribuirlo mágicamente entre todos más adelante.
        double dangling = 0.0;
        for (int v : verts)
            if (g.neighbors(v).empty())
                dangling += rank[v];

        // --- CÁLCULO BASE (Teletransportación) ---
        // Todos los nodos reciben un puntaje mínimo garantizado por si alguien "salta" 
        // hacia ellos al azar. Aquí además le sumamos la parte de los dangling nodes.
        double base = (1.0 - damping_) / n + damping_ * dangling / n;
        for (int v : verts) next[v] = base;

        // --- PROPAGACIÓN DE INFLUENCIA ---
        // Cada nodo agarra su puntaje actual, le aplica el factor de amortiguación (damping)
        // y lo reparte en partes iguales entre todos los vecinos a los que apunta.
        for (int u : verts) {
            const auto& nb = g.neighbors(u);
            if (nb.empty()) continue; // Si no apunta a nadie, lo ignoramos (ya lo sumamos en dangling)
            
            // Calculamos cuánto le toca a cada vecino
            double share = damping_ * rank[u] / nb.size();
            for (const auto& e : nb)
                next[e.to] += share;
        }

        // --- VERIFICACIÓN DE CONVERGENCIA ---
        // Comparamos el puntaje nuevo ('next') con el anterior ('rank').
        double diff = 0.0;
        for (int v : verts)
            diff += std::abs(next[v] - rank[v]);

        // Actualizamos los puntajes para la siguiente vuelta
        rank.swap(next);
        
        // Si la diferencia total es menor a la tolerancia, el sistema se estabilizó.
        // Rompemos el ciclo antes de llegar al maxIter para ahorrar mucho tiempo.
        if (diff < tol_) break;
    }
    return rank;
}