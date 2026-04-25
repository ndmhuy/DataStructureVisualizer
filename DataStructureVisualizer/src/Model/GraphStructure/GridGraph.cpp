#include "Model/GraphStructure/GridGraph.h"
#include <limits>
#include <queue>

GridGraph::GridGraph(size_t r, size_t c) : rows(r), cols(c) {
    grid.resize(rows, std::vector<int>(cols, static_cast<int>(CellState::Empty)));
}

void GridGraph::clear(Timeline& timeline) {
    for (size_t r = 0; r < rows; ++r) {
        for (size_t c = 0; c < cols; ++c) {
            grid[r][c] = static_cast<int>(CellState::Empty);
        }
    }

    timeline.addFrame(Frame(GridPayload(grid), 0, "Cleared grid"));
}

StructureType GridGraph::getStructureType() const {
    return StructureType::GridGraph;
}

void GridGraph::initialize(size_t r, size_t c, Timeline& timeline) {
    rows = r;
    cols = c;
    grid.clear();
    grid.assign(rows, std::vector<int>(cols, static_cast<int>(CellState::Empty)));
    timeline.addFrame(Frame(GridPayload(grid), 0, "Initialized grid with size " + std::to_string(rows) + "x" + std::to_string(cols)));
}

void GridGraph::setCellState(size_t r, size_t c, CellState state, Timeline& timeline) {
    if (r < rows && c < cols) {
        grid[r][c] = static_cast<int>(state);
        timeline.addFrame(Frame(GridPayload(grid, {r, c}), 0, "Set cell (" + std::to_string(r) + ", " + std::to_string(c) + ") to state " + std::to_string(static_cast<int>(state))));
    }
}

CellState GridGraph::getCellState(size_t r, size_t c) const {
    if (r < rows && c < cols) {
        return static_cast<CellState>(grid[r][c]);
    }
    return CellState::Empty;
}

void GridGraph::runBFSShortestPath(std::pair<size_t, size_t> start, std::pair<size_t, size_t> target, Timeline& timeline) {
    const int INF = std::numeric_limits<int>::max();

    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));
    std::vector<std::vector<std::pair<size_t, size_t>>> previous(rows, std::vector<std::pair<size_t, size_t>>(cols, {std::numeric_limits<size_t>::max(), std::numeric_limits<size_t>::max()}));
    std::vector<std::pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    std::queue<std::pair<size_t, size_t>> q;
    q.push(start);
    visited[start.first][start.second] = true;

    grid[start.first][start.second] = static_cast<int>(CellState::Start);
    grid[target.first][target.second] = static_cast<int>(CellState::Target);
    timeline.addFrame(Frame(GridPayload(grid, start), 0, "Starting BFS from (" + std::to_string(start.first) + ", " + std::to_string(start.second) + ")"));

    while (!q.empty()) {
        auto [r, c] = q.front();
        q.pop();
        timeline.addFrame(Frame(GridPayload(grid, {r, c}), 10, "Visiting cell (" + std::to_string(r) + ", " + std::to_string(c) + ")"));

        timeline.addFrame(Frame(GridPayload(grid, {r, c}), 11, "Check if cell (" + std::to_string(r) + ", " + std::to_string(c) + ") is the target"));
        if (std::make_pair(r, c) == target) {
            timeline.addFrame(Frame(GridPayload(grid, {r, c}), 12, "Reached target cell (" + std::to_string(r) + ", " + std::to_string(c) + ")"));
            
            std::pair<size_t, size_t> step = previous[r][c];
            while (step != start && step != std::make_pair(std::numeric_limits<size_t>::max(), std::numeric_limits<size_t>::max())) {
                grid[step.first][step.second] = static_cast<int>(CellState::Path);
                timeline.addFrame(Frame(GridPayload(grid, step), 12, "Marking cell (" + std::to_string(step.first) + ", " + std::to_string(step.second) + ") as part of the path"));
                step = previous[step.first][step.second];
            }
            return;
        }

        for (const auto& dir : directions) {
            size_t newRow = r + dir.first;
            size_t newCol = c + dir.second;

            timeline.addFrame(Frame(GridPayload(grid, {newRow, newCol}), 15, "Checking neighbor cell (" + std::to_string(newRow) + ", " + std::to_string(newCol) + ")"));

            if (newRow < rows && newCol < cols && !visited[newRow][newCol] && grid[newRow][newCol] != static_cast<int>(CellState::Wall)) {
                visited[newRow][newCol] = true;
                previous[newRow][newCol] = {r, c};
                q.push({newRow, newCol});
                grid[newRow][newCol] = static_cast<int>(CellState::Visited);
                timeline.addFrame(Frame(GridPayload(grid, {newRow, newCol}), 19, "Enqueued cell (" + std::to_string(newRow) + ", " + std::to_string(newCol) + ") for visiting"));
            }
        }
    }

    timeline.addFrame(Frame(GridPayload(grid), 20, "No path found from start to target."));
}