#include "Model/DataStructure/AdjacencyMatrix.h"

AdjacencyMatrix::AdjacencyMatrix(bool directed) : isDirected(directed) { vertexCount = 0; }

void AdjacencyMatrix::resizeMatrix(int newSize) {
    matrix.resize(newSize);
    for (auto& row : matrix) {
        row.resize(newSize, 0);
    }
}

void AdjacencyMatrix::addEdge(int from, int to, int weight) {
    if (from >= vertexCount || to >= vertexCount) {
        resizeMatrix(std::max(from, to) + 1);
        vertexCount = matrix.size();
    }

    matrix[from][to] = weight;
    if (!isDirected) {
        matrix[to][from] = weight;
    }
}

void AdjacencyMatrix::deleteEdge(int from, int to) {
    if (from < vertexCount && to < vertexCount) {
        matrix[from][to] = 0;
        if (!isDirected) {
            matrix[to][from] = 0;
        }
    }
}

bool AdjacencyMatrix::hasEdge(int from, int to) const {
    if (from < vertexCount && to < vertexCount) {
        return matrix[from][to] != 0;
    }
    return false;
}

std::vector<int> AdjacencyMatrix::getVertices() const {
    std::vector<int> vertices;
    for (size_t i = 0; i < vertexCount; ++i) {
        vertices.push_back(i);
    }
    return vertices;
}

std::vector<Edge> AdjacencyMatrix::getEdges() const {
    std::vector<Edge> edges;
    for (size_t i = 0; i < vertexCount; ++i) {
        for (size_t j = 0; j < vertexCount; ++j) {
            if (matrix[i][j] != 0) {
                edges.emplace_back(i, j, matrix[i][j]);
            }
        }
    }
    return edges;
}

std::vector<int> AdjacencyMatrix::getNeighbours(int vertex) const {
    std::vector<int> neighbours;
    if (vertex < vertexCount) {
        for (size_t j = 0; j < vertexCount; ++j) {
            if (matrix[vertex][j] != 0) {
                neighbours.push_back(j);
            }
        }
    }
    return neighbours;
}

std::vector<Edge> AdjacencyMatrix::getEdgesFromVertex(int vertex) const {
    std::vector<Edge> edges;
    if (vertex < vertexCount) {
        for (size_t j = 0; j < vertexCount; ++j) {
            if (matrix[vertex][j] != 0) {
                edges.emplace_back(vertex, j, matrix[vertex][j]);
            }
        }
    }
    return edges;
}

void AdjacencyMatrix::initialize(const std::vector<int>& data, Timeline& timeline) {

}

void AdjacencyMatrix::insert(int value, Timeline& timeline) {

}

void AdjacencyMatrix::remove(int value, Timeline& timeline) {

}

void AdjacencyMatrix::search(int value, Timeline& timeline) {

}

void AdjacencyMatrix::clear(Timeline& timeline) {

}