#include "Model/HeapStructure/MinHeap.h"

bool MinHeap::compare(size_t idx1, size_t idx2) {
    return heapArray[idx1] < heapArray[idx2];
}

StructureType MinHeap::getStructureType() const {
    return StructureType::MinHeap;
}