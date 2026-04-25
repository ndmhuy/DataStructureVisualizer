#include "Model/GraphStructure/AdjacencyList.h"
#include <limits>
#include <queue>
#include <cstddef>

const size_t INVALID_INDEX = std::numeric_limits<size_t>::max();

AdjacencyList::AdjacencyList(bool directed) : isDirected(directed) { vertexCount = 0; }

void AdjacencyList::addEdge(size_t from, size_t to, int weight) {
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
}

void AdjacencyList::deleteEdge(size_t from, size_t to) {
    if (hasEdge(from, to)) {
        adjacencyList[from].remove_if([to](const GraphNode& node) { return node.vertex == to;});
        if (!isDirected) {
            adjacencyList[to].remove_if([from](const GraphNode& node) { return node.vertex == from;});
        }
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
    timeline.addFrame(Frame(GraphPayload(getVertices(), getEdges(), {}, {}), 0, "Initializing Adjacency List with given edges..."));
    for (const auto& edge : startingEdges) {
        if (edge.to == INVALID_INDEX) {
            adjacencyList[edge.from];
        } else {
            addEdge(edge.from, edge.to, edge.weight);
        }
    }
    timeline.addFrame(Frame(GraphPayload(getVertices(), getEdges(), {}, {}), 0, "Initialization complete."));
}

void AdjacencyList::clear(Timeline& timeline) {
    timeline.addFrame(Frame(GraphPayload(getVertices(), getEdges(), {}, {}), 0, "Clearing Adjacency List..."));
    adjacencyList.clear();
    vertexCount = 0;
    timeline.addFrame(Frame(GraphPayload(getVertices(), getEdges(), {}, {}), 0, "Adjacency List cleared..."));
}

StructureType AdjacencyList::getStructureType() const {
    return StructureType::AdjacencyList;
}

void AdjacencyList::runDijkstra(size_t startVertex, Timeline& timeline) {
    const int INF = std::numeric_limits<int>::max();
    std::vector<int> distances(vertexCount, INF);
    distances[startVertex] = 0;

    std::priority_queue<std::pair<int, size_t>, std::vector<std::pair<int, size_t>>, std::greater<>> pq;
    pq.emplace(0, startVertex);

    std::vector<size_t> vertices = getVertices();
    std::vector<Edge> edges = getEdges();
    timeline.addFrame(Frame(distances, vertices, edges, {startVertex}, {}, 0, "Running Dijkstra's Algorithm..."));

    while (!pq.empty()) {
        auto [dist, vertex] = pq.top();
        pq.pop();

        if (dist > distances[vertex]) continue;

        timeline.addFrame(Frame(distances, vertices, edges, {vertex}, {}, 1, "Visiting vertex " + std::to_string(vertex) + " with current distance " + std::to_string(dist)));

        for (const auto& edge : getEdgesFromVertex(vertex)) {
            timeline.addFrame(Frame(distances, vertices, edges, {vertex, edge.to}, {edge}, 2, "Checking edge from " + std::to_string(edge.from) + " to " + std::to_string(edge.to) + " with weight " + std::to_string(edge.weight)));
            
            if (distances[vertex] != INF && dist + edge.weight < distances[edge.to]) {
                distances[edge.to] = dist + edge.weight;
                pq.emplace(distances[edge.to], edge.to);
                timeline.addFrame(Frame(distances, vertices, edges, {edge.to}, {edge}, 3, "Relaxing edge to vertex " + std::to_string(edge.to) + ", new distance " + std::to_string(distances[edge.to])));
            }
        }
    }

    timeline.addFrame(Frame(distances, vertices, edges, {}, {}, 4, "Dijkstra's Algorithm complete."));
}

void AdjacencyList::runAStar(size_t startVertex, size_t targetVerTex, Timeline& timeline) {

}

void AdjacencyList::runBellmanFord(size_t startVertex, Timeline& timeline) {

}