#include "View/Renderer.h"
#include "Model/Frame.h"
#include "Model/HeapStructure/IHeapStructure.h"

#include <string>
#include <algorithm>

void Renderer::drawHeapFrame(const Frame& frame) {
    const auto& heapArray = frame.getArrayData();
    const auto& highlights = frame.getHighlightIndices();
    
    if (heapArray.empty())
        return;
    
    // Coordinates temp buffer
    std::vector<sf::Vector2f> positions(heapArray.size());

    // Assume the resolution is 1600:900, otherwise we have to use the current resolution
    // to compute the suitable values
    float startX = 800;
    float startY = 225;
    float distanceHorizontal = 30; // Deepest leaf nodes
    float distanceVertical = 50;
    float height = std::ceil(std::log2(heapArray.size()));

    defaultNodePositions.clear(); // Xóa dữ liệu cũ phục vụ Drag & Drop

    for (size_t i = 0; i < heapArray.size(); i++) {
        // Coordinates calculation
        int level = std::floor(std::log2(i + 1));
        int posInLevel = i - (std::pow(2, level) - 1);
        float levelSpacing = distanceHorizontal * std::pow(2, height - level);
        int nodesInLevel = std::pow(2, level);
        float levelWidth = levelSpacing * nodesInLevel;

        float currentY = startY + level * distanceVertical;
        float currentX = startX - levelWidth / 2 + (posInLevel + 0.5f) * levelSpacing;

        positions[i] = {currentX, currentY};

        defaultNodePositions[i] = positions[i]; // Lưu lại phục vụ hit-box
        if (customNodePositions.find(i) != customNodePositions.end()) {
            positions[i] = customNodePositions[i]; // Dùng vị trí tùy chỉnh nếu có
        }
    }

    // Draw edges then draw nodes
    sf::Vector2f nodeSize = getNodeSize();
    for (size_t i = 0; i < heapArray.size(); ++i) {
        bool isHighlighted = std::find(highlights.begin(), highlights.end(), i) != highlights.end();
        size_t parentIdx = parent(i); // parent should return INT_MAX if there is no parent

        if (parentIdx != INT_MAX) {
            // Highlighting
            bool isHighlightedParent = std::find(highlights.begin(), highlights.end(), parentIdx) != highlights.end();
            drawLine(
                positions[i], nodeSize, ShapeType::Circle,
                positions[parentIdx], nodeSize, ShapeType::Circle,
                3.0f, isHighlighted && isHighlightedParent
            );
        }
    } 

    for (size_t i = 0; i < heapArray.size(); ++i) {
        bool isHighlighted = std::find(highlights.begin(), highlights.end(), i) != highlights.end();
        drawImageNode(positions[i], std::to_string(heapArray[i]), isHighlighted);
    }
}