#ifndef MAXHEAP_HPP
#define MAXHEAP_HPP

#include "Model/HeapStructure/IHeapStructure.h"

class MaxHeap : public IHeapStructure {
    protected:
        bool compare(int idx1, int idx2) override;

    public:
        StructureType getStructureType() const override;
};

#endif