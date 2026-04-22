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
    float startY = 150;
    float distanceHorizontal = 35; // Deepest leaf nodes
    float distanceVertical = 70;
    int heap_size = heapArray.size();
    
    float height = std::ceil(log2(heapArray.size()+1));

    for (size_t idx = 0; idx < heapArray.size(); idx++) {
        if (heapArray[idx] == INT_MAX)
            continue;

        // Coordinates calculation
        int level = std::floor(log2(idx + 1));
        int posInLevel = idx - (std::pow(2, level) - 1);
        float levelSpacing = distanceHorizontal * std::pow(2, height - level);
        int nodesInLevel = std::pow(2, level);
        float levelWidth = levelSpacing * nodesInLevel;

        float currentX = startX - levelWidth / 2 + (posInLevel + 0.5f) * levelSpacing;
        float currentY = startY + level * distanceVertical;

        positions[idx] = {currentX, currentY};
    }

    // Draw edges then draw nodes
    sf::Vector2f nodeSize = getNodeSize();
    for (size_t idx = 0; idx < heapArray.size(); ++idx) {
        size_t parentIdx = parent(heapArray.size(), idx); // parent return INT_MAX if there is no parent

        if (parentIdx == INT_MAX || heapArray[parentIdx] == INT_MAX || heapArray[idx] == INT_MAX)
            continue;

        // Highlighting
        bool isHighlighted = std::find(highlights.begin(), highlights.end(), idx) != highlights.end();
        bool isHighlightedParent = std::find(highlights.begin(), highlights.end(), parentIdx) != highlights.end();
        
        drawLine(
            positions[idx], nodeSize, ShapeType::Circle,
            positions[parentIdx], nodeSize, ShapeType::Circle,
            3.0f, isHighlighted && isHighlightedParent
        );
    } 

    for (size_t idx = 0; idx < heapArray.size(); ++idx) {
        if (heapArray[idx] == INT_MAX)
            continue;

        bool isHighlighted = std::find(highlights.begin(), highlights.end(), idx) != highlights.end();
        drawImageNode(positions[idx], std::to_string(heapArray[idx]), isHighlighted);
    }
}