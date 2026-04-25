#ifndef MAXHEAP_HPP
#define MAXHEAP_HPP

#include "Model/HeapStructure/IHeapStructure.h"

class MaxHeap : public IHeapStructure {
    protected:
        bool compare(size_t idx1, size_t idx2) override;

    public:
        StructureType getStructureType() const override;
};

#endif