#include "Model/Frame.h"
#include <limits>
#include <stdexcept>

Frame::Frame(const std::vector<int>& arrData, const std::vector<size_t>& highlights, int lineId, const std::string& msg) : arrayData(arrData), highlightIndices(highlights), codeLineId(lineId), message(msg) {}

Frame::Frame(const std::vector<std::string>& strData, const std::vector<size_t>& highlights, int lineId, const std::string& msg) : stringData(strData), highlightIndices(highlights), codeLineId(lineId), message(msg) {}

Frame::Frame(const std::vector<size_t>& vertices, const std::vector<Edge>& edges, const std::vector<size_t>& highlightedNodes, const std::vector<Edge>& highlightedEdges, int lineId, const std::string& msg) : verticesData(vertices), edgeData(edges), highlightIndices(highlightedNodes), highlightEdges(highlightedEdges), codeLineId(lineId), message(msg) {}

const std::vector<int>& Frame::getArrayData() const {
    return arrayData;
}

const std::vector<std::string>& Frame::getStringData() const {
    return stringData;
}

const std::vector<size_t>& Frame::getVerticesData() const {
    return verticesData;
}

const std::vector<Edge>& Frame::getEdgeData() const {
    return edgeData;
}

const std::vector<size_t>& Frame::getHighlightIndices() const {
    return highlightIndices;
}

const std::vector<Edge>& Frame::getHighlightEdges() const {
    return highlightEdges;
}

int Frame::getCodeLineId() const {
    return codeLineId;
}

const std::string& Frame::getMessage() const {
    return message;
}