#pragma once
#include "IParser.h"

// Parses Pajek .net format:
//   *Vertices N
//      id "label" [x y z]
//   *Arcs       ← directed edges
//      src dst weight
//   *Edges      ← undirected edges (treated as directed both ways)
//      src dst weight
class PajekParser : public IParser {
public:
    explicit PajekParser(bool directed = true);
    Graph parse(const std::string& filepath) const override;

private:
    bool directed_;
};
