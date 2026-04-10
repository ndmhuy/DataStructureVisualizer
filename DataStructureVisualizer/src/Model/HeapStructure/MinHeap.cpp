#include "Model\HeapStructure\MinHeap.h"

bool MinHeap::compare(int idx1, int idx2) {
    return heapArray[idx1] < heapArray[idx2];
}

StructureType MinHeap::getStructureType() const {
    return StructureType::MinHeap;
}