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
    // Retain old signatures to minimize disruption in other files, but instantiate payloads dynamically inside
    Frame(const std::vector<int>& arrayData, const std::vector<size_t>& highlightedIndices, int lineId, const std::string& msg);
    Frame(const std::vector<size_t>& vertices, const std::vector<Edge>& edges, const std::vector<size_t>& highlightedVertices, const std::vector<Edge>& highlightedEdges, int lineId, const std::string& msg);
    
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