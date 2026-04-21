#ifndef PAYLOADS_H
#define PAYLOADS_H

#include "IPayload.h"
#include "Utilities/Position.h"
#include "Model/GraphStructure/Edge.h"
#include <string>
#include <vector>
#include <map>

struct LinkedListPayload : public IPayload {
    std::vector<int> values;
    std::vector<size_t> highlightedNodes;
    std::map<std::string, size_t> pointers; // e.g., {"head": 0, "tail": 4, "current": 2}

    LinkedListPayload() = default;
    LinkedListPayload(std::vector<int> values, std::vector<size_t> highlightedNodes = {}, std::map<std::string, size_t> pointers = {})
        : values(std::move(values)), highlightedNodes(std::move(highlightedNodes)), pointers(std::move(pointers)) {}
        
    void accept(IPayloadVisitor& visitor) const override { visitor.visit(*this); }
    IPayload* clone() const override { return new LinkedListPayload(*this); }
};

struct TreeNodeData {
    size_t id;
    int value;
    size_t leftId = -1;
    size_t rightId = -1;
    size_t height = 0;
};

struct TreePayload : public IPayload {
    std::vector<TreeNodeData> nodes;
    std::vector<Position> positions; // Mapped to nodes by id or index
    size_t rootId;
    std::vector<size_t> highlightedNodes;
    std::map<std::string, size_t> pointers; // e.g., {"current": 2}

    TreePayload() : rootId(0) {}
    TreePayload(std::vector<TreeNodeData> nodes, size_t rootId = 0, std::vector<size_t> highlightedNodes = {}, std::map<std::string, size_t> pointers = {}, std::vector<Position> positions = {})
        : nodes(std::move(nodes)), positions(std::move(positions)), rootId(rootId), highlightedNodes(std::move(highlightedNodes)), pointers(std::move(pointers)) {}
        
    void accept(IPayloadVisitor& visitor) const override { visitor.visit(*this); }
    IPayload* clone() const override { return new TreePayload(*this); }
};

struct HeapPayload : public IPayload {
    std::vector<int> arrayData;
    std::vector<size_t> highlightedIndices;

    HeapPayload() = default;
    HeapPayload(std::vector<int> arrayData, std::vector<size_t> highlightedIndices = {})
        : arrayData(std::move(arrayData)), highlightedIndices(std::move(highlightedIndices)) {}
        
    void accept(IPayloadVisitor& visitor) const override { visitor.visit(*this); }
    IPayload* clone() const override { return new HeapPayload(*this); }
};

struct GraphPayload : public IPayload {
    std::vector<size_t> vertices;
    std::vector<Position> positions; // Mapped to vertices 1:1
    std::vector<Edge> edges;
    std::vector<size_t> highlightedVertices;
    std::vector<Edge> highlightedEdges;

    GraphPayload() = default;
    GraphPayload(
        std::vector<size_t> vertices,
        std::vector<Edge> edges = {},
        std::vector<size_t> highlightedVertices = {},
        std::vector<Edge> highlightedEdges = {},
        std::vector<Position> positions = {})
        : vertices(std::move(vertices)), positions(std::move(positions)), edges(std::move(edges)), highlightedVertices(std::move(highlightedVertices)), highlightedEdges(std::move(highlightedEdges)) {}
        
    void accept(IPayloadVisitor& visitor) const override { visitor.visit(*this); }
    IPayload* clone() const override { return new GraphPayload(*this); }
};

struct SingleSourcePayload : public IPayload {
    GraphPayload baseGraph;
    std::vector<int> distances;
    std::vector<size_t> previousVertices; // For path reconstruction
    std::vector<std::pair<int, size_t>> priorityQueueSnapShot;

    SingleSourcePayload() = default;
    SingleSourcePayload(
        GraphPayload baseGraph,
        std::vector<int> distances = {},
        std::vector<size_t> previousVertices = {},
        std::vector<std::pair<int, size_t>> priorityQueueSnapShot = {})
        : baseGraph(std::move(baseGraph)), distances(std::move(distances)), previousVertices(std::move(previousVertices)), priorityQueueSnapShot(std::move(priorityQueueSnapShot)) {}
        
    void accept(IPayloadVisitor& visitor) const override { visitor.visit(*this); }
    IPayload* clone() const override { return new SingleSourcePayload(*this); }
};

struct AStarPayload : public IPayload {
    GraphPayload baseGraph;
    std::vector<int> gCosts;
    std::vector<int> hCosts;
    std::vector<int> fCosts;
    std::vector<std::pair<int, size_t>> priorityQueueSnapShot;
    size_t targetVertex;

    AStarPayload() : targetVertex(0) {}
    AStarPayload(
        GraphPayload baseGraph,
        std::vector<int> gCosts = {},
        std::vector<int> hCosts = {},
        std::vector<int> fCosts = {},
        std::vector<std::pair<int, size_t>> priorityQueueSnapShot = {},
        size_t targetVertex = 0)
        : baseGraph(std::move(baseGraph)), gCosts(std::move(gCosts)), hCosts(std::move(hCosts)), fCosts(std::move(fCosts)), priorityQueueSnapShot(std::move(priorityQueueSnapShot)), targetVertex(targetVertex) {}
        
    void accept(IPayloadVisitor& visitor) const override { visitor.visit(*this); }
    IPayload* clone() const override { return new AStarPayload(*this); }
};

struct AllPairsPayload : public IPayload {
    GraphPayload baseGraph;
    std::vector<std::vector<int>> distances;
    std::vector<std::vector<size_t>> nextVertices; // For path reconstruction
    std::vector<size_t> currentPath;

    AllPairsPayload() = default;
    AllPairsPayload(
        GraphPayload baseGraph,
        std::vector<std::vector<int>> distances = {},
        std::vector<std::vector<size_t>> nextVertices = {},
        std::vector<size_t> currentPath = {})
        : baseGraph(std::move(baseGraph)), distances(std::move(distances)), nextVertices(std::move(nextVertices)), currentPath(std::move(currentPath)) {}
        
    void accept(IPayloadVisitor& visitor) const override { visitor.visit(*this); }
    IPayload* clone() const override { return new AllPairsPayload(*this); }
};

#endif // PAYLOADS_H
