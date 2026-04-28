#include "View/Render/Renderer.h"
#include "Model/Frame.h"
#include "Utilities/GlobalConstant.h"
#include "Utilities/LayoutConfig.h"

#include <algorithm>

Renderer::Renderer(Window& m_window, const Theme& m_theme)
    : window(m_window), theme(m_theme), bgSprite(bgTexture) {}

bool Renderer::loadAssets() {
    if (!mainFont.openFromFile(theme.fontPath)) return false;
    if (!bgTexture.loadFromFile(theme.bgImagePath)) return false;
    if (!nodeTexture.loadFromFile(theme.nodeImagePath)) return false;
    if (!arrayTexture.loadFromFile(theme.arrayImagePath)) return false;

    bgTexture.setSmooth(true);
    bgSprite.setTexture(bgTexture, true);

    sf::Vector2u texSize = bgTexture.getSize();
    sf::Vector2u winSize = window.getWindow().getSize();
    bgSprite.setScale({static_cast<float>(winSize.x) / texSize.x, static_cast<float>(winSize.y) / texSize.y});

    nodeTexture.setSmooth(true);
    arrayTexture.setSmooth(true);
    return true;
}

void Renderer::drawBackground() {
    sf::View currentView = window.getWindow().getView();
    window.getWindow().setView(window.getWindow().getDefaultView());
    window.getWindow().draw(bgSprite);
    window.getWindow().setView(currentView);
}

void Renderer::drawImageNode(sf::Vector2f pos, const std::string& text, bool isHighlighted) {
    // node
    sf::Sprite sprite(nodeTexture);
    sprite.setColor(isHighlighted ? theme.highlightColor : theme.nodeTintColor);
    sprite.setScale({theme.nodeScale, theme.nodeScale});
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin({bounds.size.x / 2, bounds.size.y / 2});
    sprite.setPosition(pos);
    window.getWindow().draw(sprite);

    // text
    unsigned int textSize = static_cast<unsigned int>(theme.nodeTextBaseSize * theme.nodeScale);
    sf::Text a(mainFont, text, textSize);
    a.setFillColor(theme.textColor);
    sf::FloatRect textRect = a.getLocalBounds();
    a.setOrigin({
        textRect.position.x + textRect.size.x / 2,
        textRect.position.y + textRect.size.y - theme.nodeTextVerticalOffset * theme.nodeScale
    });
    a.setPosition(pos);
    window.getWindow().draw(a);
}

void Renderer::drawArrayCell(sf::Vector2f pos, const std::string& text, bool isHighlighted) {
    sf::Sprite sprite(arrayTexture);
    sprite.setColor(isHighlighted ? theme.highlightColor : theme.arrayTintColor);
    sprite.setScale({theme.arrayScale, theme.arrayScale});
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin({bounds.size.x / 2, bounds.size.y / 2});
    sprite.setPosition(pos);
    window.getWindow().draw(sprite);

    unsigned int textSize = static_cast<unsigned int>(theme.arrayTextBaseSize * theme.arrayScale);
    sf::Text a(mainFont, text, textSize);
    a.setFillColor(theme.textColor);
    sf::FloatRect textRect = a.getLocalBounds();
    a.setOrigin({textRect.position.x + textRect.size.x / 2, textRect.position.y + textRect.size.y / 2});
    a.setPosition(pos);
    window.getWindow().draw(a);
}

void Renderer::drawLineWithArrow(
    sf::Vector2f p1,
    sf::Vector2f size1,
    ShapeType type1,
    sf::Vector2f p2,
    sf::Vector2f size2,
    ShapeType type2,
    float thickness,
    float arrowSize,
    bool isHighlighted,
    float offset) {
    float dx = p2.x - p1.x;
    float dy = p2.y - p1.y;
    float distance = std::sqrt(dx * dx + dy * dy);

    float angle = std::atan2(dy, dx);
    float angleDegrees = angle * 180.0f / M_PI;

    sf::Vector2f start = getBoundaryPoint(p1, size1, angle, type1);
    
    sf::Vector2f end = getBoundaryPoint(p2, size2, angle + M_PI, type2);

    if (offset != 0.0f && distance > 0.0001f) {
        sf::Vector2f perp(-dy / distance, dx / distance);
        start += perp * offset;
        end += perp * offset;
    }

    float lineLength = std::sqrt(std::pow(end.x - start.x, 2) + std::pow(end.y - start.y, 2));

    if (distance < (size1.x / 2 + size2.x / 2)) return;

    sf::RectangleShape line({lineLength, thickness});
    line.setFillColor(isHighlighted ? theme.accentColor : theme.arrowColor);
    line.setOrigin({0, thickness / 2.0f});
    line.setPosition(start);
    line.setRotation(sf::degrees(angleDegrees));
    window.getWindow().draw(line);

    sf::ConvexShape arrowHead;
    arrowHead.setPointCount(3);
    arrowHead.setPoint(0, {0.0f, 0.0f});
    arrowHead.setPoint(1, {-arrowSize, -arrowSize / 1.5f});
    arrowHead.setPoint(2, {-arrowSize, arrowSize / 1.5f});

    arrowHead.setFillColor(isHighlighted ? theme.accentColor : theme.arrowColor);
    arrowHead.setPosition(end);
    arrowHead.setRotation(sf::degrees(angleDegrees));
    
    window.getWindow().draw(arrowHead);
}

void Renderer::drawLine(sf::Vector2f p1, sf::Vector2f size1, ShapeType type1, sf::Vector2f p2, sf::Vector2f size2, ShapeType type2, float thickness, bool isHighlighted, float offset) {
    float dx = p2.x - p1.x;
    float dy = p2.y - p1.y;
    float distance = std::sqrt(dx * dx + dy * dy);

    float angle = std::atan2(dy, dx);
    float angleDegrees = angle * 180.0f / M_PI;

    sf::Vector2f start = getBoundaryPoint(p1, size1, angle, type1);
    
    sf::Vector2f end = getBoundaryPoint(p2, size2, angle + M_PI, type2);

    if (offset != 0.0f && distance > 0.0001f) {
        sf::Vector2f perp(-dy / distance, dx / distance);
        start += perp * offset;
        end += perp * offset;
    }

    float lineLength = std::sqrt(std::pow(end.x - start.x, 2) + std::pow(end.y - start.y, 2));

    if (distance < (size1.x / 2 + size2.x / 2)) return;

    sf::RectangleShape line({lineLength, thickness});
    line.setFillColor(isHighlighted ? theme.accentColor : theme.arrowColor);
    line.setOrigin({0, thickness / 2.0f});
    line.setPosition(start);
    line.setRotation(sf::degrees(angleDegrees));
    window.getWindow().draw(line);
}

sf::Vector2f Renderer::getBoundaryPoint(sf::Vector2f center, sf::Vector2f size, float angle, ShapeType type) {
    float cx = center.x, cy = center.y;
    float width = size.x, height = size.y;
    if (type == ShapeType::Circle) {
        float radius = width / 2.0f;
        return {cx + radius * std::cos(angle), cy + radius * std::sin(angle)};
    } else {
        float absCos = std::abs(std::cos(angle));
        float absSin = std::abs(std::sin(angle));
        
        float dist = 0.0f;
        if (absCos > 0.0001f && absSin > 0.0001f) {
            float distX = (width / 2.0f) / absCos;
            float distY = (height / 2.0f) / absSin;
            dist = std::min(distX, distY);
        } else if (absCos <= 0.0001f) {
            dist = height / 2.0f;
        } else {
            dist = width / 2.0f;
        }
        
        return {cx + dist * std::cos(angle), cy + dist * std::sin(angle)};
    }
}
sf::Vector2f Renderer::getNodeSize() const {
    return {nodeTexture.getSize().x * theme.nodeScale,
            nodeTexture.getSize().y * theme.nodeScale};
}

sf::Vector2f Renderer::getArraySize() const {
    return {arrayTexture.getSize().x * theme.arrayScale,
            arrayTexture.getSize().y * theme.arrayScale};
}

void Renderer::drawText(sf::Vector2f pos, const std::string& text, unsigned int size, sf::Color color, TextPosition align, sf::Angle angle) {
    sf::Text t(mainFont, text, size);
    t.setFillColor(color);
    
    sf::FloatRect bounds = t.getLocalBounds();
    
    float originX = bounds.position.x;
    float originY = bounds.position.y;

    switch (align) {
        case TextPosition::TopLeft:
            break;
        case TextPosition::TopRight:
            originX += bounds.size.x;
            break;
        case TextPosition::BottomLeft:
            originY += bounds.size.y;
            break;
        case TextPosition::BottomRight:
            originX += bounds.size.x;
            originY += bounds.size.y;
            break;
        case TextPosition::Top:
            originX += bounds.size.x / 2.0f;
            break;
        case TextPosition::Bottom:
            originX += bounds.size.x / 2.0f;
            originY += bounds.size.y;
            break;
    }

    t.setOrigin({originX, originY});
    t.setPosition(pos);
    t.setRotation(angle);

    window.getWindow().draw(t);
}

void Renderer::drawTextPositioned(
    sf::Vector2f basePosition,
    const std::string& text,
    unsigned int size,
    sf::Color color,
    TextPositionMode mode,
    float objHeight,
    float objWidth,
    float padding,
    sf::Vector2f lineEnd,
    TextPosition align,
    sf::Angle angle) {

    sf::Vector2f finalPos = basePosition;
    TextPosition finalAlign = align;
    sf::Angle finalAngle = angle;

    switch (mode) {
        case TextPositionMode::Absolute:
            // Use basePosition as is with provided alignment
            break;
        case TextPositionMode::Up:
            finalPos.y -= (objHeight / 2.0f) + padding;
            finalAlign = TextPosition::Bottom;
            break;
        case TextPositionMode::Down:
            finalPos.y += (objHeight / 2.0f) + padding;
            finalAlign = TextPosition::Top;
            break;
        case TextPositionMode::OnLine: {
            float midX = (basePosition.x + lineEnd.x) / 2.0f;
            float midY = (basePosition.y + lineEnd.y) / 2.0f;
            float lineAngle = std::atan2(lineEnd.y - basePosition.y, lineEnd.x - basePosition.x);
            float perpAngle = lineAngle - M_PI / 2.0f;
            float angleDeg = lineAngle * 180.0f / M_PI;
            if (angleDeg > 90.0f || angleDeg < -90.0f) {
                angleDeg += 180.0f;
            }
            finalPos = {midX + std::cos(perpAngle) * padding, midY + std::sin(perpAngle) * padding};
            finalAlign = TextPosition::Bottom;
            finalAngle = sf::degrees(angleDeg);
            break;
        }
        case TextPositionMode::TopLeft:
            finalPos = {basePosition.x - (objWidth / 2.0f) - padding, basePosition.y - (objHeight / 2.0f) - padding};
            finalAlign = TextPosition::BottomRight;
            break;
        case TextPositionMode::TopRight:
            finalPos = {basePosition.x + (objWidth / 2.0f) + padding, basePosition.y - (objHeight / 2.0f) - padding};
            finalAlign = TextPosition::BottomLeft;
            break;
        case TextPositionMode::BottomLeft:
            finalPos = {basePosition.x - (objWidth / 2.0f) - padding, basePosition.y + (objHeight / 2.0f) + padding};
            finalAlign = TextPosition::TopRight;
            break;
        case TextPositionMode::BottomRight:
            finalPos = {basePosition.x + (objWidth / 2.0f) + padding, basePosition.y + (objHeight / 2.0f) + padding};
            finalAlign = TextPosition::TopLeft;
            break;
    }

    drawText(finalPos, text, size, color, finalAlign, finalAngle);
}

// Convenience wrappers for common positioning modes
void Renderer::drawTextUp(sf::Vector2f center, float objHeight, float padding, const std::string& text, unsigned int size, sf::Color color) {
    drawTextPositioned(center, text, size, color, TextPositionMode::Up, objHeight, 0.0f, padding);
}

void Renderer::drawTextDown(sf::Vector2f center, float objHeight, float padding, const std::string& text, unsigned int size, sf::Color color) {
    drawTextPositioned(center, text, size, color, TextPositionMode::Down, objHeight, 0.0f, padding);
}

void Renderer::drawTextOnLine(sf::Vector2f p1, sf::Vector2f p2, float padding, const std::string& text, unsigned int size, sf::Color color) {
    drawTextPositioned(p1, text, size, color, TextPositionMode::OnLine, 0.0f, 0.0f, padding, p2);
}

void Renderer::drawTextTopLeft(sf::Vector2f center, sf::Vector2f objSize, float padding, const std::string& text, unsigned int size, sf::Color color) {
    drawTextPositioned(center, text, size, color, TextPositionMode::TopLeft, objSize.y, objSize.x, padding);
}

void Renderer::drawTextTopRight(sf::Vector2f center, sf::Vector2f objSize, float padding, const std::string& text, unsigned int size, sf::Color color) {
    drawTextPositioned(center, text, size, color, TextPositionMode::TopRight, objSize.y, objSize.x, padding);
}

void Renderer::drawTextBottomLeft(sf::Vector2f center, sf::Vector2f objSize, float padding, const std::string& text, unsigned int size, sf::Color color) {
    drawTextPositioned(center, text, size, color, TextPositionMode::BottomLeft, objSize.y, objSize.x, padding);
}

void Renderer::drawTextBottomRight(sf::Vector2f center, sf::Vector2f objSize, float padding, const std::string& text, unsigned int size, sf::Color color) {
    drawTextPositioned(center, text, size, color, TextPositionMode::BottomRight, objSize.y, objSize.x, padding);
}

void Renderer::resetCustomPositions() {
    customNodePositions.clear();
    positionHistory.clear();
    draggedNodeIndex = -1;
    hasMovedDuringDrag = false;
    defaultNodePositions.clear();
    preDragPositions.clear();
    currentChildren.clear();
}

void Renderer::undoLastDrag() {
    if (!positionHistory.empty()) {
        customNodePositions = positionHistory.back();
        positionHistory.pop_back();
    }
}
bool Renderer::handleMousePress(sf::Vector2f mousePos) {
    sf::Vector2f nodeSize = getNodeSize();
    float radius = std::max(nodeSize.x, nodeSize.y) / 2.0f;
    if (radius < 20.0f) radius = 35.0f; 

    for (const auto& pair : defaultNodePositions) {
        size_t id = pair.first;
        sf::Vector2f pos = pair.second;
        
        if (customNodePositions.find(id) != customNodePositions.end()) {
            pos = customNodePositions[id];
        }

        float dx = mousePos.x - pos.x;
        float dy = mousePos.y - pos.y;
        if (std::sqrt(dx * dx + dy * dy) <= radius) {
            draggedNodeIndex = id;
            dragOffset = pos - mousePos;
            preDragPositions = customNodePositions;
            hasMovedDuringDrag = false;
            return true;
        }
    }
    return false;
}
void Renderer::handleMouseMove(sf::Vector2f mousePos) {
    if (draggedNodeIndex != -1) {
        hasMovedDuringDrag = true;
        customNodePositions[draggedNodeIndex] = mousePos + dragOffset;
    }
}
bool Renderer::handleMouseRelease() {
    bool recorded = false;
    if (draggedNodeIndex != -1 && hasMovedDuringDrag) {
        positionHistory.push_back(preDragPositions);
        recorded = true;
    }
    hasMovedDuringDrag = false;
    draggedNodeIndex = -1;
    return recorded;
}

void Renderer::renderActiveState(const Frame* currentFrame) {
    if (!currentFrame || !currentFrame->getPayload()) return;
    
    currentFrame->getPayload()->accept(*this);
}

void Renderer::visit(const LinkedListPayload& payload) {
    // Basic implementation for linked list rendering (you can expand this later)
    if (payload.values.empty()) return;

    sf::Vector2f nodeSize = getNodeSize();
    float spacing = 50.0f * theme.nodeScale;
    float totalWidth = payload.values.size() * (nodeSize.x + spacing) - spacing;

    sf::Vector2u winSize = window.getWindow().getSize();
    // Reserve right-side space for the CodePanel so lists don't overlap it.
    float rightReserve = theme.codePanelWidth + theme.codePanelRightOffset + 16.0f;
    float startX = ((static_cast<float>(winSize.x) - rightReserve) - totalWidth) / 2.0f + nodeSize.x / 2.0f;
    float startY = winSize.y / 2.0f;

    currentChildren.clear();
    for (size_t i = 0; i + 1 < payload.values.size(); ++i) {
        currentChildren[i].push_back(i + 1); // Mỗi node nối với node tiếp theo
    }

    defaultNodePositions.clear();
    std::vector<sf::Vector2f> positions(payload.values.size());
    for (size_t i = 0; i < payload.values.size(); ++i) {
        sf::Vector2f calcPos = {startX + i * (nodeSize.x + spacing), startY};
        defaultNodePositions[i] = calcPos;

        auto customPosIt = customNodePositions.find(i);
        if (customPosIt != customNodePositions.end()) {
            positions[i] = customPosIt->second;
        } else {
            positions[i] = calcPos;
        }
    }

    // Draw edges
    for (size_t i = 0; i < payload.values.size() - 1; ++i) {
        drawLineWithArrow(positions[i], nodeSize, ShapeType::Circle, positions[i+1], nodeSize, ShapeType::Circle, 3.0f, 15.0f, false);
    }

    // Draw nodes
    for (size_t i = 0; i < payload.values.size(); ++i) {
        bool highlighted = std::find(payload.highlightedNodes.begin(), payload.highlightedNodes.end(), i) != payload.highlightedNodes.end();
        drawImageNode(positions[i], std::to_string(payload.values[i]), highlighted);
    }
}

void Renderer::visit(const TreePayload& payload) {
    const auto& nodes = payload.nodes; // This nodes only contains non-null nodes, which is different from old way, we store ~ 2^h-1 nodes
    const auto& highlights = payload.highlightedNodes;
    
    if (nodes.empty())
        return;
    
    sf::Vector2u winSize = window.getWindow().getSize();
    // Reserve right-side space for the CodePanel
    float rightReserve = theme.codePanelWidth + theme.codePanelRightOffset + 16.0f;
    // Since this is new way we need to find the actual height
    size_t maxId = 0;
    for (const auto& node : nodes) {
        if (node.id > maxId) maxId = node.id;
    }
    float virtualHeight = std::ceil(std::log2(maxId + 2)); 

    // Coordinate mapping instead of array
    // We use map since node id is not filled completely from 0 to final id
    std::map<size_t, sf::Vector2f> positions;
    sf::Vector2f nodeSize = getNodeSize();

    // Adapt with window.size() and shift center left to avoid CodePanel
    float startX = (static_cast<float>(winSize.x) - rightReserve) / 2.0f;
    float startY = static_cast<float>(winSize.y) * 0.15f;

    float distanceHorizontal = std::max(static_cast<float>(winSize.x) * 0.005f, nodeSize.x - 40.0f);
    float distanceVertical = std::max(static_cast<float>(winSize.y) * 0.095f, nodeSize.y + 40.0f);

    for (const auto& node : nodes) {
        size_t id = node.id;
        int level = std::floor(std::log2(id + 1));
        int posInLevel = id - (std::pow(2, level) - 1);
        
        float levelSpacing = distanceHorizontal * std::pow(2, virtualHeight - level);
        int nodesInLevel = std::pow(2, level);
        float levelWidth = levelSpacing * nodesInLevel;

        float currentX = startX - levelWidth / 2 + (posInLevel + 0.5f) * levelSpacing;
        float currentY = startY + level * distanceVertical;

        positions[id] = {currentX, currentY};
    }

    // Draw edges (with arrow) then draw nodes
    for (const auto& node : nodes) {
        // Lambda function 
        // As my understanding, this is "small" function that is just used for specific part and dont need using anywhere else
        auto drawEdgeIfExist = [&](size_t childId) {
            if (positions.count(childId)) {
                bool isHighlighted = std::find(highlights.begin(), highlights.end(), node.id) != highlights.end();
                bool isHighlightedChild = std::find(highlights.begin(), highlights.end(), childId) != highlights.end();
                
                // arrow: parent to child
                drawLineWithArrow(positions[node.id], nodeSize, ShapeType::Circle,
                                  positions[childId], nodeSize, ShapeType::Circle,
                                  3.0f, 12.0f, isHighlighted && isHighlightedChild);
            }
        };

        if (node.leftId != INVALID_INDEX) // != static_cast<size_t>(-1)
            drawEdgeIfExist(node.leftId);

        if (node.rightId != INVALID_INDEX)
            drawEdgeIfExist(node.rightId);
    } 

    for (const auto& node : nodes) {
        bool isHighlighted = std::find(highlights.begin(), highlights.end(), node.id) != highlights.end();
        drawImageNode(positions[node.id], std::to_string(node.value), isHighlighted);
    }
}

void Renderer::visit(const HeapPayload& payload) {
    const auto& heapArray = payload.arrayData;
    const auto& highlights = payload.highlightedIndices;
    
    if (heapArray.empty())
        return;
    
    sf::Vector2u winSize = window.getWindow().getSize();
    // Coordinates temp buffer
    std::vector<sf::Vector2f> positions(heapArray.size());
    sf::Vector2f nodeSize = getNodeSize();

        // Adapt with window.size() and shift center left to avoid CodePanel
        float rightReserve = theme.codePanelWidth + theme.codePanelRightOffset + 16.0f;
        float startX = (static_cast<float>(winSize.x) - rightReserve) / 2.0f;
    float startY = static_cast<float>(winSize.y) * 0.15f; 

    float distanceHorizontal = std::max(static_cast<float>(winSize.x) * 0.005f, nodeSize.x - 40.0f);
    float distanceVertical = std::max(static_cast<float>(winSize.y) * 0.095f, nodeSize.y + 40.0f);

    float height = std::ceil(log2(heapArray.size()+1));

    for (size_t idx = 0; idx < heapArray.size(); idx++) {
        // Coordinates calculation
        int level = std::floor(log2(idx + 1));
        int posInLevel = idx - (std::pow(2, level) - 1);
        float levelSpacing = distanceHorizontal * std::pow(2, height - level);
        int nodesInLevel = std::pow(2, level);
        float levelWidth = levelSpacing * nodesInLevel;

        float currentX = startX - levelWidth / 2 + (posInLevel + 0.5f) * levelSpacing;
        float currentY = startY + level * distanceVertical;

        size_t left = 2 * idx + 1;
        size_t right = 2 * idx + 2;
        if (left < heapArray.size()) currentChildren[idx].push_back(left);
        if (right < heapArray.size()) currentChildren[idx].push_back(right);

        sf::Vector2f calcPos = {currentX, currentY};
        defaultNodePositions[idx] = calcPos;

        auto customPosIt = customNodePositions.find(idx);
        if (customPosIt != customNodePositions.end()) {
            positions[idx] = customPosIt->second;
        } else {
            positions[idx] = calcPos;
        }
    }

    // Draw edges then draw nodes
    for (size_t idx = 1; idx < heapArray.size(); ++idx) {
        size_t parentIdx = (idx - 1) / 2;

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
        bool isHighlighted = std::find(highlights.begin(), highlights.end(), idx) != highlights.end();
        drawImageNode(positions[idx], std::to_string(heapArray[idx]), isHighlighted);
    }
}

void Renderer::visit(const GraphPayload& payload) {
    const auto& vertices = payload.vertices;
    const auto& edges = payload.edges;
    if (vertices.empty()) return;

    sf::Vector2f nodeSize = getNodeSize();
    const auto& hEdges = payload.highlightedEdges;
    const auto& hIndices = payload.highlightedVertices;

    sf::Vector2u winSize = window.getWindow().getSize();
    // Reserve right-side space for CodePanel so graph centers don't overlap it
    float rightReserve = theme.codePanelWidth + theme.codePanelRightOffset + 16.0f;
    float cx = (static_cast<float>(winSize.x) - rightReserve) / 2.0f;
    float cy = winSize.y / 2.0f - 50.0f; 
    
    float radius = std::min(cx, cy) - 100.0f;
    if (radius < 50.0f) radius = 50.0f;

    currentChildren.clear(); 
    defaultNodePositions.clear();
    std::map<size_t, sf::Vector2f> autoPositionsByVertexId;
    for (size_t i = 0; i < vertices.size(); ++i) {
        size_t vertexId = vertices[i];
        sf::Vector2f position;

        if (payload.positions.size() == vertices.size()) {
            position = {payload.positions[i].x, payload.positions[i].y};
        } else {
            if (vertices.size() == 1) {
                position = {cx, cy};
            } else {
                float angle = i * (2.0f * M_PI / vertices.size()) - M_PI / 2.0f;
                position = {cx + radius * std::cos(angle), cy + radius * std::sin(angle)};
            }
        }

        autoPositionsByVertexId[vertexId] = position;
    }

    if (!autoPositionsByVertexId.empty()) {
        auto firstIt = autoPositionsByVertexId.begin();
        float minX = firstIt->second.x;
        float maxX = firstIt->second.x;
        float minY = firstIt->second.y;
        float maxY = firstIt->second.y;

        for (const auto& [_, pos] : autoPositionsByVertexId) {
            minX = std::min(minX, pos.x);
            maxX = std::max(maxX, pos.x);
            minY = std::min(minY, pos.y);
            maxY = std::max(maxY, pos.y);
        }

        const float bboxW = std::max(1.0f, maxX - minX);
        const float bboxH = std::max(1.0f, maxY - minY);
        const float margin = std::max(nodeSize.x, nodeSize.y) * 0.75f + 20.0f;
        const float availableW = std::max(40.0f, static_cast<float>(winSize.x) - rightReserve - 2.0f * margin);
        const float availableH = std::max(40.0f, static_cast<float>(winSize.y) - 2.0f * margin);

        const float fitScale = std::min(1.0f, std::min(availableW / bboxW, availableH / bboxH));

        const sf::Vector2f sourceCenter = {(minX + maxX) / 2.0f, (minY + maxY) / 2.0f};
        const sf::Vector2f targetCenter = {cx, cy};

        for (auto& [_, pos] : autoPositionsByVertexId) {
            pos = targetCenter + (pos - sourceCenter) * fitScale;
            pos.x = std::clamp(pos.x, margin, static_cast<float>(winSize.x) - margin - rightReserve);
            pos.y = std::clamp(pos.y, margin, static_cast<float>(winSize.y) - margin);
        }
    }

    defaultNodePositions = autoPositionsByVertexId;
    std::map<size_t, sf::Vector2f> positionsByVertexId = autoPositionsByVertexId;
    for (const auto& [vertexId, customPos] : customNodePositions) {
        auto it = positionsByVertexId.find(vertexId);
        if (it != positionsByVertexId.end()) {
            it->second = customPos;
        }
    }

    std::vector<std::pair<std::pair<size_t, size_t>, int>> drawnTexts;
    for (const auto& edge : edges) {
        bool highlighted = false;
        for (const auto& he : hEdges) {
            if (he.from == edge.from && he.to == edge.to) {
                highlighted = true;
                break;
            }
        }
        
            bool hasReverse = false;
            for (const auto& e : edges) {
                if (e.from == edge.to && e.to == edge.from) {
                    hasReverse = true;
                    break;
                }
            }
            
            float edgeOffset = hasReverse ? -12.0f : 0.0f;

            auto fromPosIt = positionsByVertexId.find(edge.from);
            auto toPosIt = positionsByVertexId.find(edge.to);
            if (fromPosIt != positionsByVertexId.end() && toPosIt != positionsByVertexId.end()) {
                sf::Vector2f p1 = fromPosIt->second;
                sf::Vector2f p2 = toPosIt->second;
                
                drawLineWithArrow(p1, nodeSize, ShapeType::Circle, p2, nodeSize, ShapeType::Circle, 3.0f, 15.0f, highlighted, edgeOffset);
                
                bool textAlreadyDrawn = false;
                for (const auto& dt : drawnTexts) {
                    if ((dt.first.first == edge.from && dt.first.second == edge.to) ||
                        (!hasReverse && dt.first.first == edge.to && dt.first.second == edge.from && dt.second == edge.weight)) {
                        textAlreadyDrawn = true;
                        break;
                    }
                }
                
                if (!textAlreadyDrawn) {
                    bool textHighlighted = highlighted;
                    if (!textHighlighted && !hasReverse) {
                        for (const auto& he : hEdges) {
                            if (he.from == edge.to && he.to == edge.from) {
                                textHighlighted = true;
                                break;
                            }
                        }
                    }

                    sf::Vector2f perp(0.0f, 0.0f);
                    if (hasReverse) {
                        float dx = p2.x - p1.x;
                        float dy = p2.y - p1.y;
                        float dist = std::sqrt(dx * dx + dy * dy);
                        if (dist > 0.0001f) {
                            perp = sf::Vector2f(-dy / dist, dx / dist);
                        }
                    }
                    
                    sf::Vector2f tP1 = p1 + perp * edgeOffset;
                    sf::Vector2f tP2 = p2 + perp * edgeOffset;

                    drawTextOnLine(tP1, tP2, 12.0f, std::to_string(edge.weight), 16, textHighlighted ? theme.accentColor : theme.textColor);
                    drawnTexts.push_back({{edge.from, edge.to}, edge.weight});
                }
            }
    }

    for (size_t i = 0; i < vertices.size(); ++i) {
        size_t vertexId = vertices[i];
        bool highlighted = std::find(hIndices.begin(), hIndices.end(), vertexId) != hIndices.end();
        drawImageNode(positionsByVertexId[vertexId], std::to_string(vertexId), highlighted);
    }
}

void Renderer::visit(const SingleSourcePayload& payload) {
    visit(payload.baseGraph);
    
    sf::Vector2f nodeSize = getNodeSize();
    for (size_t i = 0; i < payload.baseGraph.vertices.size(); ++i) {
        size_t vertexId = payload.baseGraph.vertices[i];
        auto posIt = defaultNodePositions.find(vertexId);
        if (posIt == defaultNodePositions.end()) {
            continue;
        }

        sf::Vector2f pos = posIt->second;
        auto customPosIt = customNodePositions.find(vertexId);
        if (customPosIt != customNodePositions.end()) {
            pos = customPosIt->second;
        }

        if (vertexId < payload.distances.size()) {
            std::string distStr = (payload.distances[vertexId] >= 1e9) ? "INF" : std::to_string(payload.distances[vertexId]);
            drawTextPositioned(pos, "d: " + distStr, 14, theme.textColor, TextPositionMode::TopRight, nodeSize.y, nodeSize.x, 2.0f);
        }
    }
}

void Renderer::visit(const AStarPayload& payload) {
    visit(payload.baseGraph);
    
    sf::Vector2f nodeSize = getNodeSize();
    for (size_t i = 0; i < payload.baseGraph.vertices.size(); ++i) {
        size_t vertexId = payload.baseGraph.vertices[i];
        auto posIt = defaultNodePositions.find(vertexId);
        if (posIt == defaultNodePositions.end()) {
            continue;
        }

        sf::Vector2f pos = posIt->second;
        auto customPosIt = customNodePositions.find(vertexId);
        if (customPosIt != customNodePositions.end()) {
            pos = customPosIt->second;
        }

        if (vertexId < payload.fCosts.size()) {
            std::string fStr = (payload.fCosts[vertexId] >= 1e9) ? "INF" : std::to_string(payload.fCosts[vertexId]);
            std::string gStr = (payload.gCosts[vertexId] >= 1e9) ? "INF" : std::to_string(payload.gCosts[vertexId]);
            std::string hStr = (payload.hCosts[vertexId] >= 1e9) ? "INF" : std::to_string(payload.hCosts[vertexId]);
            
            std::string text = "f:" + fStr + "\ng:" + gStr + "\nh:" + hStr;
            drawTextPositioned(pos, text, 12, theme.textColor, TextPositionMode::TopRight, nodeSize.y, nodeSize.x, 2.0f);
        }
    }
}

void Renderer::visit(const AllPairsPayload& payload) {
    visit(payload.baseGraph);
    
    // Draw a small distance matrix if size is manageable
    if (payload.distances.size() <= 10 && !payload.distances.empty()) {
        float startX = 20.0f;
        float startY = 100.0f;
        float cellW = 40.0f;
        float cellH = 30.0f;
        
        for (size_t i = 0; i < payload.distances.size(); ++i) {
            for (size_t j = 0; j < payload.distances[i].size(); ++j) {
                std::string distStr = (payload.distances[i][j] >= 1e9) ? "INF" : std::to_string(payload.distances[i][j]);
                sf::Vector2f pos(startX + j * cellW, startY + i * cellH);
                drawText(pos, distStr, 14, theme.textColor, TextPosition::TopLeft);
            }
        }
    }
}

void Renderer::visit(const GridPayload& payload) {
    const auto& grid = payload.gridData;
    if (grid.empty() || grid[0].empty()) return;

    size_t rows = grid.size();
    size_t cols = grid[0].size();

    sf::Vector2u winSize = window.getWindow().getSize();
    LayoutConfig layoutDefaults{};
    float horizontalPadding = std::max(
        layoutDefaults.gridHorizontalPaddingMin,
        static_cast<float>(winSize.x) * layoutDefaults.gridHorizontalPaddingRatio);
    float topPadding = std::max(
        layoutDefaults.gridTopPaddingMin,
        static_cast<float>(winSize.y) * layoutDefaults.gridTopPaddingRatio);
    float bottomPadding = std::max(
        layoutDefaults.gridBottomPaddingMin,
        static_cast<float>(winSize.y) * layoutDefaults.gridBottomPaddingRatio);

    float rightReserve = theme.codePanelWidth + theme.codePanelRightOffset + 16.0f;
    float availableWidth = std::max(layoutDefaults.gridViewportMinSize, static_cast<float>(winSize.x) - rightReserve - 2.0f * horizontalPadding);
    float availableHeight = std::max(layoutDefaults.gridViewportMinSize, static_cast<float>(winSize.y) - topPadding - bottomPadding);

    float cellSize = std::min(availableWidth / cols, availableHeight / rows);
    float gridWidth = cols * cellSize;
    float gridHeight = rows * cellSize;

    float startX = ((static_cast<float>(winSize.x) - rightReserve) - gridWidth) / 2.0f;
    float startY = topPadding + (availableHeight - gridHeight) / 2.0f;

    for (size_t r = 0; r < rows; ++r) {
        for (size_t c = 0; c < cols; ++c) {
            sf::RectangleShape cell(sf::Vector2f(cellSize - 2.0f, cellSize - 2.0f));
            cell.setPosition({startX + c * cellSize + 1.0f, startY + r * cellSize + 1.0f});

            int state = grid[r][c];
            if (state == 0) { // Empty
                cell.setFillColor(sf::Color(200, 200, 200));
            } else if (state == 1) { // Wall
                cell.setFillColor(sf::Color(50, 50, 50));
            } else if (state == 2) { // Start
                cell.setFillColor(sf::Color::Green);
            } else if (state == 3) { // Target
                cell.setFillColor(sf::Color::Red);
            } else if (state == 4) { // Visited
                cell.setFillColor(sf::Color::Cyan);
            } else if (state == 5) { // Path
                cell.setFillColor(sf::Color::Yellow);
            }

            if (payload.currentCell.first == r && payload.currentCell.second == c) {
                cell.setOutlineThickness(3.0f);
                cell.setOutlineColor(sf::Color::Magenta);
            }

            window.getWindow().draw(cell);
        }
    }
}
