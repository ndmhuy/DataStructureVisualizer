#include "Model/Frame.h"

Frame::Frame(const std::vector<int>& data, const std::vector<size_t>& highlight, int lineId, const std::string& msg)
    : currentData(data), highlightIndices(highlight), codeLineId(lineId), message(msg) {}

const std::vector<int>& Frame::getCurrentData() const {
    return currentData;
}

const std::vector<size_t>& Frame::getHighlightIndices() const {
    return highlightIndices;
}

int Frame::getCodeLineId() const {
    return codeLineId;
}

const std::string& Frame::getMessage() const {
    return message;
}