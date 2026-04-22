#include "Model/HeapStructure/IHeapStructure.h"
#include <utility>
#include <string>
#include <algorithm>
#include <climits>

// Index
int IHeapStructure::parent(int n, int i) {
    return (i > 0 && i < n) ? ((i-1)/2) : (INT_MAX);
}

int IHeapStructure::left(int n, int i) {
    return (i >= 0 && i < n) ? (2*i+1) : (INT_MAX);
}

int IHeapStructure::right(int n, int i) {
    return (i >= 0 && i < n) ? (2*i+2) : (INT_MAX);
}

// Helper
void IHeapStructure::shiftUp(int i, Timeline& timeline) {
    int p = parent(heapArray.size(), i);
    
    while (i != 0 && p != INT_MAX) {
        timeline.addFrame(Frame(heapArray, {(unsigned long long)i, (unsigned long long)p}, 1, "Comparing node " + std::to_string(heapArray[i]) + " with its parent " + std::to_string(heapArray[p])));
        
        if (compare(i, p)) {
            timeline.addFrame(Frame(heapArray, {(unsigned long long)i, (unsigned long long)p}, 2, "Violation found! Swapping " + std::to_string(heapArray[i]) + " and " + std::to_string(heapArray[p])));
            
            std::swap(heapArray[i], heapArray[p]);
            i = p;
            p = parent(heapArray.size(), i);
            
            timeline.addFrame(Frame(heapArray, {(unsigned long long)i}, 3, "Node moved up to index " + std::to_string(i) + ". Checking again..."));
        } else {
            timeline.addFrame(Frame(heapArray, {(unsigned long long)i, (unsigned long long)p}, 4, "Node " + std::to_string(heapArray[i]) + " is in correct position relative to parent. Stop shifting up."));

            break;
        }
    }
    
    if (i == 0) {
        timeline.addFrame(Frame(heapArray, {(unsigned long long)i}, 5, "Node " + std::to_string(heapArray[i]) + " has reached the root of the heap."));
    }
}

void IHeapStructure::fixNode(int i, Timeline& timeline) {
    if (i < 0 || i >= (int)heapArray.size()) 
        return;

    int p = parent(heapArray.size(), i);
    if (p != INT_MAX && compare(i, p)) {
        shiftUp(i, timeline);
    }
    else {
        heapify(heapArray.size(), i, timeline);
    }
}

void IHeapStructure::heapify(int n, int i, Timeline& timeline) {
    int curr = i, l = left(n, i), r = right(n, i);

    std::vector<unsigned long long> highlights;
    highlights.push_back((unsigned long long)i);

    if (l < n) 
        highlights.push_back((unsigned long long)l);

    if (r < n) 
        highlights.push_back((unsigned long long)r);

    timeline.addFrame(Frame(heapArray, highlights, 1, "Comparing node " + std::to_string(heapArray[i]) + " with its children"));

    if (l < n && compare(l, curr))
        curr = l;

    if (r < n && compare(r, curr))
        curr = r;

    if (curr != i) {
        timeline.addFrame(Frame(heapArray, {(unsigned long long)i, (unsigned long long)curr}, 2, "Violation found! Swapping " + std::to_string(heapArray[i]) + " and " + std::to_string(heapArray[curr])));
        std::swap(heapArray[i], heapArray[curr]);

        timeline.addFrame(Frame(heapArray, {(unsigned long long)curr}, 3, "Heapifying " + std::to_string(heapArray[curr])));
        heapify(n, curr, timeline);
    }
    else {
        timeline.addFrame(Frame(heapArray, {}, 4, "Node " + std::to_string(heapArray[i]) + " is in correct position."));
    }
}

// Main functions
void IHeapStructure::initialize(const std::vector<int>& data, Timeline& timeline) {
    clear(timeline);

    heapArray = data;
    timeline.addFrame(Frame(heapArray, {}, 0, "Initializing Heap from data..."));

    int heap_size = data.size();
    for (int idx = heap_size/2-1; idx >= 0; idx--)
        heapify(heap_size, idx, timeline);

    timeline.addFrame(Frame(heapArray, {}, 0, "Initialization complete."));
}

void IHeapStructure::insert(int value, Timeline& timeline) {
    timeline.addFrame(Frame(heapArray, {}, 1, "Starting inserting..."));

    heapArray.push_back(value);
    int lastIdx = heapArray.size() - 1;
    timeline.addFrame(Frame(heapArray, {(unsigned long long)lastIdx}, 2, "Added " + std::to_string(value) + " to the end"));
    
    shiftUp(lastIdx, timeline);

    timeline.addFrame(Frame(heapArray, {}, 1, "Inserting successfully!"));
}

void IHeapStructure::search(int value, Timeline& timeline) {
    timeline.addFrame(Frame(heapArray, {}, 1, "Starting searching..."));

    for (int idx = 0; idx < (int)heapArray.size(); idx++) {
        timeline.addFrame(Frame(heapArray, {(unsigned long long)idx}, 2, "Checking index " + std::to_string(idx)));

        if (heapArray[idx] == value) {
            timeline.addFrame(Frame(heapArray, {(unsigned long long)idx}, 3, "Match found at index " + std::to_string(idx)));
            timeline.addFrame(Frame(heapArray, {}, 4, "Searching successfully!"));

            return;
        }
    }

    timeline.addFrame(Frame(heapArray, {}, 5, "Value not found."));
}

void IHeapStructure::remove(int value, Timeline& timeline) {
    timeline.addFrame(Frame(heapArray, {}, 1, "Starting removing..."));

    int targetIdx = INT_MAX;
    for (int idx = 0; idx < (int)heapArray.size(); idx++) {
        if (heapArray[idx] == value) {
            targetIdx = idx;
            break;
        }
    }
    
    if (targetIdx == INT_MAX) {
        timeline.addFrame(Frame(heapArray, {}, 2, "Value " + std::to_string(value) + " not found!"));

        return;
    }

    timeline.addFrame(Frame(heapArray, {(unsigned long long)targetIdx, (unsigned long long)heapArray.size()-1}, 3, "Swapping target with last element for removal"));

    std::swap(heapArray[targetIdx], heapArray.back());
    timeline.addFrame(Frame(heapArray, {(unsigned long long)heapArray.size()-1}, 4, "Removing the last element"));

    heapArray.pop_back();

    if (targetIdx < (int)heapArray.size()) {
        timeline.addFrame(Frame(heapArray, {(unsigned long long)targetIdx}, 5, "Re-balancing at swap position"));
        fixNode(targetIdx, timeline);
    }

    timeline.addFrame(Frame(heapArray, {}, 6, "Removing successfully!"));
}

void IHeapStructure::update(int oldValue, int newValue, Timeline& timeline) {
    timeline.addFrame(Frame(heapArray, {}, 1, "Starting updating..."));

    int targetIdx = INT_MAX;
    for (int idx = 0; idx < (int)heapArray.size(); idx++) {
        if (heapArray[idx] == oldValue) {
            targetIdx = idx; 
            break;
        }
    }
    
    if (targetIdx != INT_MAX) {
        heapArray[targetIdx] = newValue;
        timeline.addFrame(Frame(heapArray, {(unsigned long long)targetIdx}, 2, "Value changed. Re-balancing..."));

        fixNode(targetIdx, timeline);
        timeline.addFrame(Frame(heapArray, {}, 3, "Updating successfully!"));
    }
    else {
        timeline.addFrame(Frame(heapArray, {}, 4, "Old value not found!"));
    }
}

void IHeapStructure::clear(Timeline& timeline) {
    timeline.addFrame(Frame(heapArray, {}, 1, "Starting clearing..."));

    heapArray.clear();

    timeline.addFrame(Frame(heapArray, {}, 2, "Clearing successfully!"));
}

// Extra functions
void IHeapStructure::extractTop(Timeline& timeline) {
    timeline.addFrame(Frame(heapArray, {}, 1, "Starting extracting..."));

    if (heapArray.empty()) {
        timeline.addFrame(Frame(heapArray, {}, 2, "Heap empty!"));
        return;
    }
        
    timeline.addFrame(Frame(heapArray, {0, (unsigned long long)heapArray.size()-1, 0}, 3, "Extracting top. Swapping with end."));

    std::swap(heapArray[0], heapArray.back());
    timeline.addFrame(Frame(heapArray, {}, 4, "Removing the end"));

    heapArray.pop_back();
    if (!heapArray.empty()) {
        timeline.addFrame(Frame(heapArray, {0}, 5, "Re-heapifying from the top"));

        heapify(heapArray.size(), 0, timeline);
    }

    timeline.addFrame(Frame(heapArray, {}, 6, "Extracting successfully!"));
}

void IHeapStructure::peek(Timeline& timeline) {
    timeline.addFrame(Frame(heapArray, {}, 1, "Starting peeking..."));
    
    if (heapArray.empty()) {
        timeline.addFrame(Frame(heapArray, {}, 2, "Heap was empty. Cannot peek anything!"));

        return;
    }

    timeline.addFrame(Frame(heapArray, {0}, 3, "Top value found: " + std::to_string(heapArray[0]) + "."));
}