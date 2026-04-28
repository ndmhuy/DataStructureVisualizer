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
    
    // Initializes the structure from raw data.
    virtual void initialize(const std::vector<int>& data, Timeline& timeline) = 0;
    // Inserts a value into the structure.
    virtual void insert(int value, Timeline& timeline) = 0;
    // Removes a value from the structure.
    virtual void remove(int value, Timeline& timeline) = 0;
    // Searches for a value in the structure.
    virtual void search(int value, Timeline& timeline) = 0;
    // Updates one value to another.
    virtual void update(int value1, int value2, Timeline& timeline) = 0;
};

#endif // ISTANDARDSTRUCTURE_H