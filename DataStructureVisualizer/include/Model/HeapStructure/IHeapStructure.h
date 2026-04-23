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

    void shiftUp(int i, Timeline& timeline);
    void fixNode(int i, Timeline& timeline);
    void heapify(int n, int idx, Timeline& timeline);

    virtual bool compare(int idx1, int idx2) = 0;

    public:
    void initialize(const std::vector<int>& data, Timeline& timeline);
    void insert(int value, Timeline& timeline);
    void search(int value, Timeline& timeline);
    void remove(int value, Timeline& timeline);
    void update(int oldValue, int newValue, Timeline& timeline);
    void clear(Timeline& timeline);
    
    void extractTop(Timeline& timeline);
    void peek(Timeline& timeline);
};

#endif
