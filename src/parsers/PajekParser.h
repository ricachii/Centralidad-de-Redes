#pragma once
#include "IParser.h"

// Parsea el formato Pajek .net:
//   *Vertices N
//      id "label" [x y z]
//   *Arcs       <- aristas dirigidas
//      src dst peso
//   *Edges      <- aristas no dirigidas (se agregan en ambos sentidos)
//      src dst peso
class PajekParser : public IParser {
public:
    explicit PajekParser(bool directed = true);
    Graph parse(const std::string& filepath) const override;

private:
    bool directed_;
};
