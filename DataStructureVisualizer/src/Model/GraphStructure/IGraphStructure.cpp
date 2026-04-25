#include "Model/GraphStructure/IGraphStructure.h"

#include <algorithm>
#include <cmath>
#include <limits>
#include <queue>
#include <tuple>

namespace {
template <typename PayloadType>
void addPayloadFrame(Timeline& timeline, const PayloadType& payload, int codeLineId, const std::string& message) {
    timeline.addFrame(Frame(payload, codeLineId, message));
}
}

Edge reweightEdge(const Edge& edge, const std::vector<int>& h);

int IGraphStructure::heuristic(size_t vertex, size_t target, const std::vector<Position>& positions) {
    if (vertex >= positions.size() || target >= positions.size()) {
        return 0;
    }
    return static_cast<int>(Position::getDistance(positions[vertex], positions[target]));
}

std::vector<Position> IGraphStructure::generatePhysicsBasedLayout(const LayoutConfig& config) const {
    std::vector<Position> positions(vertexCount);
    if (vertexCount == 0) {
        return positions;
    }

    constexpr float pi = 3.14159265358979323846f;

    float centerX = config.screenWidth / 2.0f;
    float centerY = config.screenHeight / 2.0f;
    for (size_t i = 0; i < vertexCount; ++i) {
        float angle = static_cast<float>(i) * (2.0f * pi / static_cast<float>(vertexCount));
        float radius = 50.0f + 50.0f * (static_cast<float>(vertexCount) / 10.0f);
        positions[i] = Position(centerX + radius * std::cos(angle), centerY + radius * std::sin(angle));
    }

    float area = config.screenWidth * config.screenHeight;
    float k = std::sqrt(area / static_cast<float>(vertexCount));
    float temperature = config.initialTemperature;

    for (int iter = 0; iter < config.physicsIterations; ++iter) {
        std::vector<Position> displacements(vertexCount, Position());

        for (size_t v = 0; v < vertexCount; ++v) {
            for (size_t u = 0; u < vertexCount; ++u) {
                if (v == u) {
                    continue;
                }

                float dist = Position::getDistance(positions[v], positions[u]);
                if (dist <= 0.0001f) {
                    continue;
                }

                float force = (k * k) / dist;
                displacements[v] += (positions[v] - positions[u]) * (force / dist);
            }
        }

        for (const auto& edge : getEdges()) {
            if (edge.from >= positions.size() || edge.to >= positions.size()) {
                continue;
            }

            float dist = Position::getDistance(positions[edge.from], positions[edge.to]);
            if (dist <= 0.0001f) {
                continue;
            }

            float force = (dist * dist) / k;
            Position direction = (positions[edge.to] - positions[edge.from]) * (force / dist);
            displacements[edge.from] += direction;
            displacements[edge.to] -= direction;
        }

        for (size_t v = 0; v < vertexCount; ++v) {
            float dist = Position::getDistance(Position(), displacements[v]);
            if (dist > 0.0f) {
                float limitedDist = std::min(dist, temperature);
                positions[v] += displacements[v] * (limitedDist / dist);
            }

            positions[v].x = std::min(config.screenWidth - config.padding, std::max(config.padding, positions[v].x));
            positions[v].y = std::min(config.screenHeight - config.padding, std::max(config.padding, positions[v].y));
        }

        temperature *= config.coolingFactor;
    }

    return positions;
}

GraphPayload IGraphStructure::makeGraphPayload(const std::vector<size_t>& highlightedVertices, const std::vector<Edge>& highlightedEdges, const std::vector<Position>& customPositions) const {
    std::vector<Position> positions = (customPositions.size() == vertexCount)
        ? customPositions
        : generatePhysicsBasedLayout(layoutConfig);

    return GraphPayload(getVertices(), getEdges(), highlightedVertices, highlightedEdges, positions);
}

void IGraphStructure::runDAGShortestPath(size_t startVertex, Timeline& timeline) {
    const int INF = std::numeric_limits<int>::max();
    std::vector<int> distances(vertexCount, INF);
    std::vector<size_t> previousVertices(vertexCount, std::numeric_limits<size_t>::max());

    if (startVertex >= vertexCount) {
        addPayloadFrame(timeline, SingleSourcePayload(makeGraphPayload(), distances, previousVertices, {}), 16, "Start vertex is out of range. DAG shortest path not defined.");
        return;
    }

    distances[startVertex] = 0;

    std::vector<size_t> inDegree(vertexCount, 0);
    for (const auto& edge : getEdges()) {
        if (edge.to < inDegree.size()) {
            ++inDegree[edge.to];
        }
    }

    std::queue<size_t> q;
    for (size_t i = 0; i < vertexCount; ++i) {
        if (inDegree[i] == 0) {
            q.push(i);
        }
    }

    addPayloadFrame(timeline, SingleSourcePayload(makeGraphPayload({startVertex}), distances, previousVertices, {}), 0, "Running DAG shortest path...");

    size_t processed = 0;
    while (!q.empty()) {
        size_t vertex = q.front();
        q.pop();
        ++processed;

        addPayloadFrame(timeline, SingleSourcePayload(makeGraphPayload({vertex}), distances, previousVertices, {}), 7, "Processing vertex " + std::to_string(vertex));

        for (const auto& edge : getEdgesFromVertex(vertex)) {
            addPayloadFrame(timeline, SingleSourcePayload(makeGraphPayload({vertex, edge.to}, {edge}), distances, previousVertices, {}), 9,
                "Checking edge from " + std::to_string(edge.from) + " to " + std::to_string(edge.to));

            if (distances[vertex] != INF && distances[vertex] + edge.weight < distances[edge.to]) {
                distances[edge.to] = distances[vertex] + edge.weight;
                previousVertices[edge.to] = vertex;
                addPayloadFrame(timeline, SingleSourcePayload(makeGraphPayload({edge.to}, {edge}), distances, previousVertices, {}), 11,
                    "Relaxing edge to vertex " + std::to_string(edge.to) + ", new distance " + std::to_string(distances[edge.to]));
            }

            if (inDegree[edge.to] > 0) {
                --inDegree[edge.to];
                if (inDegree[edge.to] == 0) {
                    q.push(edge.to);
                }
            }
        }
    }

    if (processed != vertexCount) {
        addPayloadFrame(timeline, SingleSourcePayload(makeGraphPayload({}, {}), distances, previousVertices, {}), 16, "Graph contains a cycle. Shortest path not defined.");
    } else {
        std::vector<Edge> pathTreeEdges;
        for (size_t v = 0; v < vertexCount; ++v) {
            if (previousVertices[v] != std::numeric_limits<size_t>::max()) {
                for (const auto& e : getEdgesFromVertex(previousVertices[v])) {
                    if (e.to == v) {
                        pathTreeEdges.push_back(e);
                        break;
                    }
                }
            }
        }
        addPayloadFrame(timeline, SingleSourcePayload(makeGraphPayload({}, pathTreeEdges), distances, previousVertices, {}), 16, "DAG Shortest Path complete. Highlighted shortest path tree.");
    }
}

void IGraphStructure::runDijkstra(size_t startVertex, Timeline& timeline) {
    const int INF = std::numeric_limits<int>::max();
    std::vector<int> distances(vertexCount, INF);
    std::vector<size_t> previousVertices(vertexCount, std::numeric_limits<size_t>::max());

    if (startVertex >= vertexCount) {
        addPayloadFrame(timeline, SingleSourcePayload(makeGraphPayload(), distances, previousVertices, {}), 15, "Start vertex is out of range. Dijkstra's algorithm not defined.");
        return;
    }

    distances[startVertex] = 0;

    std::priority_queue<std::pair<int, size_t>, std::vector<std::pair<int, size_t>>, std::greater<>> pq;
    pq.emplace(0, startVertex);

    auto getSnapshot = [&pq]() {
        auto pqCopy = pq;
        std::vector<std::pair<int, size_t>> snapshot;
        while (!pqCopy.empty()) {
            snapshot.push_back(pqCopy.top());
            pqCopy.pop();
        }
        return snapshot;
    };

    addPayloadFrame(timeline, SingleSourcePayload(makeGraphPayload({startVertex}), distances, previousVertices, getSnapshot()), 0, "Running Dijkstra's Algorithm...");

    while (!pq.empty()) {
        auto [dist, vertex] = pq.top();
        pq.pop();

        if (dist > distances[vertex]) {
            continue;
        }

        addPayloadFrame(timeline, SingleSourcePayload(makeGraphPayload({vertex}), distances, previousVertices, getSnapshot()), 7,
            "Dequeued vertex " + std::to_string(vertex) + " with distance " + std::to_string(dist));

        for (const auto& edge : getEdgesFromVertex(vertex)) {
            addPayloadFrame(timeline, SingleSourcePayload(makeGraphPayload({vertex, edge.to}, {edge}), distances, previousVertices, getSnapshot()), 9,
                "Checking edge to " + std::to_string(edge.to) + " with weight " + std::to_string(edge.weight));

            if (distances[vertex] != INF && dist + edge.weight < distances[edge.to]) {
                distances[edge.to] = dist + edge.weight;
                previousVertices[edge.to] = vertex;
                pq.emplace(distances[edge.to], edge.to);
                addPayloadFrame(timeline, SingleSourcePayload(makeGraphPayload({edge.to}, {edge}), distances, previousVertices, getSnapshot()), 12,
                    "Relaxed edge. New distance to " + std::to_string(edge.to) + " is " + std::to_string(distances[edge.to]));
            }
        }
    }

    std::vector<Edge> pathTreeEdges;
    for (size_t v = 0; v < vertexCount; ++v) {
        if (previousVertices[v] != std::numeric_limits<size_t>::max()) {
            for (const auto& e : getEdgesFromVertex(previousVertices[v])) {
                if (e.to == v) {
                    pathTreeEdges.push_back(e);
                    break;
                }
            }
        }
    }
    addPayloadFrame(timeline, SingleSourcePayload(makeGraphPayload({}, pathTreeEdges), distances, previousVertices, getSnapshot()), 15, "Dijkstra's Algorithm complete. Highlighted shortest path tree.");
}

void IGraphStructure::runAStar(size_t startVertex, size_t targetVertex, Timeline& timeline) {
    const int INF = std::numeric_limits<int>::max();
    if (startVertex >= vertexCount || targetVertex >= vertexCount) {
        addPayloadFrame(timeline, AStarPayload(makeGraphPayload(), {}, {}, {}, {}, {}, targetVertex), 21, "Start or target vertex is out of range.");
        return;
    }

    std::vector<Position> positions = generatePhysicsBasedLayout(layoutConfig);
    std::vector<int> gCosts(vertexCount, INF);
    std::vector<int> hCosts(vertexCount, INF);
    std::vector<int> fCosts(vertexCount, INF);
    std::vector<size_t> previousVertices(vertexCount, std::numeric_limits<size_t>::max());

    gCosts[startVertex] = 0;
    hCosts[startVertex] = heuristic(startVertex, targetVertex, positions);
    fCosts[startVertex] = hCosts[startVertex];

    std::priority_queue<std::tuple<int, int, size_t>, std::vector<std::tuple<int, int, size_t>>, std::greater<>> pq;
    pq.emplace(fCosts[startVertex], 0, startVertex);

    auto getSnapshot = [&pq]() {
        auto pqCopy = pq;
        std::vector<std::pair<int, size_t>> snapshot;
        while (!pqCopy.empty()) {
            auto [f, g, v] = pqCopy.top();
            snapshot.emplace_back(f, v);
            pqCopy.pop();
        }
        return snapshot;
    };

    auto makePayload = [&](const std::vector<size_t>& highlightedVertices = {}, const std::vector<Edge>& highlightedEdges = {}) {
        return AStarPayload(makeGraphPayload(highlightedVertices, highlightedEdges, positions), gCosts, hCosts, fCosts, previousVertices, getSnapshot(), targetVertex);
    };

    addPayloadFrame(timeline, makePayload({startVertex}), 0, "Running A* Algorithm...");

    while (!pq.empty()) {
        auto [fScore, gScore, vertex] = pq.top();
        pq.pop();

        addPayloadFrame(timeline, makePayload({vertex}), 8, "Visiting vertex " + std::to_string(vertex));

        if (vertex == targetVertex) {
            std::vector<size_t> pathVertices;
            std::vector<Edge> pathEdges;
            size_t curr = targetVertex;
            while (curr != std::numeric_limits<size_t>::max()) {
                pathVertices.push_back(curr);
                size_t prev = previousVertices[curr];
                if (prev != std::numeric_limits<size_t>::max()) {
                    for (const auto& e : getEdgesFromVertex(prev)) {
                        if (e.to == curr) {
                            pathEdges.push_back(e);
                            break;
                        }
                    }
                }
                curr = prev;
            }
            addPayloadFrame(timeline, makePayload(pathVertices, pathEdges), 10, "Target reached. Reconstructed path.");
            return;
        }

        if (gScore > gCosts[vertex]) {
            continue;
        }

        for (const auto& edge : getEdgesFromVertex(vertex)) {
            addPayloadFrame(timeline, makePayload({vertex, edge.to}, {edge}), 13,
                "Checking edge from " + std::to_string(edge.from) + " to " + std::to_string(edge.to) + " with weight " + std::to_string(edge.weight));

            int tentativeGScore = gScore + edge.weight;
            if (tentativeGScore < gCosts[edge.to]) {
                gCosts[edge.to] = tentativeGScore;
                hCosts[edge.to] = heuristic(edge.to, targetVertex, positions);
                fCosts[edge.to] = gCosts[edge.to] + hCosts[edge.to];
                previousVertices[edge.to] = vertex;
                pq.emplace(fCosts[edge.to], gCosts[edge.to], edge.to);

                addPayloadFrame(timeline, makePayload({edge.to}, {edge}), 17,
                    "Relaxing edge to vertex " + std::to_string(edge.to) + ", new gCost " + std::to_string(gCosts[edge.to]) + ", new fCost " + std::to_string(fCosts[edge.to]));
            }
        }
    }

    addPayloadFrame(timeline, makePayload(), 21, "A* Algorithm complete. Target unreachable.");
}

void IGraphStructure::runBellmanFord(size_t startVertex, Timeline& timeline) {
    const int INF = std::numeric_limits<int>::max();
    const int NEG_INF = std::numeric_limits<int>::min();

    std::vector<int> distances(vertexCount, INF);
    std::vector<size_t> previousVertices(vertexCount, std::numeric_limits<size_t>::max());

    if (startVertex >= vertexCount) {
        addPayloadFrame(timeline, SingleSourcePayload(makeGraphPayload(), distances, previousVertices, {}), 14, "Start vertex is out of range. Bellman-Ford not defined.");
        return;
    }

    distances[startVertex] = 0;

    addPayloadFrame(timeline, SingleSourcePayload(makeGraphPayload({startVertex}), distances, previousVertices, {}), 0, "Running Bellman-Ford Algorithm...");

    for (size_t i = 0; i < vertexCount - 1; ++i) {
        addPayloadFrame(timeline, SingleSourcePayload(makeGraphPayload({}, {}), distances, previousVertices, {}), 3, "Relaxing all edges...");
        for (const auto& edge : getEdges()) {
            if (distances[edge.from] != INF && distances[edge.from] + edge.weight < distances[edge.to]) {
                distances[edge.to] = distances[edge.from] + edge.weight;
                previousVertices[edge.to] = edge.from;
                addPayloadFrame(timeline, SingleSourcePayload(makeGraphPayload({edge.from, edge.to}, {edge}), distances, previousVertices, {}), 5,
                    "Considering edge from " + std::to_string(edge.from) + " to " + std::to_string(edge.to));
            }
        }
    }

    std::queue<size_t> negativeCycleVertices;
    for (const auto& edge : getEdges()) {
        if (distances[edge.from] != INF && distances[edge.from] + edge.weight < distances[edge.to]) {
            distances[edge.to] = NEG_INF;
            negativeCycleVertices.push(edge.to);
            addPayloadFrame(timeline, SingleSourcePayload(makeGraphPayload({edge.to}, {edge}), distances, previousVertices, {}), 10,
                "Negative cycle detected involving vertex " + std::to_string(edge.to));
        }
    }

    while (!negativeCycleVertices.empty()) {
        size_t vertex = negativeCycleVertices.front();
        negativeCycleVertices.pop();

        for (const auto& edge : getEdgesFromVertex(vertex)) {
            if (distances[edge.to] != NEG_INF) {
                distances[edge.to] = NEG_INF;
                negativeCycleVertices.push(edge.to);
            }
        }
    }

    std::vector<Edge> pathTreeEdges;
    for (size_t v = 0; v < vertexCount; ++v) {
        if (distances[v] != NEG_INF && previousVertices[v] != std::numeric_limits<size_t>::max()) {
            for (const auto& e : getEdgesFromVertex(previousVertices[v])) {
                if (e.to == v) {
                    pathTreeEdges.push_back(e);
                    break;
                }
            }
        }
    }
    addPayloadFrame(timeline, SingleSourcePayload(makeGraphPayload({}, pathTreeEdges), distances, previousVertices, {}), 14, "Bellman-Ford Algorithm complete. Highlighted valid shortest path tree.");
}

void IGraphStructure::runFloydWarshall(Timeline& timeline) {
    const int INF = std::numeric_limits<int>::max();
    const int NEG_INF = std::numeric_limits<int>::min();

    std::vector<std::vector<int>> distances(vertexCount, std::vector<int>(vertexCount, INF));
    std::vector<std::vector<size_t>> nextVertices(vertexCount, std::vector<size_t>(vertexCount, std::numeric_limits<size_t>::max()));
    for (size_t i = 0; i < vertexCount; ++i) {
        distances[i][i] = 0;
        nextVertices[i][i] = i;
    }

    for (const auto& edge : getEdges()) {
        distances[edge.from][edge.to] = std::min(distances[edge.from][edge.to], edge.weight);
        nextVertices[edge.from][edge.to] = edge.to;
    }

    addPayloadFrame(timeline, AllPairsPayload(makeGraphPayload(), distances, nextVertices), 0, "Running Floyd-Warshall Algorithm...");

    for (size_t k = 0; k < vertexCount; ++k) {
        addPayloadFrame(timeline, AllPairsPayload(makeGraphPayload({k}), distances, nextVertices), 8,
            "Considering intermediate vertex " + std::to_string(k));
        for (size_t i = 0; i < vertexCount; ++i) {
            for (size_t j = 0; j < vertexCount; ++j) {
                if (distances[i][k] != INF && distances[k][j] != INF && distances[i][k] + distances[k][j] < distances[i][j]) {
                    distances[i][j] = distances[i][k] + distances[k][j];
                    nextVertices[i][j] = nextVertices[i][k];
                    addPayloadFrame(timeline, AllPairsPayload(makeGraphPayload({i, j}), distances, nextVertices), 14,
                        "Relaxing path " + std::to_string(i) + " -> " + std::to_string(j) + " via " + std::to_string(k));
                }
            }
        }
    }

    for (size_t k = 0; k < vertexCount; ++k) {
        if (distances[k][k] < 0) {
            for (size_t i = 0; i < vertexCount; ++i) {
                for (size_t j = 0; j < vertexCount; ++j) {
                    if (distances[i][k] != INF && distances[k][j] != INF) {
                        distances[i][j] = NEG_INF;
                    }
                }
            }
        }
    }

    addPayloadFrame(timeline, AllPairsPayload(makeGraphPayload(), distances, nextVertices), 16, "Floyd-Warshall Algorithm complete.");
}

void IGraphStructure::runJohnson(Timeline& timeline) {
    const int INF = std::numeric_limits<int>::max();
    if (vertexCount == 0) {
        addPayloadFrame(timeline, AllPairsPayload(makeGraphPayload(), {}, {}), 16, "Graph is empty.");
        return;
    }

    addPayloadFrame(timeline, AllPairsPayload(makeGraphPayload(), {}, {}), 0, "Running Johnson's Algorithm...");

    std::vector<Edge> edges = getEdges();
    size_t superSource = vertexCount;
    std::vector<Edge> augmentedEdges = edges;
    for (size_t vertex = 0; vertex < vertexCount; ++vertex) {
        augmentedEdges.emplace_back(superSource, vertex, 0);
    }

    std::vector<int> h(vertexCount + 1, INF);
    h[superSource] = 0;

    addPayloadFrame(timeline, AllPairsPayload(makeGraphPayload(), {}, {}), 2, "Added dummy vertex and edges for Bellman-Ford.");

    for (size_t i = 0; i < vertexCount; ++i) {
        bool changed = false;
        for (const auto& edge : augmentedEdges) {
            if (edge.from < h.size() && edge.to < h.size() && h[edge.from] != INF && h[edge.from] + edge.weight < h[edge.to]) {
                h[edge.to] = h[edge.from] + edge.weight;
                changed = true;
            }
        }
        if (!changed) {
            break;
        }
    }

    for (const auto& edge : augmentedEdges) {
        if (edge.from < h.size() && edge.to < h.size() && h[edge.from] != INF && h[edge.from] + edge.weight < h[edge.to]) {
            addPayloadFrame(timeline, AllPairsPayload(makeGraphPayload({}, {}), {}, {}), 4, "Negative cycle detected by Bellman-Ford. Johnson's algorithm terminates.");
            return;
        }
    }

    std::vector<std::vector<int>> distances(vertexCount, std::vector<int>(vertexCount, INF));
    std::vector<std::vector<size_t>> nextVertices(vertexCount, std::vector<size_t>(vertexCount, std::numeric_limits<size_t>::max()));

    for (size_t source = 0; source < vertexCount; ++source) {
        std::vector<size_t> prev(vertexCount, std::numeric_limits<size_t>::max());
        addPayloadFrame(timeline, AllPairsPayload(makeGraphPayload({source}), distances, nextVertices), 10, "Running Dijkstra from vertex " + std::to_string(source));
        std::priority_queue<std::pair<int, size_t>, std::vector<std::pair<int, size_t>>, std::greater<>> pq;
        pq.emplace(0, source);
        distances[source][source] = 0;

        while (!pq.empty()) {
            auto [dist, vertex] = pq.top();
            pq.pop();
            if (dist > distances[source][vertex]) {
                continue;
            }

            for (const auto& edge : getEdgesFromVertex(vertex)) {
                Edge reweighted = reweightEdge(edge, h);
                int newDist = dist + reweighted.weight;
                if (newDist < distances[source][edge.to]) {
                    distances[source][edge.to] = newDist;
                    prev[edge.to] = vertex;
                    pq.emplace(newDist, edge.to);
                }
            }
        }

        for (size_t target = 0; target < vertexCount; target++) {
            if (target != source && prev[target] != std::numeric_limits<size_t>::max()) {
                size_t curr = target;
                while (prev[curr] != source && prev[curr] != std::numeric_limits<size_t>::max()) {
                    curr = prev[curr];
                }
                if (prev[curr] == source) {
                    nextVertices[source][target] = curr;
                }
            }
        }
    }

    addPayloadFrame(timeline, AllPairsPayload(makeGraphPayload(), distances, nextVertices), 15, "Restoring original edge weights.");

    for (size_t i = 0; i < vertexCount; ++i) {
        for (size_t j = 0; j < vertexCount; ++j) {
            if (distances[i][j] != INF) {
                distances[i][j] += h[j] - h[i];
            }
        }
    }

    addPayloadFrame(timeline, AllPairsPayload(makeGraphPayload(), distances, nextVertices), 16, "Johnson's Algorithm complete.");
}

Edge reweightEdge(const Edge& edge, const std::vector<int>& h) {
    int newWeight = edge.weight + h[edge.from] - h[edge.to];
    return Edge(edge.from, edge.to, newWeight);
}
