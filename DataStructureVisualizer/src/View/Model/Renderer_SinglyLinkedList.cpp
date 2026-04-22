#include "View/Renderer.h"
#include "Model/StandardStructure/SinglyLinkedList.h"

#include <algorithm>

void Renderer::drawSinglyLinkedListFrame(const Frame& frame) {
    const auto& listArray = frame.getArrayData();
    const auto& highlights = frame.getHighlightIndices();
    
    if (listArray.empty())
        return;
    
    // Coordinates temp buffer
    std::vector<sf::Vector2f> positions(listArray.size());

    sf::Vector2u winSize = window.getWindow().getSize();
    float nodeWidth = 80.0f;
    float spacing = 60.0f; 

    float totalWidth = listArray.size() * (nodeWidth + spacing) - spacing;
    float startX = (winSize.x - totalWidth) / 2.0f;
    float startY = winSize.y / 2.0f;

    for (size_t idx = 0; idx < listArray.size(); ++idx) {
        positions[idx] = {startX + idx * (nodeWidth + spacing), startY};
    }

    // Draw edges then draw nodes
    sf::Vector2f nodeSize = getNodeSize();
    for (size_t idx = 1; idx < listArray.size(); ++idx) {
        // Highlighting
        bool isHighlighted = std::find(highlights.begin(), highlights.end(), idx) != highlights.end();
        bool isHighlightedPrev = std::find(highlights.begin(), highlights.end(), idx-1) != highlights.end();

        drawLineWithArrow(
            positions[idx], nodeSize, ShapeType::Circle,
            positions[idx-1], nodeSize, ShapeType::Circle,
            3.0f, 12.0f, isHighlighted && isHighlightedPrev
        );
    } 

    for (size_t idx = 0; idx < listArray.size(); ++idx) {
        bool isHighlighted = std::find(highlights.begin(), highlights.end(), idx) != highlights.end();
        drawImageNode(positions[idx], std::to_string(listArray[idx]), isHighlighted);
    }
}