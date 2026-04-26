#ifndef MINHEAP_HPP
#define MINHEAP_HPP

#include "Model/HeapStructure/IHeapStructure.h"

class MinHeap : public IHeapStructure {
    protected:
        bool compare(size_t idx1, size_t idx2) override;
        
    public:
        MinHeap(const LayoutConfig& config = LayoutConfig());

        StructureType getStructureType() const override;
};

#endif