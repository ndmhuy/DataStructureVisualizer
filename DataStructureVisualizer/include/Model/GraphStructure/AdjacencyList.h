#ifndef ADJACENCYLIST_H
#define ADJACENCYLIST_H

#include "Model/GraphStructure/IGraphStructure.h"
#include <unordered_map>
#include <list>

class AdjacencyList : public IGraphStructure {
    private:
    bool isDirected;

    struct GraphNode {
        size_t vertex;
        int weight;

        GraphNode(size_t v, int w = 1) : vertex(v), weight(w) {}
    };

    std::unordered_map<size_t, std::list<GraphNode>> adjacencyList;

    
    public:
    AdjacencyList(const LayoutConfig& config, bool directed = false);

    void addVertex(Timeline* timeline = nullptr) override;
    void addEdge(size_t from, size_t to, int weight = 1, Timeline* timeline = nullptr) override;
    void deleteEdge(size_t from, size_t to, Timeline* timeline = nullptr) override;
    bool hasEdge(size_t from, size_t to) const override;
    
    std::vector<size_t> getVertices() const override;
    std::vector<Edge> getEdges() const override;

    std::vector<size_t> getNeighbours(size_t vertex) const override;
    std::vector<Edge> getEdgesFromVertex(size_t vertex) const override;

    virtual void initialize(const std::vector<Edge>& startingEdges, Timeline& timeline) override;
    virtual void clear(Timeline& timeline) override;

    StructureType getStructureType() const override;
};

#endif // ADJACENCYLIST_H