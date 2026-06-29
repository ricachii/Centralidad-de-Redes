#pragma once
#include "IParser.h"

// Parsea archivos con lineas: "src dst [peso]"
// Salta lineas que comienzan con '#' o '%'
class EdgeListParser : public IParser {
public:
    explicit EdgeListParser(bool directed = false, char delimiter = ' ');
    Graph parse(const std::string& filepath) const override;

private:
    bool directed_;
    char delimiter_;
};
