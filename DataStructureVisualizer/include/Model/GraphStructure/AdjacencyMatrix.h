#ifndef ADJACENCYMATRIX_H
#define ADJACENCYMATRIX_H

#include "Model/GraphStructure/IGraphStructure.h"

class AdjacencyMatrix : public IGraphStructure {
    private:
    bool isDirected;
    std::vector<std::vector<int>> matrix;

    void resizeMatrix(size_t newSize);
    bool isValidVertex(size_t vertex) const;

    public:
    AdjacencyMatrix(const LayoutConfig& config = LayoutConfig(), bool directed = false);

    void addVertex(Timeline* timeline = nullptr) override;
    void addEdge(size_t from, size_t to, int weight = 1, Timeline* timeline = nullptr) override;
    void deleteEdge(size_t from, size_t to, Timeline* timeline = nullptr) override;
    bool hasEdge(size_t from, size_t to) const override;

    std::vector<size_t> getVertices() const override;
    std::vector<Edge> getEdges() const override;

    std::vector<size_t> getNeighbours(size_t vertex) const override;
    std::vector<Edge> getEdgesFromVertex(size_t vertex) const override;

    void initialize(const std::vector<Edge>& startingEdges, Timeline& timeline) override;
    void clear(Timeline& timeline) override;

    StructureType getStructureType() const override;
};

#endif // ADJACENCYMATRIX_H