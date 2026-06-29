#pragma once
#include <iostream>
#include <string>
#include <algorithm>

class ProgressBar {
public:
    explicit ProgressBar(int width = 38) : width_(width) {}

    // Se llama dentro del bucle de la metrica. Solo redibuja cada ~1% para evitar parpadeo.
    void update(int done, int total, int run, int runs) {
        if (total <= 0) return;
        int step = std::max(1, total / 100);
        if (done % step != 0 && done != total) return;

        double pct = (double)done / total;
        int filled = (int)(pct * width_);

        std::cerr << "\r    run " << run << "/" << runs << "  [";
        for (int i = 0; i < width_; i++) {
            if (i < filled)             std::cerr << '=';
            else if (i == filled)       std::cerr << '>';
            else                        std::cerr << ' ';
        }
        std::cerr << "] " << (int)(pct * 100) << "%  " << std::flush;
    }

    // Imprime la linea final de una corrida completada.
    void finishRun(int run, int runs, double ms) {
        std::cerr << "\r    run " << run << "/" << runs
                  << "  done - " << ms << " ms"
                  << "                              \n" << std::flush;
    }

    // Para metricas rapidas (sin progreso interno): solo muestra el numero de corrida.
    void tickRun(int run, int runs) {
        std::cerr << "\r    run " << run << "/" << runs << "..." << std::flush;
    }

    void clear() {
        std::cerr << "\r" << std::string(80, ' ') << "\r" << std::flush;
    }

private:
    int width_;
};
