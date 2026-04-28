#ifndef IGRAPHSTRUCTURE_H
#define IGRAPHSTRUCTURE_H

#include "Model/IVisualizable.h"
#include "Model/GraphStructure/Edge.h"
#include "Utilities/Position.h"
#include "Utilities/LayoutConfig.h"
#include <vector>

class IGraphStructure : public IVisualizable {
    protected:
    size_t vertexCount;
    LayoutConfig layoutConfig;
    mutable std::vector<Position> cachedLayout;  // Cache generated layout to avoid regenerating each frame
    mutable bool layoutCacheValid = false;  // Flag to track if cache is current

    IGraphStructure(const LayoutConfig& config = LayoutConfig()) : vertexCount(0), layoutConfig(config) {}

    GraphPayload makeGraphPayload(const std::vector<size_t>& highlightedVertices = {}, const std::vector<Edge>& highlightedEdges = {}, const std::vector<Position>& customPositions = {}) const;

    int heuristic(size_t vertex, size_t target, const std::vector<Position>& positions);

    std::vector<Position> generatePhysicsBasedLayout(const LayoutConfig& config) const;

    public:
    virtual ~IGraphStructure() = default;

    size_t size() const { return vertexCount; }
    bool empty() const { return vertexCount == 0; }

    virtual void addVertex(Timeline* timeline = nullptr) = 0;
    virtual void addEdge(size_t from, size_t to, int weight = 1, Timeline* timeline = nullptr) = 0;
    virtual void deleteEdge(size_t from, size_t to, Timeline* timeline = nullptr) = 0;
    virtual bool hasEdge(size_t from, size_t to) const = 0;

    virtual std::vector<size_t> getVertices() const = 0;
    virtual std::vector<Edge> getEdges() const = 0;

    virtual std::vector<size_t> getNeighbours(size_t vertex) const = 0;
    virtual std::vector<Edge> getEdgesFromVertex(size_t vertex) const = 0;

    virtual void initialize(const std::vector<Edge>& startingEdges, Timeline& timeline) = 0;

    void setLayoutConfig(const LayoutConfig& config) { layoutConfig = config; }
    const LayoutConfig& getLayoutConfig() const { return layoutConfig; }
    
    // Invalidate cached layout when graph structure changes
    void invalidateLayoutCache() const { layoutCacheValid = false; }

    void runDAGShortestPath(size_t startVertex, Timeline& timeline);
    void runDijkstra(size_t startVertex, Timeline& timeline);
    virtual void runAStar(size_t startVertex, size_t targetVertex, Timeline& timeline);
    virtual void runBellmanFord(size_t startVertex, Timeline& timeline);
    virtual void runFloydWarshall(Timeline& timeline);
    virtual void runJohnson(Timeline& timeline);
};

#endif // IGRAPHSTRUCTURE_H