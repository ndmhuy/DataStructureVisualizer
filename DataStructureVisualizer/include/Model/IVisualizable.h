#ifndef IVISUALIZABLE_H
#define IVISUALIZABLE_H

#include "Utilities/StructureType.h"
#include "Model/Timeline.h"

class IVisualizable {
public:
    // Destroys the visualizable interface.
    virtual ~IVisualizable() = default;
    
    // Clears the underlying structure and logs the change.
    virtual void clear(Timeline& timeline) = 0;

    // Returns the structure type used by the UI.
    virtual StructureType getStructureType() const = 0;
};

#endif // IVISUALIZABLE_H