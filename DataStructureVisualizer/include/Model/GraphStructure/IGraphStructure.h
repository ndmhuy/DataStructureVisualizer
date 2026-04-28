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

    // Builds a graph payload with optional highlighted vertices, edges, and custom positions.
    GraphPayload makeGraphPayload(const std::vector<size_t>& highlightedVertices = {}, const std::vector<Edge>& highlightedEdges = {}, const std::vector<Position>& customPositions = {}) const;

    // Computes the heuristic distance between two vertices using the provided positions.
    int heuristic(size_t vertex, size_t target, const std::vector<Position>& positions);

    // Generates a force-directed layout for the current graph.
    std::vector<Position> generatePhysicsBasedLayout(const LayoutConfig& config) const;

    public:
    virtual ~IGraphStructure() = default;

    // Returns the number of vertices in the structure.
    size_t size() const { return vertexCount; }
    // Returns true when the structure has no vertices.
    bool empty() const { return vertexCount == 0; }

    // Adds a vertex to the structure and optionally logs it to the timeline.
    virtual void addVertex(Timeline* timeline = nullptr) = 0;
    // Adds an edge to the structure and optionally logs it to the timeline.
    virtual void addEdge(size_t from, size_t to, int weight = 1, Timeline* timeline = nullptr) = 0;
    // Removes the vertex with the largest id and optionally logs it to the timeline.
    virtual void removeVertex(Timeline* timeline = nullptr) = 0;
    // Deletes an edge from the structure and optionally logs it to the timeline.
    virtual void deleteEdge(size_t from, size_t to, Timeline* timeline = nullptr) = 0;
    // Returns whether an edge exists between the given vertices.
    virtual bool hasEdge(size_t from, size_t to) const = 0;

    // Returns all vertex ids currently in the structure.
    virtual std::vector<size_t> getVertices() const = 0;
    // Returns all edges currently in the structure.
    virtual std::vector<Edge> getEdges() const = 0;

    // Returns the neighbours of a vertex.
    virtual std::vector<size_t> getNeighbours(size_t vertex) const = 0;
    // Returns all outgoing edges from a vertex.
    virtual std::vector<Edge> getEdgesFromVertex(size_t vertex) const = 0;

    // Initializes the structure from a prebuilt edge list.
    virtual void initialize(const std::vector<Edge>& startingEdges, Timeline& timeline) = 0;

    // Updates the layout configuration used for rendering.
    void setLayoutConfig(const LayoutConfig& config) { layoutConfig = config; }
    // Returns the current layout configuration.
    const LayoutConfig& getLayoutConfig() const { return layoutConfig; }
    
    // Invalidate cached layout when graph structure changes
    // Marks the cached layout as stale.
    void invalidateLayoutCache() const { layoutCacheValid = false; }

    // Runs DAG shortest path and logs the steps.
    void runDAGShortestPath(size_t startVertex, Timeline& timeline);
    // Runs Dijkstra's algorithm and logs the steps.
    void runDijkstra(size_t startVertex, Timeline& timeline);
    // Runs A* and logs the steps.
    virtual void runAStar(size_t startVertex, size_t targetVertex, Timeline& timeline);
    // Runs Bellman-Ford and logs the steps.
    virtual void runBellmanFord(size_t startVertex, Timeline& timeline);
    // Runs Floyd-Warshall and logs the steps.
    virtual void runFloydWarshall(Timeline& timeline);
    // Runs Johnson's algorithm and logs the steps.
    virtual void runJohnson(Timeline& timeline);
};

#endif // IGRAPHSTRUCTURE_H