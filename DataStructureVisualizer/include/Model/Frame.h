#ifndef FRAME_H
#define FRAME_H

#include "Model/GraphStructure/Edge.h"
#include <string>
#include <vector>

class Frame {
    private:
    std::vector<int> arrayData;
    std::vector<std::string> stringData;

    std::vector<size_t> verticesData;
    std::vector<Edge> edgeData;

    std::vector<size_t> highlightIndices;
    std::vector<Edge> highlightEdges;

    int codeLineId;
    std::string message;

    public:
    Frame(const std::vector<int>& arrData, const std::vector<size_t>& highlights, int lineId, const std::string& msg);
    Frame(const std::vector<std::string>& strData, const std::vector<size_t>& highlights, int lineId, const std::string& msg);
    Frame(const std::vector<size_t>& vertices, const std::vector<Edge>& edges, const std::vector<size_t>& highlightedNodes, const std::vector<Edge>& highlightedEdges, int lineId, const std::string& msg);

    const std::vector<int>& getArrayData() const;
    const std::vector<std::string>& getStringData() const;

    const std::vector<size_t>& getVerticesData() const;
    const std::vector<Edge>& getEdgeData() const;

    const std::vector<size_t>& getHighlightIndices() const;
    const std::vector<Edge>& getHighlightEdges() const;

    int getCodeLineId() const;
    const std::string& getMessage() const;
};

#endif // FRAME_H