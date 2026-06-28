#pragma once
#include <vector>
#include <unordered_map>
#include <cmath>
#include <numeric>
#include <algorithm>

inline double mean(const std::vector<double>& v) {
    return std::accumulate(v.begin(), v.end(), 0.0) / v.size();
}

inline double variance(const std::vector<double>& v) {
    double m = mean(v);
    double sum = 0.0;
    for (double x : v) sum += (x - m) * (x - m);
    return sum / v.size();
}

inline double stddev(const std::vector<double>& v) {
    return std::sqrt(variance(v));
}

inline double meanScores(const std::unordered_map<int, double>& scores) {
    if (scores.empty()) return 0.0;
    double sum = 0.0;
    for (const auto& [id, val] : scores) sum += val;
    return sum / scores.size();
}
