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

    public:
    GridGraph(size_t r, size_t c);

    void clear(Timeline& timeline) override;
    StructureType getStructureType() const override;

    void initialize(size_t r, size_t c, Timeline& timeline);
    void initializeFromData(const std::vector<std::vector<int>>& startGrid, Timeline& timeline);
    void setCellState(size_t r, size_t c, CellState state, Timeline& timeline);
    CellState getCellState(size_t r, size_t c) const;

    void runBFSShortestPath(std::pair<size_t, size_t> start, std::pair<size_t, size_t> target, Timeline& timeline);
    void runAStar(std::pair<size_t, size_t> start, std::pair<size_t, size_t> target, Timeline& timeline);
};

#endif // GRIDGRAPH_H