#ifndef FRAME_H
#define FRAME_H

#include <string>
#include <vector>

class Frame {
    private:
    std::vector<int> currentData;
    std::vector<size_t> highlightIndices;
    int codeLineId;
    std::string message;

    public:
    Frame(const std::vector<int>& data, const std::vector<size_t>& highlight, int lineId, const std::string& msg);

    const std::vector<int>& getCurrentData() const;
    const std::vector<size_t>& getHighlightIndices() const;
    int getCodeLineId() const;
    const std::string& getMessage() const;
};

#endif // FRAME_H