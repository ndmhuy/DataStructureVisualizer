#include "Model\HeapStructure\MaxHeap.h"

bool MaxHeap::compare(int idx1, int idx2) {
    return heapArray[idx1] > heapArray[idx2];
}

StructureType MaxHeap::getStructureType() const {
    return StructureType::MaxHeap;
}