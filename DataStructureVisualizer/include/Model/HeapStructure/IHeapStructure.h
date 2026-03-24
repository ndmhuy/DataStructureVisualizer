#ifndef IHEAPSTRUCTURE_H
#define IHEAPSTRUCTURE_H

#include "Model/IVisualizable.h"
#include <vector>

class IHeapStructure : public IVisualizable {
    public:
    virtual void initialize(const std::vector<int>& data, Timeline& timeline) = 0;
    virtual void insert(int value, Timeline& timeline) = 0;

    virtual void extractTop(Timeline& timeline) = 0;
    virtual void peek(Timeline& timeline) = 0;
};

#endif // IHEAPSTRUCTURE_H