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
    // Creates an adjacency-list graph with the selected direction mode.
    AdjacencyList(const LayoutConfig& config, bool directed = false);

    // Adds a vertex to the adjacency list.
    void addVertex(Timeline* timeline = nullptr) override;
    // Removes the vertex with the largest id from the adjacency list.
    void removeVertex(Timeline* timeline = nullptr) override;
    // Adds or updates an edge in the adjacency list.
    void addEdge(size_t from, size_t to, int weight = 1, Timeline* timeline = nullptr) override;
    // Removes an edge from the adjacency list.
    void deleteEdge(size_t from, size_t to, Timeline* timeline = nullptr) override;
    // Returns whether the adjacency list contains the edge.
    bool hasEdge(size_t from, size_t to) const override;
    
    // Returns all vertices stored in the graph.
    std::vector<size_t> getVertices() const override;
    // Returns all edges stored in the graph.
    std::vector<Edge> getEdges() const override;

    // Returns the neighbours of a vertex.
    std::vector<size_t> getNeighbours(size_t vertex) const override;
    // Returns all outgoing edges for a vertex.
    std::vector<Edge> getEdgesFromVertex(size_t vertex) const override;

    // Loads the graph from a list of edges.
    virtual void initialize(const std::vector<Edge>& startingEdges, Timeline& timeline) override;
    // Clears the graph contents.
    virtual void clear(Timeline& timeline) override;

    // Returns the structure type for UI routing.
    StructureType getStructureType() const override;
};

#endif // ADJACENCYLIST_H