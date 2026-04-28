#ifndef ADJACENCYMATRIX_H
#define ADJACENCYMATRIX_H

#include "Model/GraphStructure/IGraphStructure.h"

class AdjacencyMatrix : public IGraphStructure {
    private:
    bool isDirected;
    std::vector<std::vector<int>> matrix;

    // Resizes the backing matrix to match the vertex count.
    void resizeMatrix(size_t newSize);
    // Checks whether a vertex index is valid for the current matrix.
    bool isValidVertex(size_t vertex) const;

    public:
    // Creates an adjacency-matrix graph with the selected direction mode.
    AdjacencyMatrix(const LayoutConfig& config = LayoutConfig(), bool directed = false);

    // Adds a vertex to the matrix graph.
    void addVertex(Timeline* timeline = nullptr) override;
    // Removes the vertex with the largest id from the matrix graph.
    void removeVertex(Timeline* timeline = nullptr) override;
    // Adds or updates an edge in the matrix graph.
    void addEdge(size_t from, size_t to, int weight = 1, Timeline* timeline = nullptr) override;
    // Removes an edge from the matrix graph.
    void deleteEdge(size_t from, size_t to, Timeline* timeline = nullptr) override;
    // Returns whether the matrix contains the edge.
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
    void initialize(const std::vector<Edge>& startingEdges, Timeline& timeline) override;
    // Clears the graph contents.
    void clear(Timeline& timeline) override;

    // Returns the structure type for UI routing.
    StructureType getStructureType() const override;
};

#endif // ADJACENCYMATRIX_H