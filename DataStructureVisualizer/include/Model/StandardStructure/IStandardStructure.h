#ifndef ISTANDARDSTRUCTURE_H
#define ISTANDARDSTRUCTURE_H

#include "Model/IVisualizable.h"
#include "Utilities/LayoutConfig.h"

#include <vector>

class IStandardStructure : public IVisualizable {
    protected:
    LayoutConfig layoutConfig; 

    IStandardStructure(const LayoutConfig& config = LayoutConfig()) 
        : layoutConfig(config) {}

    public:
    virtual ~IStandardStructure() = default;
    
    virtual void initialize(const std::vector<int>& data, Timeline& timeline) = 0;
    virtual void insert(int value, Timeline& timeline) = 0;
    virtual void remove(int value, Timeline& timeline) = 0;
    virtual void search(int value, Timeline& timeline) = 0;
    virtual void update(int value1, int value2, Timeline& timeline) = 0;
};

#endif // ISTANDARDSTRUCTURE_H