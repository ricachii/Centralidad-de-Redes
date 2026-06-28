#include "PajekParser.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <algorithm>
#include <cctype>

PajekParser::PajekParser(bool directed) : directed_(directed) {}

Graph PajekParser::parse(const std::string& filepath) const {
    std::ifstream file(filepath);
    if (!file.is_open())
        throw std::runtime_error("Cannot open: " + filepath);

    Graph g(directed_);
    std::string line;

    enum class Section { NONE, VERTICES, ARCS, EDGES } section = Section::NONE;

    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::string lower = line;
        std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
        // Trim leading whitespace for section detection
        size_t first = lower.find_first_not_of(" \t");
        if (first == std::string::npos) continue;
        std::string trimmed = lower.substr(first);

        if (trimmed.rfind("*vertices", 0) == 0) { section = Section::VERTICES; continue; }
        if (trimmed.rfind("*arcs",     0) == 0) { section = Section::ARCS;     continue; }
        if (trimmed.rfind("*edges",    0) == 0) { section = Section::EDGES;    continue; }

        std::istringstream ss(line);

        if (section == Section::VERTICES) {
            int id;
            if (ss >> id) g.addVertex(id);

        } else if (section == Section::ARCS) {
            int src, dst;
            double weight = 1.0;
            if (!(ss >> src >> dst)) continue;
            ss >> weight;
            g.addEdge(src, dst, weight);

        } else if (section == Section::EDGES) {
            int src, dst;
            double weight = 1.0;
            if (!(ss >> src >> dst)) continue;
            ss >> weight;
            // *Edges are undirected: add both directions explicitly
            g.addEdge(src, dst, weight);
            if (directed_) g.addEdge(dst, src, weight);
        }
    }

    std::cout << "[PajekParser] " << filepath << " -> "
              << g.vertexCount() << "V, " << g.edgeCount() << "E\n";
    return g;
}
