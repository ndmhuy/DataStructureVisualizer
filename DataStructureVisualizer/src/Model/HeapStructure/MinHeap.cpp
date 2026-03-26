#include "Model\HeapStructure\MinHeap.h"

bool MaxHeap::compare(int idx1, int idx2) {
    return heapArray[idx1] < heapArray[idx2];
}