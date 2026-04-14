#include "Renderer.h"
// #include "Model/StandardStructure/AVLTree.h"

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
    float startY = 225;
    float distanceHorizontal = 30; // Deepest leaf nodes
    float distanceVertical = 50;
    float height = ceil(log2(treeArray.size()));

    for (size_t i = 0; i < treeArray.size(); i++) {
        // Coordinates calculation
        if (treeArray[i] != INT_MAX) { // In AVLTree model, nullptr should be set as INT_MAX in toVector()
            int level = std::floor(log2(i + 1));
            int posInLevel = i - (std::pow(2, level) - 1);
            float levelSpacing = distanceHorizontal * std::pow(2, height - level);
            int nodesInLevel = std::pow(2, level);
            float levelWidth = levelSpacing * nodesInLevel;

            float currentY = startY + level * distanceVertical;
            float currentX = startX - levelWidth / 2 + (posInLevel + 0.5f) * levelSpacing;

            positions[i] = {currentX, currentY};
        }
        else {
            positions[i] = {0, 0};
        }

    }

    // Draw edges then draw nodes
    sf::Vector2f nodeSize = getNodeSize();
    for (size_t i = 0; i < treeArray.size(); ++i) {
        if (treeArray[i] != INT_MAX) {
            bool isHighlighted = std::find(highlights.begin(), highlights.end(), i) != highlights.end();
            // Heap style
            size_t leftIdx = 2*i+1, rightIdx = 2*i+2;
            
            if (leftIdx < treeArray.size() && treeArray[leftIdx] != INT_MAX) {
                bool isHighlightedLeft = std::find(highlights.begin(), highlights.end(), leftIdx) != highlights.end();
                drawLine(
                    positions[i], nodeSize, ShapeType::Circle,
                    positions[leftIdx], nodeSize, ShapeType::Circle,
                    3.0f, isHighlighted && isHighlightedLeft
                );
            }
            if (rightIdx < treeArray.size() && treeArray[rightIdx] != INT_MAX) {
                bool isHighlightedRight = std::find(highlights.begin(), highlights.end(), rightIdx) != highlights.end();
                drawLine(
                    positions[i], nodeSize, ShapeType::Circle,
                    positions[rightIdx], nodeSize, ShapeType::Circle,
                    3.0f, isHighlighted && isHighlightedRight
                );
            }
        }
    } 

    for (size_t i = 0; i < treeArray.size(); ++i) {
        if (treeArray[i] != INT_MAX) {
            bool isHighlighted = std::find(highlights.begin(), highlights.end(), i) != highlights.end();
            drawImageNode(positions[i], std::to_string(treeArray[i]), isHighlighted);
        }
    }
}