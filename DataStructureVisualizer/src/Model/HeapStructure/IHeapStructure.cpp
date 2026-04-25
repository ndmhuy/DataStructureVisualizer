#include "Model/HeapStructure/IHeapStructure.h"

#include <algorithm>
#include <climits>
#include <string>
#include <utility>

// Index
int IHeapStructure::parent(int n, int i) {
    return (i > 0 && i < n) ? ((i - 1) / 2) : INT_MAX;
}

int IHeapStructure::left(int n, int i) {
    return (i >= 0 && i < n) ? (2 * i + 1) : INT_MAX;
}

int IHeapStructure::right(int n, int i) {
    return (i >= 0 && i < n) ? (2 * i + 2) : INT_MAX;
}

// Helpers
void IHeapStructure::shiftUp(int i, Timeline& timeline, bool isInsert) {
    int p = parent(static_cast<int>(heapArray.size()), i);

    int line3 = isInsert ? 3 : -1;
    int line4 = isInsert ? 4 : -1;
    int line5 = isInsert ? 5 : -1;

    while (i != 0 && p != INT_MAX) {
        timeline.addFrame(Frame(HeapPayload(heapArray, {static_cast<size_t>(i), static_cast<size_t>(p)}), line3,
            "Comparing node " + std::to_string(heapArray[i]) + " with its parent " + std::to_string(heapArray[p])));

        if (compare(i, p)) {
            timeline.addFrame(Frame(HeapPayload(heapArray, {static_cast<size_t>(i), static_cast<size_t>(p)}), line4,
                "Violation found! Swapping " + std::to_string(heapArray[i]) + " and " + std::to_string(heapArray[p])));

            std::swap(heapArray[i], heapArray[p]);
            i = p;
            p = parent(static_cast<int>(heapArray.size()), i);

            timeline.addFrame(Frame(HeapPayload(heapArray, {static_cast<size_t>(i)}), line5,
                "Node moved up to index " + std::to_string(i) + ". Checking again..."));
        } else {
            timeline.addFrame(Frame(HeapPayload(heapArray, {static_cast<size_t>(i), static_cast<size_t>(p)}), line3,
                "Node " + std::to_string(heapArray[i]) + " is in correct position relative to parent. Stop shifting up."));
            break;
        }
    }

    if (i == 0 && !heapArray.empty()) {
        timeline.addFrame(Frame(HeapPayload(heapArray, {0}), line3,
            "Node " + std::to_string(heapArray[0]) + " has reached the root of the heap."));
    }
}

void IHeapStructure::fixNode(int i, Timeline& timeline) {
    if (i < 0 || i >= static_cast<int>(heapArray.size())) {
        return;
    }

    int p = parent(static_cast<int>(heapArray.size()), i);
    if (p != INT_MAX && compare(i, p)) {
        shiftUp(i, timeline, false);
    } else {
        heapify(static_cast<int>(heapArray.size()), i, timeline);
    }
}

void IHeapStructure::heapify(int n, int i, Timeline& timeline) {
    int curr = i;
    int l = left(n, i);
    int r = right(n, i);

    std::vector<size_t> highlights = {static_cast<size_t>(i)};
    if (l < n) {
        highlights.push_back(static_cast<size_t>(l));
    }
    if (r < n) {
        highlights.push_back(static_cast<size_t>(r));
    }

    timeline.addFrame(Frame(HeapPayload(heapArray, highlights), -1,
        "Comparing node " + std::to_string(heapArray[i]) + " with its children"));

    if (l < n && compare(l, curr)) {
        curr = l;
    }
    if (r < n && compare(r, curr)) {
        curr = r;
    }

    if (curr != i) {
        timeline.addFrame(Frame(HeapPayload(heapArray, {static_cast<size_t>(i), static_cast<size_t>(curr)}), -1,
            "Violation found! Swapping " + std::to_string(heapArray[i]) + " and " + std::to_string(heapArray[curr])));

        std::swap(heapArray[i], heapArray[curr]);

        timeline.addFrame(Frame(HeapPayload(heapArray, {static_cast<size_t>(curr)}), -1,
            "Heapifying " + std::to_string(heapArray[curr])));

        heapify(n, curr, timeline);
    } else {
        timeline.addFrame(Frame(HeapPayload(heapArray, {}), -1,
            "Node " + std::to_string(heapArray[i]) + " is in correct position."));
    }
}

// Main functions
void IHeapStructure::initialize(const std::vector<int>& data, Timeline& timeline) {
    clear(timeline);

    heapArray = data;
    timeline.addFrame(Frame(HeapPayload(heapArray, {}), -1, "Initializing Heap from data..."));

    int heapSize = static_cast<int>(heapArray.size());
    for (int idx = heapSize / 2 - 1; idx >= 0; --idx) {
        heapify(heapSize, idx, timeline);
    }

    timeline.addFrame(Frame(HeapPayload(heapArray, {}), -1, "Initialization complete."));
}

void IHeapStructure::insert(int value, Timeline& timeline) {
    timeline.addFrame(Frame(HeapPayload(heapArray, {}), 0, "Starting inserting..."));

    heapArray.push_back(value);
    int lastIdx = static_cast<int>(heapArray.size()) - 1;
    timeline.addFrame(Frame(HeapPayload(heapArray, {static_cast<size_t>(lastIdx)}), 1,
        "Added " + std::to_string(value) + " to the end"));

    shiftUp(lastIdx, timeline, true);

    timeline.addFrame(Frame(HeapPayload(heapArray, {}), -1, "Inserting successfully!"));
}

void IHeapStructure::search(int value, Timeline& timeline) {
    timeline.addFrame(Frame(HeapPayload(heapArray, {}), 0, "Starting searching..."));

    for (int idx = 0; idx < static_cast<int>(heapArray.size()); ++idx) {
        timeline.addFrame(Frame(HeapPayload(heapArray, {static_cast<size_t>(idx)}), 1,
            "Checking index " + std::to_string(idx)));

        if (heapArray[idx] == value) {
            timeline.addFrame(Frame(HeapPayload(heapArray, {static_cast<size_t>(idx)}), 2,
                "Match found at index " + std::to_string(idx)));
            timeline.addFrame(Frame(HeapPayload(heapArray, {}), 3, "Searching successfully!"));
            return;
        }
    }

    timeline.addFrame(Frame(HeapPayload(heapArray, {}), 4, "Value not found."));
}

void IHeapStructure::remove(int value, Timeline& timeline) {
    timeline.addFrame(Frame(HeapPayload(heapArray, {}), 0, "Starting removing..."));

    int targetIdx = INT_MAX;
    for (int idx = 0; idx < static_cast<int>(heapArray.size()); ++idx) {
        if (heapArray[idx] == value) {
            targetIdx = idx;
            break;
        }
    }

    if (targetIdx == INT_MAX) {
        timeline.addFrame(Frame(HeapPayload(heapArray, {}), 2,
            "Value " + std::to_string(value) + " not found!"));
        return;
    }

    timeline.addFrame(Frame(HeapPayload(heapArray, {static_cast<size_t>(targetIdx), heapArray.size() - 1}), 3,
        "Swapping target with last element for removal"));

    std::swap(heapArray[targetIdx], heapArray.back());
    timeline.addFrame(Frame(HeapPayload(heapArray, {heapArray.size() - 1}), 4, "Removing the last element"));

    heapArray.pop_back();

    if (targetIdx < static_cast<int>(heapArray.size())) {
        timeline.addFrame(Frame(HeapPayload(heapArray, {static_cast<size_t>(targetIdx)}), 5,
            "Re-balancing at swap position"));
        fixNode(targetIdx, timeline);
    }

    timeline.addFrame(Frame(HeapPayload(heapArray, {}), -1, "Removing successfully!"));
}

void IHeapStructure::update(int oldValue, int newValue, Timeline& timeline) {
    timeline.addFrame(Frame(HeapPayload(heapArray, {}), 0, "Starting updating..."));

    int targetIdx = INT_MAX;
    for (int idx = 0; idx < static_cast<int>(heapArray.size()); ++idx) {
        if (heapArray[idx] == oldValue) {
            targetIdx = idx;
            break;
        }
    }

    if (targetIdx == INT_MAX) {
        timeline.addFrame(Frame(HeapPayload(heapArray, {}), 2, "Old value not found!"));
        return;
    }

    heapArray[targetIdx] = newValue;
    timeline.addFrame(Frame(HeapPayload(heapArray, {static_cast<size_t>(targetIdx)}), 3,
        "Value changed. Re-balancing..."));

    fixNode(targetIdx, timeline);

    timeline.addFrame(Frame(HeapPayload(heapArray, {}), 5, "Updating successfully!"));
}

void IHeapStructure::clear(Timeline& timeline) {
    timeline.addFrame(Frame(HeapPayload(heapArray, {}), -1, "Starting clearing..."));

    heapArray.clear();

    timeline.addFrame(Frame(HeapPayload(heapArray, {}), -1, "Clearing successfully!"));
}

// Extra functions
void IHeapStructure::extractTop(Timeline& timeline) {
    timeline.addFrame(Frame(HeapPayload(heapArray, {}), 0, "Starting extracting..."));

    if (heapArray.empty()) {
        timeline.addFrame(Frame(HeapPayload(heapArray, {}), 2, "Heap empty!"));
        return;
    }

    timeline.addFrame(Frame(HeapPayload(heapArray, {0, heapArray.size() - 1}), 3,
        "Extracting top. Swapping with end."));

    std::swap(heapArray[0], heapArray.back());
    timeline.addFrame(Frame(HeapPayload(heapArray, {}), 4, "Removing the end"));

    heapArray.pop_back();
    if (!heapArray.empty()) {
        timeline.addFrame(Frame(HeapPayload(heapArray, {0}), 5, "Re-heapifying from the top"));
        heapify(static_cast<int>(heapArray.size()), 0, timeline);
    }

    timeline.addFrame(Frame(HeapPayload(heapArray, {}), -1, "Extracting successfully!"));
}

void IHeapStructure::peek(Timeline& timeline) {
    timeline.addFrame(Frame(HeapPayload(heapArray, {}), 0, "Starting peeking..."));

    if (heapArray.empty()) {
        timeline.addFrame(Frame(HeapPayload(heapArray, {}), 4, "Heap was empty. Cannot peek anything!"));
        return;
    }

    timeline.addFrame(Frame(HeapPayload(heapArray, {0}), 3,
        "Top value found: " + std::to_string(heapArray[0]) + "."));
}
