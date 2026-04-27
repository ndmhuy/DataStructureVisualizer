#include "Model/HeapStructure/IHeapStructure.h"
#include "Utilities/GlobalConstant.h"

#include <algorithm>
#include <string>
#include <utility>

// Index
size_t IHeapStructure::parent(size_t n, size_t i) {
    return (i > 0 && i < n) ? ((i - 1) / 2) : INVALID_INDEX;
}

size_t IHeapStructure::left(size_t n, size_t i) {
    size_t res = 2 * i + 1;
    return (res < n) ? res : INVALID_INDEX;
}

size_t IHeapStructure::right(size_t n, size_t i) {
    size_t res = 2 * i + 2;
    return (res < n) ? res : INVALID_INDEX;
}

// Helpers
void IHeapStructure::shiftUp(size_t i, Timeline& timeline) {
    size_t p = parent(heapArray.size(), i);

    while (i != 0 && p != INVALID_INDEX) {
        timeline.addFrame(Frame(HeapPayload(heapArray, {i, p}), 1,
            "Comparing node " + std::to_string(heapArray[i]) + " with its parent " + std::to_string(heapArray[p])));

        if (compare(i, p)) {
            timeline.addFrame(Frame(HeapPayload(heapArray, {i, p}), 2,
                "Violation found! Swapping " + std::to_string(heapArray[i]) + " and " + std::to_string(heapArray[p])));

            std::swap(heapArray[i], heapArray[p]);
            i = p;
            p = parent(heapArray.size(), i);

            timeline.addFrame(Frame(HeapPayload(heapArray, {i}), 3,
                "Node moved up to index " + std::to_string(i) + ". Checking again..."));
        }
        else {
            timeline.addFrame(Frame(HeapPayload(heapArray, {i, p}), 4,
                "Node " + std::to_string(heapArray[i]) + " is in correct position relative to parent. Stop shifting up."));

            break;
        }
    }

    if (i == 0 && !heapArray.empty()) {
        timeline.addFrame(Frame(HeapPayload(heapArray, {0}), 5,
            "Node " + std::to_string(heapArray[0]) + " has reached the root of the heap."));
    }
}

void IHeapStructure::fixNode(size_t i, Timeline& timeline) {
    if (i < 0 || i >= heapArray.size()) {
        return;
    }

    size_t p = parent(heapArray.size(), i);
    if (p != INVALID_INDEX && compare(i, p)) {
        shiftUp(i, timeline);
    }
    else {
        heapify(heapArray.size(), i, &timeline);
    }
}

void IHeapStructure::heapify(size_t n, size_t i, Timeline* timeline) {
    size_t curr = i;
    size_t l = left(n, i);
    size_t r = right(n, i);

    std::vector<size_t> highlights = {i};
    if (l != INVALID_INDEX) {
        highlights.push_back(l);
    }
    if (r != INVALID_INDEX) {
        highlights.push_back(r);
    }

    if (timeline)
    timeline->addFrame(Frame(HeapPayload(heapArray, highlights), 1,
        "Comparing node " + std::to_string(heapArray[i]) + " with its children"));

    if (l != INVALID_INDEX && compare(l, curr)) {
        curr = l;
    }
    if (r != INVALID_INDEX && compare(r, curr)) {
        curr = r;
    }

    if (curr != i) {
        if (timeline)
        timeline->addFrame(Frame(HeapPayload(heapArray, {i, curr}), 2,
            "Violation found! Swapping " + std::to_string(heapArray[i]) + " and " + std::to_string(heapArray[curr])));

        std::swap(heapArray[i], heapArray[curr]);

        if (timeline)
        timeline->addFrame(Frame(HeapPayload(heapArray, {curr}), 3,
            "Heapifying " + std::to_string(heapArray[curr])));

        heapify(n, curr, timeline);
    }
    else {
        if (timeline)
        timeline->addFrame(Frame(HeapPayload(heapArray, {}), 4,
            "Node " + std::to_string(heapArray[i]) + " is in correct position."));
    }
}

// Main functions
void IHeapStructure::initialize(const std::vector<int>& data, Timeline& timeline) {
    clear(timeline);

    heapArray = data;
    timeline.addFrame(Frame(HeapPayload(heapArray, {}), 0, "Initializing Heap from data..."));

    size_t heapSize = heapArray.size();
    if (heapSize > 1) {
        for (size_t idx = heapSize / 2; idx > 0; --idx) {
            heapify(heapSize, idx - 1, nullptr);
        }
    }

    timeline.addFrame(Frame(HeapPayload(heapArray, {}), 0, "Initialization complete."));
}

void IHeapStructure::insert(int value, Timeline& timeline) {
    timeline.addFrame(Frame(HeapPayload(heapArray, {}), 1, "Starting inserting..."));

    heapArray.push_back(value);
    size_t lastIdx = heapArray.size() - 1;
    timeline.addFrame(Frame(HeapPayload(heapArray, {lastIdx}), 2,
        "Added " + std::to_string(value) + " to the end"));

    shiftUp(lastIdx, timeline);

    timeline.addFrame(Frame(HeapPayload(heapArray, {}), 3, "Inserting successfully!"));
}

void IHeapStructure::search(int value, Timeline& timeline) {
    timeline.addFrame(Frame(HeapPayload(heapArray, {}), 1, "Starting searching..."));

    for (size_t idx = 0; idx < heapArray.size(); ++idx) {
        timeline.addFrame(Frame(HeapPayload(heapArray, {idx}), 2,
            "Checking index " + std::to_string(idx)));

        if (heapArray[idx] == value) {
            timeline.addFrame(Frame(HeapPayload(heapArray, {idx}), 3,
                "Match found at index " + std::to_string(idx)));
            timeline.addFrame(Frame(HeapPayload(heapArray, {}), 4, "Searching successfully!"));
            return;
        }
    }

    timeline.addFrame(Frame(HeapPayload(heapArray, {}), 5, "Value not found."));
}

void IHeapStructure::remove(int value, Timeline& timeline) {
    timeline.addFrame(Frame(HeapPayload(heapArray, {}), 1, "Starting removing..."));

    size_t targetIdx = INVALID_INDEX;
    for (size_t idx = 0; idx < heapArray.size(); ++idx) {
        if (heapArray[idx] == value) {
            targetIdx = idx;
            break;
        }
    }

    if (targetIdx == INVALID_INDEX) {
        timeline.addFrame(Frame(HeapPayload(heapArray, {}), 2,
            "Value " + std::to_string(value) + " not found!"));
        return;
    }

    size_t lastIdx = heapArray.size() - 1;
    timeline.addFrame(Frame(HeapPayload(heapArray, {targetIdx, lastIdx}), 3,
        "Swapping target with last element for removal"));

    std::swap(heapArray[targetIdx], heapArray.back());
    timeline.addFrame(Frame(HeapPayload(heapArray, {lastIdx}), 4, "Removing the last element"));

    heapArray.pop_back();

    if (targetIdx < heapArray.size()) {
        timeline.addFrame(Frame(HeapPayload(heapArray, {targetIdx}), 5,
            "Re-balancing at swap position"));
        fixNode(targetIdx, timeline);
    }

    timeline.addFrame(Frame(HeapPayload(heapArray, {}), 6, "Removing successfully!"));
}

void IHeapStructure::update(int oldValue, int newValue, Timeline& timeline) {
    timeline.addFrame(Frame(HeapPayload(heapArray, {}), 1, "Starting updating..."));

    size_t targetIdx = INVALID_INDEX;
    for (size_t idx = 0; idx < heapArray.size(); ++idx) {
        if (heapArray[idx] == oldValue) {
            targetIdx = idx;
            break;
        }
    }

    if (targetIdx == INVALID_INDEX) {
        timeline.addFrame(Frame(HeapPayload(heapArray, {}), 2, "Old value not found!"));
        return;
    }

    heapArray[targetIdx] = newValue;
    timeline.addFrame(Frame(HeapPayload(heapArray, {targetIdx}), 3,
        "Value changed. Re-balancing..."));

    fixNode(targetIdx, timeline);

    timeline.addFrame(Frame(HeapPayload(heapArray, {}), 4, "Updating successfully!"));
}

void IHeapStructure::clear(Timeline& timeline) {
    timeline.addFrame(Frame(HeapPayload(heapArray, {}), 1, "Starting clearing..."));

    heapArray.clear();

    timeline.addFrame(Frame(HeapPayload(heapArray, {}), 2, "Clearing successfully!"));
}

// Extra functions
void IHeapStructure::extractTop(Timeline& timeline) {
    timeline.addFrame(Frame(HeapPayload(heapArray, {}), 1, "Starting extracting..."));

    if (heapArray.empty()) {
        timeline.addFrame(Frame(HeapPayload(heapArray, {}), 2, "Heap empty!"));
        return;
    }

    size_t lastIdx = heapArray.size() - 1;
    timeline.addFrame(Frame(HeapPayload(heapArray, {0, lastIdx}), 3,
        "Extracting top. Swapping with end."));

    std::swap(heapArray[0], heapArray.back());
    timeline.addFrame(Frame(HeapPayload(heapArray, {}), 4, "Removing the end"));

    heapArray.pop_back();
    if (!heapArray.empty()) {
        timeline.addFrame(Frame(HeapPayload(heapArray, {0}), 5, "Re-heapifying from the top"));
        heapify(heapArray.size(), 0, &timeline);
    }

    timeline.addFrame(Frame(HeapPayload(heapArray, {}), 6, "Extracting successfully!"));
}

void IHeapStructure::peek(Timeline& timeline) {
    timeline.addFrame(Frame(HeapPayload(heapArray, {}), 1, "Starting peeking..."));

    if (heapArray.empty()) {
        timeline.addFrame(Frame(HeapPayload(heapArray, {}), 2, "Heap was empty. Cannot peek anything!"));
        return;
    }

    timeline.addFrame(Frame(HeapPayload(heapArray, {0}), 3,
        "Top value found: " + std::to_string(heapArray[0]) + "."));
}
