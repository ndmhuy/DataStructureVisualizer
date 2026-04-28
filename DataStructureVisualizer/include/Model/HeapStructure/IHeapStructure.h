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

    // Compares two heap positions according to the heap ordering.
    virtual bool compare(size_t idx1, size_t idx2) = 0;

    public:
    virtual ~IHeapStructure() = default;
    
    // Initializes the heap from raw data.
    void initialize(const std::vector<int>& data, Timeline& timeline);
    // Inserts a value into the heap.
    void insert(int value, Timeline& timeline);
    // Searches for a value in the heap.
    void search(int value, Timeline& timeline);
    // Removes a value from the heap.
    void remove(int value, Timeline& timeline);
    // Updates one value to another.
    void update(int oldValue, int newValue, Timeline& timeline);
    // Clears the heap.
    void clear(Timeline& timeline);
    
    // Extracts the top element from the heap.
    void extractTop(Timeline& timeline);
    // Peeks at the top element of the heap.
    void peek(Timeline& timeline);
};

#endif
