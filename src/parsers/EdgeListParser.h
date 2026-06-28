#pragma once
#include "IParser.h"

// Parses files with lines: "src dst [weight]"
// Skips lines starting with '#' or '%'
class EdgeListParser : public IParser {
public:
    explicit EdgeListParser(bool directed = false, char delimiter = ' ');
    Graph parse(const std::string& filepath) const override;

private:
    bool directed_;
    char delimiter_;
};
