#ifndef ADJACENCYMATRIX_H
#define ADJACENCYMATRIX_H

#include "Model/DataStructure/IGraph.h"

class AdjacencyMatrix : public IGraph {
    private:
    bool isDirected;
    std::vector<std::vector<int>> matrix;

    void resizeMatrix(int newSize);

    public:
    AdjacencyMatrix(bool directed = false);

    void addEdge(int from, int to, int weight = 1) override;
    void deleteEdge(int from, int to) override;
    bool hasEdge(int from, int to) const override;
    
    std::vector<int> getVertices() const override;
    std::vector<Edge> getEdges() const override;

    std::vector<int> getNeighbours(int vertex) const override;
    std::vector<Edge> getEdgesFromVertex(int vertex) const override;

    void initialize(const std::vector<int>& data, Timeline& timeline) override;
    void insert(int value, Timeline& timeline) override;
    void remove(int value, Timeline& timeline) override;
    void search(int value, Timeline& timeline) override;
    void clear(Timeline& timeline) override;
};

#endif // ADJACENCYMATRIX_H