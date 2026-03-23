#include "Model/DataStructure/Heap.h"
#include <vector>
#include <utility>
#include <string>

std::vector<int> Heap::toVector() const {
    return heapArray;
}

int Heap::parent(int n, int i) {
    return (i >= 0 && i < n) ? ((i-1)/2) : (-1);
}

int Heap::left(int n, int i) {
    return (i >= 0 && i < n) ? (2*i+1) : (-1);
}

int Heap::right(int n, int i) {
    return (i >= 0 && i < n) ? (2*i+2) : (-1);
}

void Heap::maxHeapify(int n, int i) {
    int curr = i, l = left(n, i), r = right(n, i);

    if (l < n && heapArray[l] > heapArray[curr])
        curr = l;

    if (r < n && heapArray[r] > heapArray[curr])
        curr = r;

    if (curr != i) {
        // This should include the Frame
        std::swap(heapArray[i], heapArray[curr]);
        maxHeapify(n, curr);
    }
}

void Heap::buildMaxHeap(int n) {
    for (int i = n/2-1; i >= 0; i--)
        maxHeapify(n, i);
}

void Heap::deleteArray() {
    heapArray.clear();
}

Heap::Heap() : heapArray() {}

Heap::~Heap() {
    deleteArray();
}

void Heap::initialize(const std::vector<int>& data, Timeline& timeline) {
    clear(timeline);
    
    timeline.addFrame(Frame(toVector(), {}, 0, "Initializing Heap from data..."));

    heapArray = data;
    buildMaxHeap(data.size());

    timeline.addFrame(Frame(toVector(), {}, 0, "Initialization complete."));
}

void Heap::insert(int value, Timeline& timeline) {
    timeline.addFrame(Frame(toVector(), {}, 1, "Starting insertion of " + std::to_string(value)));

    if (heapArray.empty()) {
        heapArray.push_back(value);
        timeline.addFrame(Frame(toVector(), {}, 2, "Heap was empty. Inserted at top."));
        return;
    }

    heapArray.push_back(value);
    int heap_size = heapArray.size();
    timeline.addFrame(Frame(toVector(), {heap_size-1}, 3, "Added at the end"));
    
    int idx = heap_size-1;
    int currParent = parent(heap_size, heap_size-1);

    while (idx > 0 && heapArray[idx] >= heapArray[currParent]) {
        timeline.addFrame(Frame(toVector(), {idx, currParent}, 4, "Comparing parent " + std::to_string(heapArray[currParent]) + " and child " + std::to_string(heapArray[idx])));
        timeline.addFrame(Frame(toVector(), {currParent}, 5, "Heapifying node " + std::to_string(heapArray[currParent])));
        timeline.addFrame(Frame(toVector(), {idx, currParent}, 6, "Swapping"));
        maxHeapify(heap_size, currParent);
        idx = currParent;
        currParent = parent(heap_size, currParent);
    }

    // This should include the highlighting
    timeline.addFrame(Frame(toVector(), {}, 7, "Successfully inserted " + std::to_string(value) + " in the heap"));
}

void Heap::remove(int value, Timeline& timeline) {
    timeline.addFrame(Frame(toVector(), {}, 1, "Attempting to remove " + std::to_string(value)));

    if (heapArray.empty()) {
        timeline.addFrame(Frame(toVector(), {}, 2, "Heap is empty. Nothing to remove."));
        return;
    }

    int heap_size = heapArray.size();

    for (int idx = 0; idx < heap_size; ++idx) {
        if (heapArray[idx] == value) {
            timeline.addFrame(Frame(toVector(), {idx, heap_size-1}, 3, "Value found. Replacing it with the last element"));
            heapArray[idx] = heapArray.back();
            timeline.addFrame(Frame(toVector(), {heap_size-1}, 4, "Removing the last element"));
            heapArray.pop_back();
            timeline.addFrame(Frame(toVector(), {idx}, 5, "Heapifying"));
            maxHeapify(heap_size-1, idx);
            return;
        }
    } 

    timeline.addFrame(Frame(toVector(), {}, 6, "Value " + std::to_string(value) + " was not found in the heap."));
}

void Heap::search(int value, Timeline& timeline) {
    std::vector<int> currentState = toVector();
    timeline.addFrame(Frame(currentState, {}, 1, "Searching for " + std::to_string(value)));

    for (int idx = 0; idx < heapArray.size(); ++idx) {
        timeline.addFrame(Frame(currentState, {idx}, 2, "Comparing with node value " + std::to_string(heapArray[idx])));
        if (heapArray[idx] == value) {
            timeline.addFrame(Frame(currentState, {idx}, 3, "Value found at index " + std::to_string(idx)));
            return;
        }
    }
    timeline.addFrame(Frame(currentState, {}, 4, "Value " + std::to_string(value) + " was not found in the list."));
}

void Heap::clear(Timeline& timeline) {
    timeline.addFrame(Frame(toVector(), {}, 1, "Clearing the entire heap..."));

    deleteArray();

    timeline.addFrame(Frame(toVector(), {}, 2, "Heap cleared successfully."));
}