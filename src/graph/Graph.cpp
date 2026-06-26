#include "Graph.h"
#include <algorithm>

Graph::Graph(bool directed) : directed_(directed) {}

void Graph::addVertex(int id) {
    adj_.emplace(id, std::vector<Edge>{});
}

void Graph::addEdge(int from, int to, double weight) {
    adj_.emplace(from, std::vector<Edge>{});
    adj_.emplace(to,   std::vector<Edge>{});

    adj_[from].push_back({to, weight});
    edgeCount_++;

    if (!directed_) {
        adj_[to].push_back({from, weight});
    }
}

void Graph::removeEdge(int from, int to) {
    auto it = adj_.find(from);
    if (it == adj_.end()) return;

    auto& vec = it->second;
    auto prev = vec.size();
    vec.erase(std::remove_if(vec.begin(), vec.end(),
        [to](const Edge& e){ return e.to == to; }), vec.end());

    long long removed = (long long)(prev - vec.size());
    edgeCount_ -= removed;

    if (!directed_) {
        auto& vec2 = adj_[to];
        vec2.erase(std::remove_if(vec2.begin(), vec2.end(),
            [from](const Edge& e){ return e.to == from; }), vec2.end());
    }
}

bool Graph::hasVertex(int id) const {
    return adj_.count(id) > 0;
}

bool Graph::hasEdge(int from, int to) const {
    auto it = adj_.find(from);
    if (it == adj_.end()) return false;
    for (const auto& e : it->second)
        if (e.to == to) return true;
    return false;
}

int Graph::vertexCount() const {
    return (int)adj_.size();
}

long long Graph::edgeCount() const {
    return edgeCount_;
}

const std::vector<Graph::Edge>& Graph::neighbors(int v) const {
    auto it = adj_.find(v);
    if (it == adj_.end())
        throw std::out_of_range("Vertex not found: " + std::to_string(v));
    return it->second;
}

std::vector<int> Graph::vertices() const {
    std::vector<int> result;
    result.reserve(adj_.size());
    for (const auto& [id, _] : adj_)
        result.push_back(id);
    return result;
}

size_t Graph::memoryBytes() const {
    size_t total = sizeof(*this);
    total += adj_.bucket_count() * (sizeof(void*) + sizeof(int) + sizeof(std::vector<Edge>));
    for (const auto& [id, vec] : adj_)
        total += vec.capacity() * sizeof(Edge);
    return total;
}
