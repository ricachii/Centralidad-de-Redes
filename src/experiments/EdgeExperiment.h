#pragma once
#include "graph/Graph.h"
#include "metrics/IMetric.h"
#include <memory>
#include <vector>
#include <string>

// Mide el impacto en las metricas al agregar o quitar aristas
class EdgeExperiment {
public:
    // Donde en la red se coloca/quita la arista
    enum class Zone {
        RANDOM,      // vertices uniformemente aleatorios
        HUB,         // 10% superior por grado (muy conectados)
        PERIPHERAL   // 10% inferior por grado (poco conectados)
    };

    struct Delta {
        std::string metricName;
        double      beforeMean;
        double      afterMean;
        double      percentChange;
    };

    // Agrega k aristas desde la zona dada y reporta los deltas de cada metrica
    static std::vector<Delta> addEdges(
        Graph g,
        const std::vector<std::shared_ptr<IMetric>>& metrics,
        int  k    = 5,
        Zone zone = Zone::RANDOM
    );

    // Quita k aristas desde la zona dada y reporta los deltas de cada metrica
    static std::vector<Delta> removeEdges(
        Graph g,
        const std::vector<std::shared_ptr<IMetric>>& metrics,
        int  k    = 5,
        Zone zone = Zone::RANDOM
    );
};
