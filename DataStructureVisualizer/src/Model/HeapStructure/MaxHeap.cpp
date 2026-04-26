#include "Model/HeapStructure/MaxHeap.h"

MaxHeap::MaxHeap(const LayoutConfig& config) : IHeapStructure(config) {}

bool MaxHeap::compare(size_t idx1, size_t idx2) {
    return heapArray[idx1] > heapArray[idx2];
}

StructureType MaxHeap::getStructureType() const {
    return StructureType::MaxHeap;
}