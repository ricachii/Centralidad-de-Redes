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
// ¡Esta función es clave! El enunciado pide explícitamente reportar la varianza
// de los tiempos después de correr las métricas 10 veces.
inline double variance(const std::vector<double>& v) {
    double m = mean(v);
    double sum = 0.0;
    // Fórmula estándar: sumatoria de (valor_actual - promedio)^2
    for (double x : v) sum += (x - m) * (x - m);
    return sum / v.size();
}

// Calcula la desviación estándar (la raíz cuadrada de la varianza).
// Aunque el profe no la pidió explícitamente, siempre es útil tenerla 
// para armar los gráficos de barras con márgenes de error en el informe.
inline double stddev(const std::vector<double>& v) {
    return std::sqrt(variance(v));
}

// Calcula el promedio, pero directamente desde un diccionario (mapa).
// Esta nos salva la vida en los experimentos adicionales: como las métricas devuelven 
// un diccionario con [ID_NODO -> PUNTAJE], esta función suma todos los puntajes y saca 
// el promedio global de la red entera de una sola pasada.
inline double meanScores(const std::unordered_map<int, double>& scores) {
    if (scores.empty()) return 0.0;
    
    double sum = 0.0;
    // Solo nos interesa el valor (val), ignoramos el ID del nodo (id)
    for (const auto& [id, val] : scores) sum += val;
    
    return sum / scores.size();
}