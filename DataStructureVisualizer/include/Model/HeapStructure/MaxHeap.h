#ifndef MAXHEAP_HPP
#define MAXHEAP_HPP

#include "Model/HeapStructure/IHeapStructure.h"

class MaxHeap : public IHeapStructure {
    protected:
        // Returns true when the first index should be above the second in a max-heap.
        bool compare(size_t idx1, size_t idx2) override;

    public:
        // Creates a max-heap with the selected layout configuration.
        MaxHeap(const LayoutConfig& config = LayoutConfig());
        
        // Returns the structure type for UI routing.
        StructureType getStructureType() const override;
};

#endif