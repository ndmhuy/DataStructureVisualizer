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
    int rows;
    int cols;

    public:
    GridGraph(int r, int c);

    void clear(Timeline& timeline) override;
    StructureType getStructureType() const override;

    void initialize(int r, int c, Timeline& timeline);
};

#endif // GRIDGRAPH_H