#include "EdgeListParser.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <unordered_map>
#include <unordered_set>

EdgeListParser::EdgeListParser(bool directed, char delimiter)
    : directed_(directed), delimiter_(delimiter) {}

Graph EdgeListParser::parse(const std::string& filepath) const {
    std::ifstream file(filepath);
    if (!file.is_open())
        throw std::runtime_error("Cannot open file: " + filepath);

    Graph g(directed_);
    std::string line;

    // Mapeo string -> int (cuando los IDs de vertice no son enteros)
    std::unordered_map<std::string, int> vertexMap;
    int nextId = 0;

    // Conjunto de aristas vistas para deduplicar (archivos que listan cada arista dos veces)
    std::unordered_set<long long> seen;

    auto getId = [&](const std::string& token) -> int {
        try { return std::stoi(token); }
        catch (...) {}
        auto it = vertexMap.find(token);
        if (it != vertexMap.end()) return it->second;
        vertexMap[token] = nextId;
        return nextId++;
    };

    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#' || line[0] == '%') continue;

        if (delimiter_ != ' ' && delimiter_ != '\t')
            for (char& c : line) if (c == delimiter_) c = ' ';

        std::istringstream ss(line);
        std::string tokA, tokB;
        if (!(ss >> tokA >> tokB)) continue;

        double weight = 1.0;
        ss >> weight;

        int src = getId(tokA);
        int dst = getId(tokB);

        // Salta aristas no dirigidas duplicadas (ej. archivos que listan A-B y B-A)
        if (!directed_) {
            int lo = std::min(src, dst), hi = std::max(src, dst);
            long long key = (long long)lo * 2000000LL + hi;
            if (!seen.insert(key).second) continue;
        }

        g.addEdge(src, dst, weight);
    }

    std::cout << "[Parser] " << filepath << " -> "
              << g.vertexCount() << "V, " << g.edgeCount() << "E\n";
    return g;
}
