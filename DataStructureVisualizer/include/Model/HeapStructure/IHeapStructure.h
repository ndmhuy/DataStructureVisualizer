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
    void buildHeap(int n);

    virtual void initialize(const std::vector<int>& data, Timeline& timeline);
    virtual void insert(int value, Timeline& timeline);

    virtual void extractTop(Timeline& timeline);
    virtual void peek(Timeline& timeline);
};

#endif
