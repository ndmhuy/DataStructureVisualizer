#ifndef IHEAPSTRUCTURE_H
#define IHEAPSTRUCTURE_H

#include "Model/IVisualizable.h"
#include <vector>

class IHeapStructure : public IVisualizable {
    protected:
    std::vector<int> heapArray;

    int parent(int n, int idx);
    int left(int n, int idx);
    int right(int n, int idx);
    void heapify(int n, int idx);

    virtual bool compare(int idx1, int idx2) = 0;

    public:
    void initialize(const std::vector<int>& data, Timeline& timeline);
    void insert(int value, Timeline& timeline);

    void extractTop(Timeline& timeline);
    void peek(Timeline& timeline);
};

#endif
