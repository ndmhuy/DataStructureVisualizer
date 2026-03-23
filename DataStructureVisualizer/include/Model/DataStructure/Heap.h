#ifndef HEAP_HPP
#define HEAP_HPP

#include "Model/IDataStructure.h"
#include "Model/Timeline.h"
#include <vector>

class Heap : public IDataStructure {
    private:

    std::vector<int> heapArray;

    std::vector<int> toVector() const;
    int parent(int n, int idx);
    int left(int n, int idx);
    int right(int n, int idx);
    void maxHeapify(int n, int idx);
    void buildMaxHeap(int n);
    void deleteArray();

    public:
    Heap();
    ~Heap() override;

    Heap(const Heap&) = delete;
    Heap& operator=(const Heap&) = delete;

    void initialize(const std::vector<int>& data, Timeline& timeline) override;
    void insert(int value, Timeline& timeline) override;
    void remove(int value, Timeline& timeline) override;
    void search(int value, Timeline& timeline) override;
    void clear(Timeline& timeline) override;
};

#endif
