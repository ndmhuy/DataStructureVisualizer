#ifndef IGRAPHSTRUCTURE_H
#define IGRAPHSTRUCTURE_H

#include "Model/IVisualizable.h"
#include <vector>

struct Edge {
    int from;
    int to;
    int weight;

    Edge(int f, int t, int w = 1) : from(f), to(t), weight(w) {}
};

class IGraphStructure : public IVisualizable {
    protected:
    size_t vertexCount;

    public:
    virtual ~IGraphStructure() = default;

    size_t size() const { return vertexCount; }
    bool empty() const { return vertexCount == 0; }

    virtual void addEdge(int from, int to, int weight = 1) = 0;
    virtual void deleteEdge(int from, int to) = 0;
    virtual bool hasEdge(int from, int to) const = 0;
    
    virtual std::vector<int> getVertices() const = 0;
    virtual std::vector<Edge> getEdges() const = 0;

    virtual std::vector<int> getNeighbours(int vertex) const = 0;
    virtual std::vector<Edge> getEdgesFromVertex(int vertex) const = 0;

    virtual void initialize(const std::vector<Edge>& startingEdges, Timeline& timeline) = 0;
};

#endif // IGRAPHSTRUCTURE_H