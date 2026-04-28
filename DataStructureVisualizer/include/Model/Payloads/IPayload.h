#ifndef IPAYLOAD_H
#define IPAYLOAD_H

#include "IPayloadVisitor.h"

class IPayload {
public:
    // Destroys the payload interface.
    virtual ~IPayload() = default;
    // Dispatches the payload to the active visitor.
    virtual void accept(IPayloadVisitor& visitor) const = 0;
    
    // Required since Frame stores a raw pointer and Timeline uses std::vector<Frame>
    // Creates a deep copy of the payload.
    virtual IPayload* clone() const = 0;
};

#endif // IPAYLOAD_H
