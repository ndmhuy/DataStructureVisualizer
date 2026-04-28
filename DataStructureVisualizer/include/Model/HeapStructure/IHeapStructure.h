#ifndef IHEAPSTRUCTURE_H
#define IHEAPSTRUCTURE_H

#include "Model/IVisualizable.h"
#include "Utilities/LayoutConfig.h"

#include <vector>
#include <cstddef>

class IHeapStructure : public IVisualizable {
    protected:
    LayoutConfig layoutConfig; 

    IHeapStructure(const LayoutConfig& config = LayoutConfig()) 
        : layoutConfig(config) {}

    std::vector<int> heapArray;

    size_t parent(size_t n, size_t idx);
    size_t left(size_t n, size_t idx);
    size_t right(size_t n, size_t idx);

    void shiftUp(size_t i, Timeline& timeline);
    void fixNode(size_t i, Timeline& timeline);
    void heapify(size_t n, size_t idx, Timeline* timeline);

    virtual bool compare(size_t idx1, size_t idx2) = 0;

    public:
    virtual ~IHeapStructure() = default;
    
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
