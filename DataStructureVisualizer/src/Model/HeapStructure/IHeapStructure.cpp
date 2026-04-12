#include "Model/HeapStructure/IHeapStructure.h"
#include <utility>
#include <string>

int IHeapStructure::parent(int n, int i) {
    return (i > 0 && i < n) ? ((i-1)/2) : (-1);
}

int IHeapStructure::left(int n, int i) {
    return (i >= 0 && i < n) ? (2*i+1) : (-1);
}

int IHeapStructure::right(int n, int i) {
    return (i >= 0 && i < n) ? (2*i+2) : (-1);
}

void IHeapStructure::heapify(int n, int i, Timeline& timeline) {
    int curr = i, l = left(n, i), r = right(n, i);

    std::vector<unsigned long long> highlights;
    highlights.push_back((unsigned long long)i);
    if (l < n) 
        highlights.push_back((unsigned long long)l);
    if (r < n) 
        highlights.push_back((unsigned long long)r);
    timeline.addFrame(Frame(heapArray, highlights, 0, "Comparing node " + std::to_string(heapArray[i]) + " with its children"));

    if (l < n && compare(l, curr))
        curr = l;

    if (r < n && compare(r, curr))
        curr = r;

    if (curr != i) {
        timeline.addFrame(Frame(heapArray, {(unsigned long long)i, (unsigned long long)curr}, 1, "Violation found! Swapping " + std::to_string(heapArray[i]) + " and " + std::to_string(heapArray[curr])));
        std::swap(heapArray[i], heapArray[curr]);

        timeline.addFrame(Frame(heapArray, {(unsigned long long)curr}, 2, "Heapifying " + std::to_string(heapArray[curr])));
        heapify(n, curr, timeline);
    }
    else {
        timeline.addFrame(Frame(heapArray, {}, 3, "Node " + std::to_string(heapArray[i]) + " is in correct position."));
    }
}

void IHeapStructure::initialize(const std::vector<int>& data, Timeline& timeline) {
    clear(timeline);
    
    timeline.addFrame(Frame(heapArray, {}, 0, "Initializing Heap from data..."));

    heapArray = data;
    int heap_size = data.size();
    for (int idx = heap_size/2-1; idx >= 0; idx--)
        heapify(heap_size, idx, timeline);

    timeline.addFrame(Frame(heapArray, {}, 0, "Initialization complete."));
}

void IHeapStructure::insert(int value, Timeline& timeline) {
    timeline.addFrame(Frame(heapArray, {}, 1, "Starting insertion of " + std::to_string(value)));

    if (heapArray.empty()) {
        heapArray.push_back(value);
        timeline.addFrame(Frame(heapArray, {}, 2, "Heap was empty. Inserted at top."));
        return;
    }

    heapArray.push_back(value);
    int heap_size = heapArray.size();
    timeline.addFrame(Frame(heapArray, {(unsigned long long)heap_size-1}, 3, "Added at the end"));
    
    unsigned long long idx = heap_size-1;
    unsigned long long currParent = parent(heap_size, heap_size-1);

    while (idx > 0 && compare(idx, currParent)) {
        timeline.addFrame(Frame(heapArray, {(unsigned long long)idx, (unsigned long long)currParent}, 4, 
            "Comparing parent " + std::to_string(heapArray[currParent]) + 
            " and child " + std::to_string(heapArray[idx])));
            
        timeline.addFrame(Frame(heapArray, {(unsigned long long)idx, (unsigned long long)currParent}, 5, 
            "Child should precede parent, swapping..."));

        std::swap(heapArray[idx], heapArray[currParent]);
        
        timeline.addFrame(Frame(heapArray, {(unsigned long long)idx, (unsigned long long)currParent}, 6, "Swapped"));

        idx = currParent;
        if (parent(heap_size, idx) >= 0) 
            currParent = parent(heap_size, idx);
    }

    timeline.addFrame(Frame(heapArray, {idx}, 7, "Successfully inserted " + std::to_string(value) + " in the heap"));
}

void IHeapStructure::extractTop(Timeline& timeline) {
    timeline.addFrame(Frame(heapArray, {}, 1, "Starting extracting"));

    if (heapArray.empty()) {
        timeline.addFrame(Frame(heapArray, {0}, 2, "Heap was empty. Cannot extract."));
        return;
    }

    int extract_value = heapArray[0];
    timeline.addFrame(Frame(heapArray, {0}, 3, "Top value found: " + std::to_string(extract_value)));
    timeline.addFrame(Frame(heapArray, {0, (unsigned long long)heapArray.size()-1}, 4, "Swapping the top with the end"));
    std::swap(heapArray[0], heapArray[heapArray.size()-1]);
    timeline.addFrame(Frame(heapArray, {}, 5, "Removing the end"));
    heapArray.pop_back();

    if (!heapArray.empty()) {
        timeline.addFrame(Frame(heapArray, {0}, 6, "Heapifying the top"));
        heapify(heapArray.size(), 0, timeline);
    }

    timeline.addFrame(Frame(heapArray, {}, 7, "Successfully extracted " + std::to_string(extract_value) + " in the heap"));
}

void IHeapStructure::peek(Timeline& timeline) {
    timeline.addFrame(Frame(heapArray, {}, 1, "Starting peeking"));
    
    if (heapArray.empty()) {
        timeline.addFrame(Frame(heapArray, {}, 2, "Heap was empty. Cannot peek anything"));

        return;
    }

    timeline.addFrame(Frame(heapArray, {0}, 3, "Top value found: " + std::to_string(heapArray[0])));
}

void IHeapStructure::clear(Timeline& timeline) {
    timeline.addFrame(Frame(heapArray, {}, 1, "Starting clearing"));

    heapArray.clear();

    timeline.addFrame(Frame(heapArray, {}, 2, "Clearing successfully"));
}