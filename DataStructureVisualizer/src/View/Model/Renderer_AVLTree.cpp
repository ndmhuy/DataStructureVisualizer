#include "Renderer.h"
#include "Model/StandardStructure/AVLTree.h"

#include <algorithm>

void Renderer::drawAVLTreeFrame(const Frame& frame) {
    const auto& treeArray = frame.getArrayData();
    const auto& highlights = frame.getHighlightIndices();
    
    if (treeArray.empty())
        return;
    
    // Coordinates temp buffer
    std::vector<sf::Vector2f> positions(treeArray.size());

    // Assume the resolution is 1600:900, otherwise we have to use the current resolution
    // to compute the suitable values
    float startX = 800;
    float startY = 150;
    float distanceHorizontal = 35; // Deepest leaf nodes
    float distanceVertical = 70;
    float height = ceil(log2(treeArray.size()+1));

    for (size_t idx = 0; idx < treeArray.size(); idx++) {
        // Coordinates calculation
        if (treeArray[idx] != INT_MAX) { // In AVLTree model, nullptr should be set as INT_MAX in toVector()
            int level = std::floor(log2(idx + 1));
            int posInLevel = idx - (std::pow(2, level) - 1);
            float levelSpacing = distanceHorizontal * std::pow(2, height - level);
            int nodesInLevel = std::pow(2, level);
            float levelWidth = levelSpacing * nodesInLevel;

            float currentX = startX - levelWidth / 2 + (posInLevel + 0.5f) * levelSpacing;
            float currentY = startY + level * distanceVertical;

            positions[idx] = {currentX, currentY};
        }
    }

    // Draw edges then draw nodes
    sf::Vector2f nodeSize = getNodeSize();
    for (size_t idx = 1; idx < treeArray.size(); ++idx) {
        if (treeArray[idx] != INT_MAX) {
            // Heap style      
            size_t parentIdx = (idx - 1) / 2; 

            if (treeArray[parentIdx] != INT_MAX) {
                bool isHighlighted = std::find(highlights.begin(), highlights.end(), idx) != highlights.end();
                bool isHighlightedParent = std::find(highlights.begin(), highlights.end(), parentIdx) != highlights.end();
                
                drawLine(positions[idx], nodeSize, ShapeType::Circle,
                        positions[parentIdx], nodeSize, ShapeType::Circle,
                        3.0f, isHighlighted && isHighlightedParent);
            }
        }
    } 

    for (size_t idx = 0; idx < treeArray.size(); ++idx) {
        if (treeArray[idx] != INT_MAX) {
            bool isHighlighted = std::find(highlights.begin(), highlights.end(), idx) != highlights.end();
            drawImageNode(positions[idx], std::to_string(treeArray[idx]), isHighlighted);
        }
    }
}