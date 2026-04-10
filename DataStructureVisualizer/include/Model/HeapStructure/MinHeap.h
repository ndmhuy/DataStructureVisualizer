#ifndef MINHEAP_HPP
#define MINHEAP_HPP

#include "Model/HeapStructure/IHeapStructure.h"

class MinHeap : public IHeapStructure {
    protected:
        bool compare(int idx1, int idx2) override;
};

#endif