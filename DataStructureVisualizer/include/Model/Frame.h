#ifndef FRAME_H
#define FRAME_H

#include "Model/Payloads/Payloads.h"
#include <string>
#include <vector>

class Frame {
private:
    int codeLineId;
    std::string message;
    IPayload* payload;

public:
    Frame(const LinkedListPayload& p, int lineId, const std::string& msg);
    Frame(const TreePayload& p, int lineId, const std::string& msg);
    Frame(const HeapPayload& p, int lineId, const std::string& msg);
    Frame(const GraphPayload& p, int lineId, const std::string& msg);
    Frame(const SingleSourcePayload& p, int lineId, const std::string& msg);
    Frame(const AStarPayload& p, int lineId, const std::string& msg);
    Frame(const AllPairsPayload& p, int lineId, const std::string& msg);

    // Rule of Three: destructor, copy constructor, assignment operator
    ~Frame();
    Frame(const Frame& other);
    Frame& operator=(const Frame& other);

    // Getters
    const IPayload* getPayload() const;
    int getCodeLineId() const;
    const std::string& getMessage() const;
};

#endif // FRAME_H