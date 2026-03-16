#ifndef IDATASTRUCTURE_H
#define IDATASTRUCTURE_H

#include "Model/Timeline.h"
#include <vector>

class IDataStructure {
public:
    virtual ~IDataStructure() = default;
    
    virtual void initialize(const std::vector<int>& data, Timeline& timeline) = 0;

    virtual void insert(int value, Timeline& timeline) = 0;
    virtual void remove(int value, Timeline& timeline) = 0;
    virtual void search(int value, Timeline& timeline) = 0;
    
    virtual void clear(Timeline& timeline) = 0;
};

#endif // IDATASTRUCTURE_H