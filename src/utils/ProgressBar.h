#pragma once
#include <iostream>
#include <string>
#include <algorithm>

class ProgressBar {
public:
    explicit ProgressBar(int width = 38) : width_(width) {}

    // Call inside the metric loop. Only redraws every ~1% to avoid flicker.
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

    // Print final line for a completed run.
    void finishRun(int run, int runs, double ms) {
        std::cerr << "\r    run " << run << "/" << runs
                  << "  done - " << ms << " ms"
                  << "                              \n" << std::flush;
    }

    // Used for fast metrics (no internal progress): just show run number.
    void tickRun(int run, int runs) {
        std::cerr << "\r    run " << run << "/" << runs << "..." << std::flush;
    }

    void clear() {
        std::cerr << "\r" << std::string(80, ' ') << "\r" << std::flush;
    }

private:
    int width_;
};
