#include "Model/DataStructure/AdjacencyList.h"

AdjacencyList::AdjacencyList(bool directed = false) : isDirected(directed) { vertexCount = 0; }

void AdjacencyList::addEdge(int from, int to, int weight = 1) {
    if (!hasEdge(from, to)) {
        adjacencyList[from].emplace_back(to, weight);
        if (!isDirected) {
            adjacencyList[to].emplace_back(from, weight);
        }
        vertexCount = adjacencyList.size();
    }
}

void AdjacencyList::deleteEdge(int from, int to) {
    if (hasEdge(from, to)) {
        adjacencyList[from].remove_if([to](const GraphNode& node) { return node.vertex == to;});
        if (!isDirected) {
            adjacencyList[to].remove_if([from](const GraphNode& node) { return node.vertex == from;});
        }
    }
}

bool AdjacencyList::hasEdge(int from, int to) const {
    auto it = adjacencyList.find(from);
    if (it != adjacencyList.end()) {
        for (const auto& node : it->second) {
            if (node.vertex == to) {
                return true;
            }
        }
    }
    return false;
}

std::vector<int> AdjacencyList::getVertices() const {
    std::vector<int> vertices;
    for (const auto& pair : adjacencyList) {
        vertices.push_back(pair.first);
    }
    return vertices;
}

// may return only one side in case of undirected graph if renderer needs
std::vector<Edge> AdjacencyList::getEdges() const {
    std::vector<Edge> edges;
    for (const auto& pair : adjacencyList) {
        int from = pair.first;
        for (const auto& node : pair.second) {
            edges.emplace_back(from, node.vertex, node.weight);
        }
    }
    return edges;
}

std::vector<int> AdjacencyList::getNeighbours(int vertex) const {
    std::vector<int> neighbours;
    auto it = adjacencyList.find(vertex);
    if (it != adjacencyList.end()) {
        for (const auto& node : it->second) {
            neighbours.push_back(node.vertex);
        }
    }
    return neighbours;
}

std::vector<Edge> AdjacencyList::getEdgesFromVertex(int vertex) const {
    std::vector<Edge> edges;
    auto it = adjacencyList.find(vertex);
    if (it != adjacencyList.end()) {
        for (const auto& node : it->second) {
            edges.emplace_back(vertex, node.vertex, node.weight);
        }
    }
    return edges;
}

virtual void AdjacencyList::initialize(const std::vector<int>& data, Timeline& timeline) {

}
virtual void AdjacencyList::insert(int value, Timeline& timeline) {

}
virtual void AdjacencyList::remove(int value, Timeline& timeline) {

}
virtual void AdjacencyList::search(int value, Timeline& timeline) {

}
virtual void AdjacencyList::clear(Timeline& timeline) {

}