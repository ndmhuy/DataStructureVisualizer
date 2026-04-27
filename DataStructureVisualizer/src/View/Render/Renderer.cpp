#include "View/Render/Renderer.h"
#include "Model/Frame.h"
#include "Utilities/GlobalConstant.h"

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

void Renderer::reloadBackground() {
    if (bgTexture.loadFromFile(theme.bgImagePath)) {
        bgTexture.setSmooth(true);
        bgSprite.setTexture(bgTexture, true);
        sf::Vector2u texSize = bgTexture.getSize();
        sf::Vector2u winSize = window.getWindow().getSize();
        bgSprite.setScale({static_cast<float>(winSize.x) / texSize.x, static_cast<float>(winSize.y) / texSize.y});
    }
}

void Renderer::drawBackground() {
    sf::View currentView = window.getWindow().getView();
    window.getWindow().setView(window.getWindow().getDefaultView());
    
    bgSprite.setColor(theme.bgTintColor);
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
    bool isHighlighted) {
    float dx = p2.x - p1.x;
    float dy = p2.y - p1.y;
    float distance = std::sqrt(dx * dx + dy * dy);

    float angle = std::atan2(dy, dx);
    float angleDegrees = angle * 180.0f / M_PI;

    sf::Vector2f start = getBoundaryPoint(p1, size1, angle, type1);
    
    sf::Vector2f end = getBoundaryPoint(p2, size2, angle + M_PI, type2);

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

void Renderer::drawLine(sf::Vector2f p1, sf::Vector2f size1, ShapeType type1, sf::Vector2f p2, sf::Vector2f size2, ShapeType type2, float thickness, bool isHighlighted) {
    float dx = p2.x - p1.x;
    float dy = p2.y - p1.y;
    float distance = std::sqrt(dx * dx + dy * dy);

    float angle = std::atan2(dy, dx);
    float angleDegrees = angle * 180.0f / M_PI;

    sf::Vector2f start = getBoundaryPoint(p1, size1, angle, type1);
    
    sf::Vector2f end = getBoundaryPoint(p2, size2, angle + M_PI, type2);

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
    float startX = (winSize.x - totalWidth) / 2.0f + nodeSize.x / 2.0f;
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
    // Since this is new way we need to find the actual height
    size_t maxId = 0;
    for (const auto& node : nodes) {
        if (node.id > maxId) maxId = node.id;
    }
    float virtualHeight = std::ceil(std::log2(maxId + 2)); 

    // Coordinate mapping instead of array
    // We use map since node id is not filled completely from 0 to final id
    std::map<size_t, sf::Vector2f> positions;

    // Adapt with window.size()
    float startX = static_cast<float>(winSize.x) / 2.0f;
    float startY = static_cast<float>(winSize.y) * 0.15f;
    float distanceHorizontal = static_cast<float>(winSize.x) * 0.022f;
    float distanceVertical = static_cast<float>(winSize.y) * 0.078f;

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
    sf::Vector2f nodeSize = getNodeSize();
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

    // Adapt with window.size()
    float startX = static_cast<float>(winSize.x) / 2.0f;
    float startY = static_cast<float>(winSize.y) * 0.15f; 
    float distanceHorizontal = static_cast<float>(winSize.x) * 0.022f;
    float distanceVertical = static_cast<float>(winSize.y) * 0.078f;
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
    sf::Vector2f nodeSize = getNodeSize();
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
    float cx = winSize.x / 2.0f;
    float cy = winSize.y / 2.0f - 50.0f; 
    
    float radius = std::min(cx, cy) - 100.0f;
    if (radius < 50.0f) radius = 50.0f;

    currentChildren.clear(); 
    defaultNodePositions.clear();
    std::vector<sf::Vector2f> positions(vertices.size());
    for (size_t i = 0; i < vertices.size(); ++i) {
        if (payload.positions.size() == vertices.size()) {
            positions[i] = {payload.positions[i].x, payload.positions[i].y};
        } else {
            if (vertices.size() == 1) {
                positions[i] = {cx, cy};
            } else {
                float angle = i * (2.0f * M_PI / vertices.size()) - M_PI / 2.0f;
                positions[i] = {cx + radius * std::cos(angle), cy + radius * std::sin(angle)};
            }
        }

        defaultNodePositions[i] = positions[i];
        auto customPosIt = customNodePositions.find(i);
        if (customPosIt != customNodePositions.end()) {
            positions[i] = customPosIt->second;
        }
    }

    for (const auto& edge : edges) {
        bool highlighted = false;
        for (const auto& he : hEdges) {
            if (he.from == edge.from && he.to == edge.to) {
                highlighted = true;
                break;
            }
        }
        if (edge.from < vertices.size() && edge.to < vertices.size()) {
            drawLineWithArrow(positions[edge.from], nodeSize, ShapeType::Circle, positions[edge.to], nodeSize, ShapeType::Circle, 3.0f, 15.0f, highlighted);
        }
    }

    for (size_t i = 0; i < vertices.size(); ++i) {
        bool highlighted = std::find(hIndices.begin(), hIndices.end(), i) != hIndices.end();
        drawImageNode(positions[i], std::to_string(vertices[i]), highlighted);
    }
}

void Renderer::visit(const SingleSourcePayload& payload) {
    visit(payload.baseGraph);
    
    sf::Vector2f nodeSize = getNodeSize();
    for (size_t i = 0; i < payload.distances.size(); ++i) {
        if (i < defaultNodePositions.size()) {
            std::string distStr = (payload.distances[i] >= 1e9) ? "INF" : std::to_string(payload.distances[i]);
            drawTextPositioned(defaultNodePositions[i], "d: " + distStr, 14, theme.textColor, TextPositionMode::TopRight, nodeSize.y, nodeSize.x, 2.0f);
        }
    }
}

void Renderer::visit(const AStarPayload& payload) {
    visit(payload.baseGraph);
    
    sf::Vector2f nodeSize = getNodeSize();
    for (size_t i = 0; i < payload.fCosts.size(); ++i) {
        if (i < defaultNodePositions.size()) {
            std::string fStr = (payload.fCosts[i] >= 1e9) ? "INF" : std::to_string(payload.fCosts[i]);
            std::string gStr = (payload.gCosts[i] >= 1e9) ? "INF" : std::to_string(payload.gCosts[i]);
            std::string hStr = (payload.hCosts[i] >= 1e9) ? "INF" : std::to_string(payload.hCosts[i]);
            
            std::string text = "f:" + fStr + "\ng:" + gStr + "\nh:" + hStr;
            drawTextPositioned(defaultNodePositions[i], text, 12, theme.textColor, TextPositionMode::TopRight, nodeSize.y, nodeSize.x, 2.0f);
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
    float availableWidth = winSize.x * 0.8f;
    float availableHeight = winSize.y * 0.8f;

    float cellSize = std::min(availableWidth / cols, availableHeight / rows);
    float gridWidth = cols * cellSize;
    float gridHeight = rows * cellSize;

    float startX = (winSize.x - gridWidth) / 2.0f;
    float startY = (winSize.y - gridHeight) / 2.0f;

    for (size_t r = 0; r < rows; ++r) {
        for (size_t c = 0; c < cols; ++c) {
            sf::RectangleShape cell(sf::Vector2f(cellSize - 2.0f, cellSize - 2.0f));
            cell.setPosition({startX + c * cellSize + 1.0f, startY + r * cellSize + 1.0f});

            int state = grid[r][c];
            bool isDark = theme.codePanelBackgroundColor.r < 100; // Nhận diện Dark Mode dựa vào màu theme
            
            if (state == 0) { // Empty
                cell.setFillColor(isDark ? sf::Color(60, 65, 75, 180) : sf::Color(220, 220, 230, 180));
            } else if (state == 1) { // Wall
                cell.setFillColor(isDark ? sf::Color(200, 210, 220) : sf::Color(50, 50, 50));
            } else if (state == 2) { // Start
                cell.setFillColor(sf::Color(50, 200, 100)); // Xanh lá dịu
            } else if (state == 3) { // Target
                cell.setFillColor(sf::Color(230, 80, 80)); // Đỏ dịu
            } else if (state == 4) { // Visited
                cell.setFillColor(isDark ? sf::Color(50, 120, 180, 200) : sf::Color(100, 200, 255, 200));
            } else if (state == 5) { // Path
                cell.setFillColor(sf::Color(255, 200, 50)); // Vàng cam
            }

            if (payload.currentCell.first == r && payload.currentCell.second == c) {
                cell.setOutlineThickness(3.0f);
                cell.setOutlineColor(theme.highlightColor); // Dùng màu highlight của Theme cho đồng bộ
            }

            window.getWindow().draw(cell);
        }
    }
}

void Renderer::visit(const MenuAnimPayload& payload) {
    float btnWidth = 350.0f;
    float btnHeight = 80.0f;
    float spacingX = 40.0f;
    float spacingY = 120.0f;
    int cols = 2;
    
    int totalItems = 0;
    if (payload.menuState == 0) totalItems = 4;
    else if (payload.menuState == 1) totalItems = 2;
    else if (payload.menuState == 2) totalItems = 2;
    else if (payload.menuState == 3) totalItems = 2;
    
    int rows = (totalItems + cols - 1) / cols;
    float startY = payload.winSize.y * 0.48f;
    
    float time = payload.time;
    
    sf::View currentView = window.getWindow().getView();
    window.getWindow().setView(sf::View(sf::FloatRect({0.f, 0.f}, {payload.winSize.x, payload.winSize.y})));

    // Hàm helper tạo màu có độ trong suốt (Alpha)
    auto fade = [](sf::Color c, std::uint8_t alpha) {
        return sf::Color(c.r, c.g, c.b, alpha);
    };

    sf::Color primary = theme.inputMenuPrimaryColor;
    sf::Color accent = theme.inputMenuAccentColor;
    sf::Color highlight = theme.highlightColor;
    bool isDark = theme.codePanelBackgroundColor.r < 100;
    sf::Color silverCol = sf::Color(200, 210, 220);

    for (int i = 0; i < totalItems; ++i) {
        int row = i / cols;
        int col = i % cols;
        int itemsInThisRow = (row == rows - 1 && totalItems % cols != 0) ? (totalItems % cols) : cols;
        float startX = (payload.winSize.x - (itemsInThisRow * btnWidth + (itemsInThisRow - 1) * spacingX)) * 0.5f;
        
        float cx = startX + col * (btnWidth + spacingX) + btnWidth / 2.0f;
        float cy = startY + row * (btnHeight + spacingY) - 60.0f; // Vẽ lơ lửng trên hộp 60px
        
        if (payload.menuState == 0) { // Main Menu
            if (i == 0) {
                // Cyber-Pulse Linked List
                int numNodes = 3;
                float nodeSpacing = 70.0f;
                float cycle = std::fmod(time * 1.5f, numNodes);
                int activeIdx = static_cast<int>(cycle);
                float progress = cycle - activeIdx;

                std::vector<sf::Vector2f> listNodes(numNodes);
                for (int j = 0; j < numNodes; ++j) {
                    float offsetY = std::sin(time * 3.0f + j * 1.0f) * 10.0f;
                    listNodes[j] = {cx + (j - 1) * nodeSpacing, cy + offsetY};
                }

                for (int j = 0; j < numNodes - 1; ++j) {
                    sf::Vector2f start = listNodes[j] + sf::Vector2f(20.0f, 0.0f);
                    sf::Vector2f end = listNodes[j+1] - sf::Vector2f(20.0f, 0.0f);

                    sf::VertexArray link(sf::PrimitiveType::Lines, 2);
                    link[0].position = start; link[0].color = fade(primary, 150);
                    link[1].position = end;   link[1].color = fade(accent, 150);
                    window.getWindow().draw(link);

                    if (j == activeIdx) {
                        sf::Vector2f packetPos = start + (end - start) * progress;
                        sf::CircleShape glow(6.0f);
                        glow.setOrigin({6.0f, 6.0f}); glow.setPosition(packetPos);
                        glow.setFillColor(fade(highlight, 100));
                        window.getWindow().draw(glow);

                        sf::CircleShape packet(3.0f);
                        packet.setOrigin({3.0f, 3.0f}); packet.setPosition(packetPos);
                        packet.setFillColor(fade(highlight, 255));
                        window.getWindow().draw(packet);
                    }
                }

                for (int j = 0; j < numNodes; ++j) {
                    bool isActive = (j == activeIdx);
                    sf::RectangleShape block({40.0f, 20.0f});
                    block.setOrigin({20.0f, 10.0f});
                    block.setPosition(listNodes[j]);
                    block.setFillColor(isActive ? fade(primary, 200) : fade(primary, 80));
                    block.setOutlineThickness(1.5f);
                    block.setOutlineColor(isActive ? fade(highlight, 220) : fade(accent, 150));
                    window.getWindow().draw(block);

                    sf::RectangleShape divider({1.5f, 20.0f});
                    divider.setOrigin({0.75f, 10.0f});
                    divider.setPosition({listNodes[j].x + 4.0f, listNodes[j].y});
                    divider.setFillColor(fade(accent, 150));
                    window.getWindow().draw(divider);
                }
            } else if (i == 1) {
                // Diamond Tree (Heap)
                std::vector<sf::Vector2f> treeNodes = {
                    {cx, cy - 20.0f + std::sin(time)*8.f},
                    {cx - 35.0f, cy + 20.0f + std::sin(time+1.f)*8.f},
                    {cx + 35.0f, cy + 20.0f + std::sin(time+2.f)*8.f}
                };
                std::vector<std::pair<int, int>> edges = {{0,1}, {0,2}};

                for (auto edge : edges) {
                    sf::VertexArray line(sf::PrimitiveType::Lines, 2);
                    line[0].position = treeNodes[edge.first];  line[0].color = fade(highlight, 180);
                    line[1].position = treeNodes[edge.second]; line[1].color = fade(primary, 80);
                    window.getWindow().draw(line);
                }

                for (size_t j = 0; j < treeNodes.size(); ++j) {
                    sf::CircleShape diamond(14.0f, 4);
                    diamond.setOrigin({14.0f, 14.0f});
                    diamond.setPosition(treeNodes[j]);
                    float scale = (j == 0) ? 1.0f + std::sin(time * 5.0f) * 0.15f : 1.0f;
                    diamond.setScale({scale, scale});
                    diamond.setFillColor(fade(highlight, 100));
                    diamond.setOutlineThickness(2.0f);
                    diamond.setOutlineColor(fade(highlight, 220));
                    window.getWindow().draw(diamond);
                }
            } else if (i == 2) {
                // AVL Tree (Rotating / Balancing)
                float rot = std::sin(time * 2.0f) * 0.3f; // Swaying motion
                sf::Vector2f root = {cx, cy - 20.f};
                sf::Vector2f l = {cx - 35.f * std::cos(rot) + 25.f * std::sin(rot), cy + 20.f * std::cos(rot) + 35.f * std::sin(rot)};
                sf::Vector2f r = {cx + 35.f * std::cos(rot) - 25.f * std::sin(rot), cy + 20.f * std::cos(rot) - 35.f * std::sin(rot)};

                sf::VertexArray lines(sf::PrimitiveType::Lines, 4);
                lines[0].position = root; lines[0].color = fade(accent, 200);
                lines[1].position = l;    lines[1].color = fade(primary, 100);
                lines[2].position = root; lines[2].color = fade(accent, 200);
                lines[3].position = r;    lines[3].color = fade(primary, 100);
                window.getWindow().draw(lines);

                sf::Vector2f pts[3] = {root, l, r};
                for(int j=0; j<3; ++j) {
                    sf::CircleShape circle(12.0f);
                    circle.setOrigin({12.0f, 12.0f});
                    circle.setPosition(pts[j]);
                    circle.setFillColor(fade(primary, 150));
                    circle.setOutlineThickness(2.0f);
                    circle.setOutlineColor(fade(accent, 255));
                    window.getWindow().draw(circle);
                    
                    // Orbital dot
                    sf::CircleShape orb(3.0f);
                    orb.setOrigin({3.0f, 3.0f});
                    orb.setPosition({pts[j].x + 18.0f * std::cos(time * 4.0f + j), pts[j].y + 18.0f * std::sin(time * 4.0f + j)});
                    orb.setFillColor(fade(highlight, 200));
                    window.getWindow().draw(orb);
                }
            } else if (i == 3) {
                // Shortest Path (Grid Wave)
                int gRows = 3, gCols = 5;
                float gSpace = 16.0f;
                float ox = cx - (gCols - 1) * gSpace * 0.5f;
                float oy = cy - (gRows - 1) * gSpace * 0.5f;

                for (int r = 0; r < gRows; ++r) {
                    for (int c = 0; c < gCols; ++c) {
                        float px = ox + c * gSpace;
                        float py = oy + r * gSpace;
                        float dist = r + c;
                        float wave = std::sin(time * 5.0f - dist * 0.8f);

                        sf::RectangleShape cell({5.0f, 5.0f});
                        cell.setOrigin({2.5f, 2.5f});
                        cell.setPosition({px, py});

                        if (wave > 0.6f) {
                            cell.setFillColor(fade(accent, 200));
                            cell.setScale({1.5f, 1.5f});
                        } else {
                        cell.setFillColor(isDark ? fade(silverCol, 150) : fade(primary, 80));
                        }
                        window.getWindow().draw(cell);
                    }
                }
                
                // Simple path
                std::vector<sf::Vector2f> path = {{ox, oy}, {ox + 2*gSpace, oy}, {ox + 2*gSpace, oy + 2*gSpace}, {ox + 4*gSpace, oy + 2*gSpace}};
                for (size_t k = 0; k < path.size() - 1; ++k) {
                    sf::VertexArray line(sf::PrimitiveType::Lines, 2);
                    std::uint8_t alpha = 100 + static_cast<std::uint8_t>(std::abs(std::sin(time * 4.0f)) * 155);
                    line[0].position = path[k]; line[0].color = fade(highlight, alpha);
                    line[1].position = path[k+1]; line[1].color = fade(highlight, alpha);
                    window.getWindow().draw(line);
                }
            }
        } else if (payload.menuState == 1) {
            // 3D Array
            for (int j = -1; j <= 1; ++j) {
                float offset = std::sin(time * 3.0f + j * 0.5f) * 8.0f;
                float dir = (i == 0) ? 1.0f : -1.0f; // Min heap goes up, Max heap goes down slightly

                sf::RectangleShape cell({36.0f, 36.0f});
                cell.setOrigin({18.0f, 18.0f});
                cell.setPosition({cx + j * 40.0f, cy + offset * dir});
                cell.setFillColor(fade(primary, 100));
                cell.setOutlineThickness(2.0f);
                cell.setOutlineColor(fade(accent, 180));
                window.getWindow().draw(cell);

                sf::RectangleShape inner({28.0f, 28.0f});
                inner.setOrigin({14.0f, 14.0f});
                inner.setPosition({cx + j * 40.0f, cy + offset * dir});
                inner.setFillColor(fade(accent, 50));
                window.getWindow().draw(inner);
            }
        } else if (payload.menuState == 2) {
            if (i == 0) {
                // Grid Wave
                int gRows = 4, gCols = 4;
                float gSpace = 18.0f;
                float ox = cx - (gCols - 1) * gSpace * 0.5f;
                float oy = cy - (gRows - 1) * gSpace * 0.5f;

                for (int r = 0; r < gRows; ++r) {
                    for (int c = 0; c < gCols; ++c) {
                        float px = ox + c * gSpace;
                        float py = oy + r * gSpace;
                        float dist = r + c;
                        float wave = std::sin(time * 5.0f - dist * 0.8f);

                        sf::RectangleShape cell({8.0f, 8.0f});
                        cell.setOrigin({4.0f, 4.0f});
                        cell.setPosition({px, py});

                        if (wave > 0.6f) {
                            cell.setFillColor(fade(highlight, 200));
                            cell.setScale({1.5f, 1.5f});
                        } else {
                        cell.setFillColor(isDark ? fade(silverCol, 150) : fade(primary, 80));
                        }
                        window.getWindow().draw(cell);
                    }
                }
            } else if (i == 1) {
                // Hexagon Graph
                std::vector<sf::Vector2f> hexNodes = {
                    {cx, cy - 30.0f + std::sin(time)*5.f},
                    {cx + 30.0f, cy - 10.0f + std::cos(time)*5.f},
                    {cx + 20.0f, cy + 25.0f + std::sin(time*1.2f)*5.f},
                    {cx - 20.0f, cy + 25.0f + std::cos(time*0.8f)*5.f},
                    {cx - 30.0f, cy - 10.0f + std::sin(time*1.5f)*5.f}
                };

                for (size_t j = 0; j < hexNodes.size(); ++j) {
                    size_t next = (j + 1) % hexNodes.size();
                    size_t cross = (j + 2) % hexNodes.size();

                    sf::VertexArray line(sf::PrimitiveType::Lines, 2);
                    line[0].position = hexNodes[j];  line[0].color = fade(accent, 150);
                    line[1].position = hexNodes[next]; line[1].color = isDark ? fade(silverCol, 80) : fade(primary, 50);
                    window.getWindow().draw(line);

                    sf::VertexArray crossLine(sf::PrimitiveType::Lines, 2);
                    crossLine[0].position = hexNodes[j];  crossLine[0].color = isDark ? fade(silverCol, 80) : fade(primary, 80);
                    crossLine[1].position = hexNodes[cross]; crossLine[1].color = fade(highlight, 40);
                    window.getWindow().draw(crossLine);
                }

                for (const auto& pos : hexNodes) {
                    sf::CircleShape hex(12.0f, 6);
                    hex.setOrigin({12.0f, 12.0f});
                    hex.setPosition(pos);
                    hex.setFillColor(isDark ? fade(silverCol, 60) : fade(primary, 120));
                    hex.setOutlineThickness(1.5f);
                    hex.setOutlineColor(fade(accent, 220));
                    hex.setRotation(sf::degrees(time * 40.0f));
                    window.getWindow().draw(hex);
                }
            }
        } else if (payload.menuState == 3) {
            if (i == 0) {
                // Adj Matrix (Glowing Data Grid)
                int mSize = 4;
                float mSpace = 16.0f;
                float ox = cx - (mSize - 1) * mSpace * 0.5f;
                float oy = cy - (mSize - 1) * mSpace * 0.5f;
                
                for(int r = 0; r < mSize; ++r) {
                    for(int c = 0; c < mSize; ++c) {
                        sf::RectangleShape cell({12.0f, 12.0f});
                        cell.setOrigin({6.0f, 6.0f});
                        cell.setPosition({ox + c * mSpace, oy + r * mSpace});
                        
                        float blink = std::sin(time * 6.0f + r * 2.0f + c * 3.0f);
                        if (r == c) {
                            cell.setFillColor(fade(highlight, 180));
                        } else if (blink > 0.5f) {
                            cell.setFillColor(fade(accent, 150));
                        } else {
                        cell.setFillColor(isDark ? fade(silverCol, 50) : fade(primary, 60));
                        cell.setOutlineThickness(1.0f);
                        cell.setOutlineColor(isDark ? fade(silverCol, 120) : fade(primary, 100));
                        }
                        window.getWindow().draw(cell);
                    }
                }
            } else if (i == 1) {
                // Adj List (Central radiating nodes)
                sf::Vector2f root = {cx - 20.0f, cy};
                std::vector<sf::Vector2f> children = {
                    {cx + 20.0f, cy - 25.0f},
                    {cx + 25.0f, cy},
                    {cx + 20.0f, cy + 25.0f}
                };

                float cycle = std::fmod(time * 2.0f, 3.0f);
                int activeIdx = static_cast<int>(cycle);
                float progress = cycle - activeIdx;

                for(int j = 0; j < 3; ++j) {
                    sf::VertexArray line(sf::PrimitiveType::Lines, 2);
                    line[0].position = root; line[0].color = fade(primary, 150);
                    line[1].position = children[j]; line[1].color = fade(accent, 150);
                    window.getWindow().draw(line);
                    
                    if (j == activeIdx) {
                        sf::Vector2f packetPos = root + (children[j] - root) * progress;
                        sf::CircleShape glow(6.0f);
                        glow.setOrigin({6.0f, 6.0f}); glow.setPosition(packetPos);
                        glow.setFillColor(fade(highlight, 120));
                        window.getWindow().draw(glow);
                    }

                    sf::CircleShape childNode(10.0f);
                    childNode.setOrigin({10.0f, 10.0f});
                    childNode.setPosition(children[j]);
                childNode.setFillColor(isDark ? fade(silverCol, 60) : fade(primary, 120));
                    childNode.setOutlineThickness(1.5f);
                    childNode.setOutlineColor(fade(accent, 200));
                    window.getWindow().draw(childNode);
                }
                
                sf::RectangleShape rootNode({24.0f, 24.0f});
                rootNode.setOrigin({12.0f, 12.0f});
                rootNode.setPosition(root);
                rootNode.setFillColor(fade(primary, 200));
                rootNode.setOutlineThickness(2.0f);
                rootNode.setOutlineColor(fade(highlight, 200));
                window.getWindow().draw(rootNode);
            }
        }
    }
    window.getWindow().setView(currentView);
}

void Renderer::visit(const DecorationPayload& payload) {
    float time = payload.time;
    float w = payload.winSize.x;
    float h = payload.winSize.y;

    sf::View currentView = window.getWindow().getView();
    window.getWindow().setView(sf::View(sf::FloatRect({0.f, 0.f}, {w, h})));

    auto fade = [](sf::Color c, std::uint8_t alpha) {
        return sf::Color(c.r, c.g, c.b, alpha);
    };

    sf::Color pCol = theme.inputMenuPrimaryColor;
    sf::Color aCol = theme.inputMenuAccentColor;
    sf::Color hlCol = theme.highlightColor;

    // Hàm Helper vẽ hình học nhanh
    auto drawLineLocal = [&](float x1, float y1, float x2, float y2, sf::Color c, sf::RenderStates st) {
        sf::VertexArray line(sf::PrimitiveType::Lines, 2);
        line[0].position = {x1, y1}; line[0].color = c;
        line[1].position = {x2, y2}; line[1].color = c;
        window.getWindow().draw(line, st);
    };

    auto drawCircleLocal = [&](float x, float y, float r, sf::Color cFill, sf::Color cOut, sf::RenderStates st) {
        sf::CircleShape circle(r);
        circle.setOrigin({r, r}); circle.setPosition({x, y});
        circle.setFillColor(cFill); circle.setOutlineThickness(2.0f); circle.setOutlineColor(cOut);
        window.getWindow().draw(circle, st);
    };

    auto drawRectLocal = [&](float x, float y, float rw, float rh, sf::Color cFill, sf::Color cOut, sf::RenderStates st) {
        sf::RectangleShape rect({rw, rh});
        rect.setOrigin({rw/2.0f, rh/2.0f}); rect.setPosition({x, y});
        rect.setFillColor(cFill); rect.setOutlineThickness(2.0f); rect.setOutlineColor(cOut);
        window.getWindow().draw(rect, st);
    };

    // 1. LƯỚI KỸ THUẬT BACKGROUND (Blueprint Grid)
    float gridSize = 80.0f;
    float offsetX = std::fmod(time * 15.0f, gridSize);
    float offsetY = std::fmod(time * 15.0f, gridSize);
    sf::Color gridCol = fade(pCol, 20); // Rất mờ
    for(float x = offsetX; x < w; x += gridSize) drawLineLocal(x, 0, x, h, gridCol, sf::RenderStates::Default);
    for(float y = offsetY; y < h; y += gridSize) drawLineLocal(0, y, w, y, gridCol, sf::RenderStates::Default);

    // 2. GÓC TRÊN-TRÁI: CÂY NHỊ PHÂN LƠ LỬNG
    {
        sf::RenderStates st; sf::Transform t;
        t.translate({w * 0.15f, h * 0.25f});
        t.rotate(sf::degrees(std::sin(time * 0.5f) * 5.0f)); // Đung đưa nhẹ
        st.transform = t;

        std::vector<sf::Vector2f> tNodes = {{0, -90}, {-70, -20}, {70, -20}, {-110, 50}, {-30, 50}, {30, 50}, {110, 50}, {-130, 120}, {-90, 120}};
        std::vector<std::pair<int, int>> tEdges = {{0,1}, {0,2}, {1,3}, {1,4}, {2,5}, {2,6}, {3,7}, {3,8}};
        for (auto& edge : tEdges) drawLineLocal(tNodes[edge.first].x, tNodes[edge.first].y, tNodes[edge.second].x, tNodes[edge.second].y, fade(aCol, 100), st);
        for (size_t i = 0; i < tNodes.size(); ++i) {
            float pulse = (i == 0) ? std::abs(std::sin(time * 3.0f)) * 0.5f : 0.0f; // Gốc cây nhấp nháy
            drawCircleLocal(tNodes[i].x, tNodes[i].y, 16.0f + pulse * 5.0f, fade(pCol, 40), fade(hlCol, 150), st);
            drawCircleLocal(tNodes[i].x, tNodes[i].y, 4.0f, fade(hlCol, 200), sf::Color::Transparent, st); // Lõi phát sáng
        }
    }

    // 3. GÓC DƯỚI-PHẢI: ĐỒ THỊ LIÊN KẾT ĐẦY ĐỦ (Complete Graph K5)
    {
        sf::RenderStates st; sf::Transform t;
        t.translate({w * 0.85f, h * 0.75f});
        t.rotate(sf::degrees(time * -15.0f)); // Xoay vòng liên tục
        st.transform = t;

        int n = 5; std::vector<sf::Vector2f> gNodes;
        for(int i = 0; i < n; ++i) gNodes.push_back({100.0f * static_cast<float>(std::cos(i * 2.0f * M_PI / n)), 100.0f * static_cast<float>(std::sin(i * 2.0f * M_PI / n))});
        for(int i = 0; i < n; ++i) for(int j = i + 1; j < n; ++j) drawLineLocal(gNodes[i].x, gNodes[i].y, gNodes[j].x, gNodes[j].y, fade(pCol, 120), st);
        for(int i = 0; i < n; ++i) { drawCircleLocal(gNodes[i].x, gNodes[i].y, 18.0f, fade(aCol, 40), fade(aCol, 180), st); drawCircleLocal(gNodes[i].x, gNodes[i].y, 6.0f, fade(hlCol, 220), sf::Color::Transparent, st); }
    }

    // 4. GÓC DƯỚI-TRÁI: DANH SÁCH LIÊN KẾT UỐN LƯỢN (Wavy Linked List)
    {
        sf::RenderStates st; sf::Transform t;
        t.translate({w * 0.2f, h * 0.8f});
        t.rotate(sf::degrees(-10.0f + std::sin(time * 1.2f) * 5.0f));
        st.transform = t;

        int n = 4; std::vector<sf::Vector2f> lNodes;
        for(int i = 0; i < n; ++i) lNodes.push_back({(i - 1.5f) * 110.0f, std::sin(time * 3.0f + i * 1.5f) * 20.0f});
        for(int i = 0; i < n - 1; ++i) { drawLineLocal(lNodes[i].x + 30.0f, lNodes[i].y, lNodes[i+1].x - 30.0f, lNodes[i+1].y, fade(hlCol, 150), st); drawCircleLocal(lNodes[i+1].x - 35.0f, lNodes[i+1].y, 4.0f, fade(hlCol, 200), sf::Color::Transparent, st); }
        for(int i = 0; i < n; ++i) { drawRectLocal(lNodes[i].x, lNodes[i].y, 60.0f, 35.0f, fade(pCol, 80), fade(aCol, 180), st); drawLineLocal(lNodes[i].x + 10.0f, lNodes[i].y - 17.5f, lNodes[i].x + 10.0f, lNodes[i].y + 17.5f, fade(aCol, 180), st); }
    }

    // 5. GÓC TRÊN-PHẢI: MA TRẬN 3D ISOMETRIC (Isometric Matrix / Array)
    {
        sf::RenderStates st; sf::Transform t;
        t.translate({w * 0.8f, h * 0.25f});
        t.rotate(sf::degrees(25.0f)); t.scale({1.0f, 0.6f}); t.rotate(sf::degrees(45.0f)); // Ép hình vuông thành không gian Isometric 3D
        st.transform = t;

        int n = 4; float offset = (n - 1) * 45.0f / 2.0f;
        for(int i = 0; i < n; ++i) for(int j = 0; j < n; ++j) { float wave = std::sin(time * 4.0f + i * 0.5f + j * 0.5f); drawRectLocal(i * 45.0f - offset, j * 45.0f - offset + ((wave > 0.5f) ? -15.0f : 0.0f), 35.0f, 35.0f, (wave > 0.5f) ? fade(hlCol, 100) : fade(pCol, 40), (wave > 0.5f) ? fade(hlCol, 200) : fade(aCol, 120), st); }
    }

    // 6. GIỮA-DƯỚI: BIỂU ĐỒ SẮP XẾP (Sorting Histogram)
    {
        sf::RenderStates st; sf::Transform t;
        t.translate({w * 0.5f, h * 0.85f});
        st.transform = t;

        int numBars = 16;
        float barWidth = 12.0f;
        float spacing = 6.0f;
        float totalW = numBars * (barWidth + spacing);
        float startX = -totalW / 2.0f;

        for(int i = 0; i < numBars; ++i) {
            // Hàm sin phức tạp tạo ra các cột nhấp nhô có vẻ ngẫu nhiên nhưng rất mượt
            float barH = 20.0f + 60.0f * std::abs(std::sin(time * 1.5f + i * 0.8f) * std::cos(time * 0.5f + i * 0.3f));
            float x = startX + i * (barWidth + spacing) + barWidth / 2.0f; 
            float y = -barH / 2.0f;
            
            // Thanh quét sáng (Scanline effect)
            bool isScanning = ((int)(time * 6.0f) % numBars == i) || ((int)(time * 6.0f) % numBars == i + 1);
            sf::Color fillC = isScanning ? fade(hlCol, 150) : fade(pCol, 50);
            sf::Color outC = isScanning ? fade(hlCol, 220) : fade(aCol, 90);
            
            drawRectLocal(x, y, barWidth, barH, fillC, outC, st);
        }
    }

    // 7. GIỮA-TRÊN: MẠNG NƠ-RON NHÂN TẠO (Neural Network)
    {
        sf::RenderStates st; sf::Transform t;
        t.translate({w * 0.5f, h * 0.16f});
        st.transform = t;

        std::vector<int> layers = {3, 5, 4, 2}; // Mạng gồm 4 layer (Input, Hidden x2, Output)
        float layerSpacing = 70.0f;
        float nodeSpacing = 25.0f;
        
        std::vector<std::vector<sf::Vector2f>> nnNodes;
        float startX = -(layers.size() - 1) * layerSpacing / 2.0f;
        
        for(size_t l = 0; l < layers.size(); ++l) {
            std::vector<sf::Vector2f> curLayer;
            float startY = -(layers[l] - 1) * nodeSpacing / 2.0f;
            for(int n = 0; n < layers[l]; ++n) curLayer.push_back({startX + l * layerSpacing, startY + n * nodeSpacing});
            nnNodes.push_back(curLayer);
        }
        
        for(size_t l = 0; l < layers.size() - 1; ++l) {
            for(size_t n1 = 0; n1 < layers[l]; ++n1) {
                for(size_t n2 = 0; n2 < layers[l+1]; ++n2) {
                    float signal = std::sin(time * 6.0f - l * 1.5f + n1 * 0.5f + n2 * 0.5f);
                    sf::Color edgeCol = (signal > 0.85f) ? fade(hlCol, 200) : fade(pCol, 25);
                    drawLineLocal(nnNodes[l][n1].x, nnNodes[l][n1].y, nnNodes[l+1][n2].x, nnNodes[l+1][n2].y, edgeCol, st);
                }
            }
        }
        
        for(size_t l = 0; l < layers.size(); ++l) {
            for(size_t n = 0; n < layers[l]; ++n) drawCircleLocal(nnNodes[l][n].x, nnNodes[l][n].y, 6.0f, fade(pCol, 120), fade(aCol, 180), st);
        }
    }

    {
        sf::RenderStates st; sf::Transform t;
        t.translate({w * 0.5f, h * 0.12f}); // Đẩy xuống một chút cho không gian lớn hơn
        t.translate({0.0f, std::sin(time * 2.0f) * 8.0f}); // Hiệu ứng lơ lửng (Floating)
        st.transform = t;

        // Vòng năng lượng tỏa ra (Aura)
        float auraScale = 1.0f + 0.15f * std::sin(time * 4.0f);
        drawCircleLocal(0, 0, 75.0f * auraScale, fade(pCol, 15), fade(pCol, 40), st);

        // Các vòng quỹ đạo Elip cắt nhau 3D (Atomic Orbits)
        int numRings = 5;
        for (int i = 0; i < numRings; ++i) {
            sf::Transform tRing = t;
            // Mỗi vòng xoay một góc ban đầu và quay với tốc độ khác nhau
            tRing.rotate(sf::degrees(time * (15.0f + i * 8.0f) * (i % 2 == 0 ? 1 : -1) + i * (180.0f / numRings)));
            tRing.scale({1.0f, 0.15f + i * 0.08f}); // Ép trục Y để tạo phối cảnh 3D nghiêng
            
            sf::RenderStates stRing; stRing.transform = tRing;
            float radius = 55.0f + i * 12.0f;
            drawCircleLocal(0, 0, radius, sf::Color::Transparent, fade(aCol, 100 + i * 20), stRing);

            // Hạt dữ liệu (Data Particle) chạy dọc theo quỹ đạo 3D
            sf::Transform tDot = tRing;
            tDot.rotate(sf::degrees(time * (45.0f + i * 20.0f)));
            sf::RenderStates stDot; stDot.transform = tDot;
            drawCircleLocal(radius, 0, 4.0f, fade(hlCol, 255), sf::Color::Transparent, stDot);
            drawCircleLocal(radius - 3.0f, 0, 2.0f, fade(hlCol, 120), sf::Color::Transparent, stDot); // Vệt đuôi
        }

        // Lục giác không gian xoay ngược chiều ở giữa
        sf::Transform tPoly = t;
        tPoly.rotate(sf::degrees(time * -25.0f));
        sf::RenderStates stPoly; stPoly.transform = tPoly;
        int nPoly = 6;
        float pRadius = 38.0f;
        std::vector<sf::Vector2f> polyPts(nPoly);
        for(int i = 0; i < nPoly; ++i) polyPts[i] = {pRadius * std::cos(i * 2.0f * M_PI / nPoly), pRadius * std::sin(i * 2.0f * M_PI / nPoly)};
        for(int i = 0; i < nPoly; ++i) {
            drawLineLocal(polyPts[i].x, polyPts[i].y, polyPts[(i+1)%nPoly].x, polyPts[(i+1)%nPoly].y, fade(hlCol, 150), stPoly); // Viền lục giác
            drawLineLocal(polyPts[i].x, polyPts[i].y, 0, 0, fade(pCol, 80), stPoly); // Trục nối tâm
            drawCircleLocal(polyPts[i].x, polyPts[i].y, 3.0f, fade(aCol, 200), sf::Color::Transparent, stPoly); // Đỉnh lục giác
        }

        // Lõi lượng tử trung tâm nhấp nháy mạnh
        float corePulse = std::abs(std::sin(time * 8.0f));
        drawCircleLocal(0, 0, 14.0f, fade(pCol, 150), fade(aCol, 200), st);
        drawCircleLocal(0, 0, 6.0f + corePulse * 4.0f, fade(hlCol, 200), sf::Color::Transparent, st);

        // Hai bên: Khung giá đỡ Holo-HUD & Mã vạch (Barcode Data Streams)
        for (int i = -1; i <= 1; i += 2) {
            float hx = i * 115.0f; // Tọa độ dạt ra 2 bên
            drawLineLocal(hx, -35.0f, hx, 35.0f, fade(aCol, 180), st);
            drawLineLocal(hx, -35.0f, hx - i * 18.0f, -35.0f, fade(aCol, 180), st);
            drawLineLocal(hx, 35.0f, hx - i * 18.0f, 35.0f, fade(aCol, 180), st);
            
            // Barcode quét nhấp nhô
            for(int b = 0; b < 6; ++b) {
                float bHeight = 10.0f + 25.0f * std::abs(std::sin(time * 12.0f + b * 1.7f + i));
                drawLineLocal(hx + i * (12.0f + b * 6.0f), -bHeight/2.0f, hx + i * (12.0f + b * 6.0f), bHeight/2.0f, fade(pCol, 220), st);
            }
        }
    }

    window.getWindow().setView(currentView);
}
