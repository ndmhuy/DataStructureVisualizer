#include "Model/GraphStructure/AdjacencyList.h"
#include "Utilities/GlobalConstant.h"
#include <queue>

AdjacencyList::AdjacencyList(const LayoutConfig& config, bool directed) : IGraphStructure(config), isDirected(directed) {}

void AdjacencyList::addVertex(Timeline* timeline) {
    adjacencyList[vertexCount];
    ++vertexCount;
    invalidateLayoutCache();

    if (timeline) {
        timeline->addFrame(Frame(makeGraphPayload({vertexCount - 1}), 0, "Added vertex " + std::to_string(vertexCount - 1)));
    }
}

void AdjacencyList::removeVertex(Timeline* timeline) {
    if (vertexCount == 0) {
        if (timeline) {
            timeline->addFrame(Frame(makeGraphPayload(), 0, "No vertex to remove."));
        }
        return;
    }

    size_t removedVertex = vertexCount - 1;

    if (timeline) {
        std::vector<Edge> removedEdges = getEdgesFromVertex(removedVertex);
        for (const auto& [vertex, neighbours] : adjacencyList) {
            if (vertex == removedVertex) {
                continue;
            }
            for (const auto& node : neighbours) {
                if (node.vertex == removedVertex) {
                    removedEdges.emplace_back(vertex, removedVertex, node.weight);
                }
            }
        }
        timeline->addFrame(Frame(makeGraphPayload({removedVertex}, removedEdges), 0, "Removing vertex " + std::to_string(removedVertex) + " and its incident edges"));
    }

    adjacencyList.erase(removedVertex);

    for (auto& [vertex, neighbours] : adjacencyList) {
        neighbours.remove_if([removedVertex](const GraphNode& node) { return node.vertex == removedVertex; });
    }

    --vertexCount;
    invalidateLayoutCache();

    if (timeline) {
        timeline->addFrame(Frame(makeGraphPayload(), 0, "Removed vertex " + std::to_string(removedVertex)));
    }
}

void AdjacencyList::addEdge(size_t from, size_t to, int weight, Timeline* timeline) {
    bool createdFrom = adjacencyList.find(from) == adjacencyList.end();
    bool createdTo = adjacencyList.find(to) == adjacencyList.end();

    adjacencyList[from];
    adjacencyList[to];

    if (timeline) {
        if (createdFrom) {
            timeline->addFrame(Frame(makeGraphPayload({from}), 0, "Added vertex " + std::to_string(from) + " while creating edge"));
        }
        if (createdTo && to != from) {
            timeline->addFrame(Frame(makeGraphPayload({to}), 0, "Added vertex " + std::to_string(to) + " while creating edge"));
        }
    }
    
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
    invalidateLayoutCache();
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
        invalidateLayoutCache();
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
        if (edge.to == INVALID_INDEX) {
            adjacencyList[edge.from];
        } else {
            addEdge(edge.from, edge.to, edge.weight);
        }
    }
    timeline.addFrame(Frame(makeGraphPayload({}, {}), 0, "Initialization complete."));
}

void AdjacencyList::clear(Timeline& timeline) {
    timeline.addFrame(Frame(makeGraphPayload({}, {}), 0, "Clearing Adjacency List..."));
    adjacencyList.clear();
    vertexCount = 0;
    invalidateLayoutCache();
    timeline.addFrame(Frame(makeGraphPayload({}, {}), 0, "Adjacency List cleared..."));
}

StructureType AdjacencyList::getStructureType() const {
    return StructureType::AdjacencyList;
}