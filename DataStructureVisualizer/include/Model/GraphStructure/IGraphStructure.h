#ifndef IGRAPHSTRUCTURE_H
#define IGRAPHSTRUCTURE_H

#include "Model/IVisualizable.h"
#include <vector>

struct Edge {
    size_t from;
    size_t to;
    int weight;

    Edge(size_t f, size_t t, int w = 1) : from(f), to(t), weight(w) {}
};

class IGraphStructure : public IVisualizable {
    protected:
    size_t vertexCount;

    public:
    virtual ~IGraphStructure() = default;

    size_t size() const { return vertexCount; }
    bool empty() const { return vertexCount == 0; }

    virtual void addEdge(size_t from, size_t to, int weight = 1) = 0;
    virtual void deleteEdge(size_t from, size_t to) = 0;
    virtual bool hasEdge(size_t from, size_t to) const = 0;

    virtual std::vector<size_t> getVertices() const = 0;
    virtual std::vector<Edge> getEdges() const = 0;

    virtual std::vector<size_t> getNeighbours(size_t vertex) const = 0;
    virtual std::vector<Edge> getEdgesFromVertex(size_t vertex) const = 0;

    virtual void initialize(const std::vector<Edge>& startingEdges, Timeline& timeline) = 0;
};

#endif // IGRAPHSTRUCTURE_H