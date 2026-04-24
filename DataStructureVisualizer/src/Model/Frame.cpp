#include "Model/Frame.h"

Frame::Frame(const LinkedListPayload& p, int lineId, const std::string& msg)
    : codeLineId(lineId), message(msg), payload(new LinkedListPayload(p)) {}

Frame::Frame(const TreePayload& p, int lineId, const std::string& msg)
    : codeLineId(lineId), message(msg), payload(new TreePayload(p)) {}

Frame::Frame(const HeapPayload& p, int lineId, const std::string& msg)
    : codeLineId(lineId), message(msg), payload(new HeapPayload(p)) {}

Frame::Frame(const GraphPayload& p, int lineId, const std::string& msg)
    : codeLineId(lineId), message(msg), payload(new GraphPayload(p)) {}

Frame::Frame(const SingleSourcePayload& p, int lineId, const std::string& msg)
    : codeLineId(lineId), message(msg), payload(new SingleSourcePayload(p)) {}

Frame::Frame(const AStarPayload& p, int lineId, const std::string& msg)
    : codeLineId(lineId), message(msg), payload(new AStarPayload(p)) {}

Frame::Frame(const AllPairsPayload& p, int lineId, const std::string& msg)
    : codeLineId(lineId), message(msg), payload(new AllPairsPayload(p)) {}

Frame::~Frame() {
    delete payload;
}

Frame::Frame(const Frame& other)
    : codeLineId(other.codeLineId), message(other.message), payload(other.payload ? other.payload->clone() : nullptr) {}

Frame& Frame::operator=(const Frame& other) {
    if (this != &other) {
        codeLineId = other.codeLineId;
        message = other.message;
        delete payload;
        payload = other.payload ? other.payload->clone() : nullptr;
    }
    return *this;
}

const IPayload* Frame::getPayload() const {
    return payload;
}

int Frame::getCodeLineId() const {
    return codeLineId;
}

const std::string& Frame::getMessage() const {
    return message;
}