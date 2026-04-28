#ifndef MINHEAP_HPP
#define MINHEAP_HPP

#include "Model/HeapStructure/IHeapStructure.h"

class MinHeap : public IHeapStructure {
    protected:
        // Returns true when the first index should be above the second in a min-heap.
        bool compare(size_t idx1, size_t idx2) override;
        
    public:
        // Creates a min-heap with the selected layout configuration.
        MinHeap(const LayoutConfig& config = LayoutConfig());

        // Returns the structure type for UI routing.
        StructureType getStructureType() const override;
};

#endif