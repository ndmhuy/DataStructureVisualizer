#ifndef GRIDGRAPH_H
#define GRIDGRAPH_H

#include "Model/IVisualizable.h"

enum class CellState {
    Empty = 0,
    Wall = 1,
    Start = 2,
    Target = 3,
    Visited = 4,
    Path = 5
};

class GridGraph : public IVisualizable {
    private:
    std::vector<std::vector<int>> grid; // 2D grid representation
    size_t rows;
    size_t cols;

    // Returns whether a cell is inside the grid bounds.
    bool isValidCell(size_t r, size_t c) const {
        return r < rows && c < cols;
    }
    // Clears algorithm-specific visitation and path state.
    void resetAlgorithmStates(Timeline& timeline);

    public:
    // Creates a grid graph with the requested dimensions.
    GridGraph(size_t r, size_t c);

    // Clears the grid contents.
    void clear(Timeline& timeline) override;
    // Returns the structure type for UI routing.
    StructureType getStructureType() const override;

    // Initializes the grid with the requested size.
    void initialize(size_t r, size_t c, Timeline& timeline);
    // Loads a grid from prebuilt cell data.
    void initializeFromData(const std::vector<std::vector<int>>& startGrid, Timeline& timeline);
    // Updates the state of a single grid cell.
    void setCellState(size_t r, size_t c, CellState state, Timeline& timeline);
    // Returns the current state of a grid cell.
    CellState getCellState(size_t r, size_t c) const;

    // Runs BFS shortest path on the grid.
    void runBFSShortestPath(std::pair<size_t, size_t> start, std::pair<size_t, size_t> target, Timeline& timeline);
    // Runs A* shortest path on the grid.
    void runAStar(std::pair<size_t, size_t> start, std::pair<size_t, size_t> target, Timeline& timeline);
};

#endif // GRIDGRAPH_H