#ifndef IPAYLOAD_H
#define IPAYLOAD_H

#include "IPayloadVisitor.h"

class IPayload {
public:
    virtual ~IPayload() = default;
    virtual void accept(IPayloadVisitor& visitor) const = 0;
    
    // Required since Frame stores a raw pointer and Timeline uses std::vector<Frame>
    virtual IPayload* clone() const = 0;
};

#endif // IPAYLOAD_H
