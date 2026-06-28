#pragma once
#include <vector>
#include <unordered_map>
#include <cmath>
#include <numeric>
#include <algorithm>

// Calcula el promedio (media) de una lista de números.
// Usamos std::accumulate porque es la forma más rápida y limpia en C++
// de sumar todos los elementos de un vector sin hacer un 'for' manual.
inline double mean(const std::vector<double>& v) {
    return std::accumulate(v.begin(), v.end(), 0.0) / v.size();
}

// Calcula la varianza poblacional.
// El enunciado pide explícitamente reportar la varianza de los tiempos
// después de correr las métricas 10 veces.
inline double variance(const std::vector<double>& v) {
    double m = mean(v);
    double sum = 0.0;
    // Fórmula estándar: sumatoria de (valor_actual - promedio)^2
    for (double x : v) sum += (x - m) * (x - m);
    return sum / v.size();
}

// Calcula la desviación estándar (raíz cuadrada de la varianza).
inline double stddev(const std::vector<double>& v) {
    return std::sqrt(variance(v));
}

// Calcula el promedio directamente desde un mapa [ID_NODO -> PUNTAJE].
// Las métricas devuelven este formato, y esta función saca el promedio
// global de la red entera en una sola pasada.
inline double meanScores(const std::unordered_map<int, double>& scores) {
    if (scores.empty()) return 0.0;
    double sum = 0.0;
    for (const auto& [id, val] : scores) sum += val;
    return sum / scores.size();
}
