#ifndef EDGE_H
#define EDGE_H

#include <cstddef>

struct Edge {
    size_t from;
    size_t to;
    int weight;

    // Creates an edge from one vertex to another with an optional weight.
    Edge(size_t f, size_t t, int w = 1) : from(f), to(t), weight(w) {}
};

#endif // EDGE_H