#ifndef IVISUALIZABLE_H
#define IVISUALIZABLE_H

#include "Utilities/StructureType.h"
#include "Model/Timeline.h"

class IVisualizable {
public:
    virtual ~IVisualizable() = default;
    
    virtual void clear(Timeline& timeline) = 0;

    virtual StructureType getStructureType() const = 0;
};

#endif // IVISUALIZABLE_H