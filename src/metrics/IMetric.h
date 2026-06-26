#pragma once
#include <unordered_map>
#include <functional>
#include <string>
#include "graph/Graph.h"

// Callback type: (vertices/iterations done, total)
using ProgressFn = std::function<void(int done, int total)>;

class IMetric {
public:
    virtual ~IMetric() = default;

    virtual std::unordered_map<int, double> compute(
        const Graph& g,
        ProgressFn progress = nullptr) const = 0;

    virtual std::string name() const = 0;
};
