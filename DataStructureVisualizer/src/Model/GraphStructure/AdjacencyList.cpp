#include "Model/GraphStructure/AdjacencyList.h"
#include <limits>
#include <queue>

AdjacencyList::AdjacencyList(const LayoutConfig& config, bool directed) : IGraphStructure(config), isDirected(directed) {}

void AdjacencyList::addVertex(Timeline* timeline) {
    adjacencyList[vertexCount];
    ++vertexCount;

    if (timeline) {
        timeline->addFrame(Frame(makeGraphPayload({vertexCount - 1}), 0, "Added vertex " + std::to_string(vertexCount - 1)));
    }
}

void AdjacencyList::addEdge(size_t from, size_t to, int weight, Timeline* timeline) {
    adjacencyList[from];
    adjacencyList[to];
    
    if (!hasEdge(from, to)) {
        adjacencyList[from].emplace_back(to, weight);
        if (!isDirected) {
            adjacencyList[to].emplace_back(from, weight);
        }
    } else { // Update weight if edge already exists
        auto &fromList = adjacencyList[from];
        for (auto &node : fromList) {
            if (node.vertex == to) {
                node.weight = weight;
                break;
            }
        }
        if (!isDirected) {
            auto &toList = adjacencyList[to];
            for (auto &node : toList) {
                if (node.vertex == from) {
                    node.weight = weight;
                    break;
                }
            }
        }
    }
    vertexCount = adjacencyList.size();

    if (timeline) {
        timeline->addFrame(Frame(makeGraphPayload({from, to}, {Edge(from, to, weight)}), 0, "Added edge from " + std::to_string(from) + " to " + std::to_string(to)));
    }
}

void AdjacencyList::deleteEdge(size_t from, size_t to, Timeline* timeline) {
    if (hasEdge(from, to)) {
        adjacencyList[from].remove_if([to](const GraphNode& node) { return node.vertex == to;});
        if (!isDirected) {
            adjacencyList[to].remove_if([from](const GraphNode& node) { return node.vertex == from;});
        }
    }

    if (timeline) {
        timeline->addFrame(Frame(makeGraphPayload(), 0, "Deleted edge from " + std::to_string(from) + " to " + std::to_string(to)));
    }
}

bool AdjacencyList::hasEdge(size_t from, size_t to) const {
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

std::vector<size_t> AdjacencyList::getVertices() const {
    std::vector<size_t> vertices;
    for (const auto& pair : adjacencyList) {
        vertices.push_back(pair.first);
    }
    return vertices;
}

// may return only one side in case of undirected graph if renderer needs
std::vector<Edge> AdjacencyList::getEdges() const {
    std::vector<Edge> edges;
    for (const auto& pair : adjacencyList) {
        size_t from = pair.first;
        for (const auto& node : pair.second) {
            edges.emplace_back(from, node.vertex, node.weight);
        }
    }
    return edges;
}

std::vector<size_t> AdjacencyList::getNeighbours(size_t vertex) const {
    std::vector<size_t> neighbours;
    auto it = adjacencyList.find(vertex);
    if (it != adjacencyList.end()) {
        for (const auto& node : it->second) {
            neighbours.push_back(node.vertex);
        }
    }
    return neighbours;
}

std::vector<Edge> AdjacencyList::getEdgesFromVertex(size_t vertex) const {
    std::vector<Edge> edges;
    auto it = adjacencyList.find(vertex);
    if (it != adjacencyList.end()) {
        for (const auto& node : it->second) {
            edges.emplace_back(vertex, node.vertex, node.weight);
        }
    }
    return edges;
}

void AdjacencyList::initialize(const std::vector<Edge>& startingEdges, Timeline& timeline) {
    clear(timeline);
    timeline.addFrame(Frame(makeGraphPayload({}, {}), 0, "Initializing Adjacency List with given edges..."));
    for (const auto& edge : startingEdges) {
        addEdge(edge.from, edge.to, edge.weight);
    }
    timeline.addFrame(Frame(makeGraphPayload({}, {}), 0, "Initialization complete."));
}

void AdjacencyList::clear(Timeline& timeline) {
    timeline.addFrame(Frame(makeGraphPayload({}, {}), 0, "Clearing Adjacency List..."));
    adjacencyList.clear();
    vertexCount = 0;
    timeline.addFrame(Frame(makeGraphPayload({}, {}), 0, "Adjacency List cleared..."));
}

StructureType AdjacencyList::getStructureType() const {
    return StructureType::AdjacencyList;
}