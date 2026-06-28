#pragma once
#include <string>
#include "graph/Graph.h"

class IParser {
public:
    virtual ~IParser() = default;
    virtual Graph parse(const std::string& filepath) const = 0;
};
