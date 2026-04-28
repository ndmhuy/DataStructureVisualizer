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
    // Creates a frame for a linked-list payload.
    Frame(const LinkedListPayload& p, int lineId, const std::string& msg);
    // Creates a frame for a tree payload.
    Frame(const TreePayload& p, int lineId, const std::string& msg);
    // Creates a frame for a heap payload.
    Frame(const HeapPayload& p, int lineId, const std::string& msg);
    // Creates a frame for a graph payload.
    Frame(const GraphPayload& p, int lineId, const std::string& msg);
    // Creates a frame for a single-source shortest-path payload.
    Frame(const SingleSourcePayload& p, int lineId, const std::string& msg);
    // Creates a frame for an A* payload.
    Frame(const AStarPayload& p, int lineId, const std::string& msg);
    // Creates a frame for an all-pairs payload.
    Frame(const AllPairsPayload& p, int lineId, const std::string& msg);
    // Creates a frame for a grid payload.
    Frame(const GridPayload& p, int lineId, const std::string& msg);

    // Rule of Three: destructor, copy constructor, assignment operator
    // Destroys the frame and its owned payload.
    ~Frame();
    // Copies a frame and clones its payload.
    Frame(const Frame& other);
    // Assigns a frame and clones its payload.
    Frame& operator=(const Frame& other);

    // Getters
    // Returns the owned payload.
    const IPayload* getPayload() const;
    // Returns the code line identifier.
    int getCodeLineId() const;
    // Returns the message attached to the frame.
    const std::string& getMessage() const;
};

#endif // FRAME_H