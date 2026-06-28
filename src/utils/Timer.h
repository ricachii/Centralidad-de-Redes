#pragma once
#include <chrono>

class Timer {
public:
    void start() {
        t0_ = std::chrono::high_resolution_clock::now();
    }
    double elapsedMs() const {
        auto t1 = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double, std::milli>(t1 - t0_).count();
    }

private:
    std::chrono::high_resolution_clock::time_point t0_;
};
