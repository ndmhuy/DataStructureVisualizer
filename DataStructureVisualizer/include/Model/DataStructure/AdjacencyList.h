#ifndef ADJACENCYLIST_H
#define ADJACENCYLIST_H

#include "Model/DataStructure/IGraph.h"
#include <unordered_map>
#include <list>

class AdjacencyList : public IGraph {
    private:
    bool isDirected;

    struct GraphNode {
        int vertex;
        int weight;

        GraphNode(int v, int w = 1) : vertex(v), weight(w) {}
    };

    std::unordered_map<int, std::list<GraphNode>> adjacencyList;

    
    public:
    AdjacencyList(bool directed = false) : isDirected(directed) {}

    void addEdge(int from, int to, int weight = 1) override;
    void deleteEdge(int from, int to) override;
    bool hasEdge(int from, int to) const override;
    
    std::vector<int> getVertices() const override;
    std::vector<Edge> getEdges() const override;

    std::vector<int> getNeighbours(int vertex) const override;
    std::vector<Edge> getEdgesFromVertex(int vertex) const override;

    virtual void initialize(const std::vector<int>& data, Timeline& timeline) override;
    virtual void insert(int value, Timeline& timeline) override;
    virtual void remove(int value, Timeline& timeline) override;
    virtual void search(int value, Timeline& timeline) override;
    virtual void clear(Timeline& timeline) override;
};

#endif // ADJACENCYLIST_H