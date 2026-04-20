#include "Model/Frame.h"

namespace {
const std::vector<int>& emptyIntVector() {
    static const std::vector<int> empty;
    return empty;
}

const std::vector<size_t>& emptySizeVector() {
    static const std::vector<size_t> empty;
    return empty;
}

const std::vector<Edge>& emptyEdgeVector() {
    static const std::vector<Edge> empty;
    return empty;
}

const LinkedListPayload& emptyLinkedListPayload() {
    static const LinkedListPayload empty;
    return empty;
}

const TreePayload& emptyTreePayload() {
    static const TreePayload empty{};
    return empty;
}

const HeapPayload& emptyHeapPayload() {
    static const HeapPayload empty;
    return empty;
}

const GraphPayload& emptyGraphPayload() {
    static const GraphPayload empty;
    return empty;
}

const SingleSourcePayload& emptySingleSourcePayload() {
    static const SingleSourcePayload empty;
    return empty;
}

const AStarPayload& emptyAStarPayload() {
    static const AStarPayload empty{};
    return empty;
}

const AllPairsPayload& emptyAllPairsPayload() {
    static const AllPairsPayload empty;
    return empty;
}
}

Frame::Frame(const std::vector<int>& arrayData, const std::vector<size_t>& highlightedIndices, int lineId, const std::string& msg)
    : codeLineId(lineId), message(msg), payload(std::in_place_type<HeapPayload>, arrayData, highlightedIndices) {}

Frame::Frame(const std::vector<size_t>& vertices, const std::vector<Edge>& edges, const std::vector<size_t>& highlightedVertices, const std::vector<Edge>& highlightedEdges, int lineId, const std::string& msg)
    : codeLineId(lineId), message(msg), payload(std::in_place_type<GraphPayload>, vertices, edges, highlightedVertices, highlightedEdges) {}

Frame::Frame(const LinkedListPayload& p, const std::string& msg, int lineId)
    : codeLineId(lineId), message(msg), payload(std::in_place_type<LinkedListPayload>, p) {}

Frame::Frame(const TreePayload& p, const std::string& msg, int lineId)
    : codeLineId(lineId), message(msg), payload(std::in_place_type<TreePayload>, p) {}

Frame::Frame(const HeapPayload& p, const std::string& msg, int lineId)
    : codeLineId(lineId), message(msg), payload(std::in_place_type<HeapPayload>, p) {}

Frame::Frame(const GraphPayload& p, const std::string& msg, int lineId)
    : codeLineId(lineId), message(msg), payload(std::in_place_type<GraphPayload>, p) {}

Frame::Frame(const SingleSourcePayload& p, const std::string& msg, int lineId)
    : codeLineId(lineId), message(msg), payload(std::in_place_type<SingleSourcePayload>, p) {}

Frame::Frame(const AStarPayload& p, const std::string& msg, int lineId)
    : codeLineId(lineId), message(msg), payload(std::in_place_type<AStarPayload>, p) {}

Frame::Frame(const AllPairsPayload& p, const std::string& msg, int lineId)
    : codeLineId(lineId), message(msg), payload(std::in_place_type<AllPairsPayload>, p) {}

const std::variant<LinkedListPayload, TreePayload, HeapPayload, GraphPayload, SingleSourcePayload, AStarPayload, AllPairsPayload>& Frame::getPayload() const {
    return payload;
}

int Frame::getCodeLineId() const {
    return codeLineId;
}

const std::string& Frame::getMessage() const {
    return message;
}

const std::vector<int>& Frame::getArrayData() const {
    if (const auto* linkedList = std::get_if<LinkedListPayload>(&payload)) {
        return linkedList->values;
    }
    if (const auto* heap = std::get_if<HeapPayload>(&payload)) {
        return heap->arrayData;
    }
    return emptyIntVector();
}

const std::vector<size_t>& Frame::getHighlightIndices() const {
    if (const auto* linkedList = std::get_if<LinkedListPayload>(&payload)) {
        return linkedList->highlightedNodes;
    }
    if (const auto* tree = std::get_if<TreePayload>(&payload)) {
        return tree->highlightedNodes;
    }
    if (const auto* heap = std::get_if<HeapPayload>(&payload)) {
        return heap->highlightedIndices;
    }
    if (const auto* graph = std::get_if<GraphPayload>(&payload)) {
        return graph->highlightedVertices;
    }
    if (const auto* singleSource = std::get_if<SingleSourcePayload>(&payload)) {
        return singleSource->highlightedVertices;
    }
    if (const auto* aStar = std::get_if<AStarPayload>(&payload)) {
        return aStar->highlightedVertices;
    }
    if (const auto* allPairs = std::get_if<AllPairsPayload>(&payload)) {
        return allPairs->highlightedVertices;
    }
    return emptySizeVector();
}

const std::vector<size_t>& Frame::getVerticesData() const {
    if (const auto* graph = std::get_if<GraphPayload>(&payload)) {
        return graph->vertices;
    }
    return emptySizeVector();
}

const std::vector<Edge>& Frame::getEdgeData() const {
    if (const auto* graph = std::get_if<GraphPayload>(&payload)) {
        return graph->edges;
    }
    return emptyEdgeVector();
}

const std::vector<Edge>& Frame::getHighlightEdges() const {
    if (const auto* graph = std::get_if<GraphPayload>(&payload)) {
        return graph->highlightedEdges;
    }
    if (const auto* singleSource = std::get_if<SingleSourcePayload>(&payload)) {
        return singleSource->highlightedEdges;
    }
    if (const auto* aStar = std::get_if<AStarPayload>(&payload)) {
        return aStar->highlightedEdges;
    }
    if (const auto* allPairs = std::get_if<AllPairsPayload>(&payload)) {
        return allPairs->highlightedEdges;
    }
    return emptyEdgeVector();
}

const LinkedListPayload& Frame::getLinkedListPayload() const {
    if (const auto* linkedList = std::get_if<LinkedListPayload>(&payload)) {
        return *linkedList;
    }
    return emptyLinkedListPayload();
}

const TreePayload& Frame::getTreePayload() const {
    if (const auto* tree = std::get_if<TreePayload>(&payload)) {
        return *tree;
    }
    return emptyTreePayload();
}

const HeapPayload& Frame::getHeapPayload() const {
    if (const auto* heap = std::get_if<HeapPayload>(&payload)) {
        return *heap;
    }
    return emptyHeapPayload();
}

const GraphPayload& Frame::getGraphPayload() const {
    if (const auto* graph = std::get_if<GraphPayload>(&payload)) {
        return *graph;
    }
    return emptyGraphPayload();
}

const SingleSourcePayload& Frame::getSingleSourcePayload() const {
    if (const auto* singleSource = std::get_if<SingleSourcePayload>(&payload)) {
        return *singleSource;
    }
    return emptySingleSourcePayload();
}

const AStarPayload& Frame::getAStarPayload() const {
    if (const auto* aStar = std::get_if<AStarPayload>(&payload)) {
        return *aStar;
    }
    return emptyAStarPayload();
}

const AllPairsPayload& Frame::getAllPairsPayload() const {
    if (const auto* allPairs = std::get_if<AllPairsPayload>(&payload)) {
        return *allPairs;
    }
    return emptyAllPairsPayload();
}