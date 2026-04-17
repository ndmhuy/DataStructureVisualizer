#include "View/Renderer.h"
#include "Model/Frame.h"
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
    window.getWindow().draw(bgSprite);
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
    draggedNodeIndex = -1;
}
void Renderer::handleMousePress(sf::Vector2f mousePos) {
    sf::Vector2f nodeSize = getNodeSize();
    float radius = std::max(nodeSize.x, nodeSize.y) / 2.0f;

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
            return;
        }
    }
}
void Renderer::handleMouseMove(sf::Vector2f mousePos) {
    if (draggedNodeIndex != -1) {
        customNodePositions[draggedNodeIndex] = mousePos + dragOffset;
    }
}
void Renderer::handleMouseRelease() {
    draggedNodeIndex = -1;
}

void Renderer::drawFrame(const Frame* frame) {
    if (!frame) return;
    
    drawGraphData(frame);
    drawArrayData(frame);
}

void Renderer::drawArrayData(const Frame* frame) {
    const auto& arr = frame->getArrayData();
    if (arr.empty()) return;

    sf::Vector2f cellSize = getArraySize();
    float spacing = 10.0f;
    float totalWidth = arr.size() * (cellSize.x + spacing) - spacing;

    sf::Vector2u winSize = window.getWindow().getSize();
    float startX = (winSize.x - totalWidth) / 2.0f + cellSize.x / 2.0f;
    float startY = winSize.y - cellSize.y - 50.0f;

    const auto& highlights = frame->getHighlightIndices();

    for (size_t i = 0; i < arr.size(); ++i) {
        bool highlighted = std::find(highlights.begin(), highlights.end(), i) != highlights.end();
        
        sf::Vector2f pos(startX + i * (cellSize.x + spacing), startY);
        drawArrayCell(pos, std::to_string(arr[i]), highlighted);
        
        // Draw array index below the cell
        drawTextDown(pos, cellSize.y, 5.0f, std::to_string(i), 18, theme.textColor);
    }
}

void Renderer::drawGraphData(const Frame* frame) {
    const auto& vertices = frame->getVerticesData();
    const auto& edges = frame->getEdgeData();
    if (vertices.empty()) return;

    sf::Vector2u winSize = window.getWindow().getSize();
    float cx = winSize.x / 2.0f;
    float cy = winSize.y / 2.0f - 50.0f; 
    
    float radius = std::min(cx, cy) - 100.0f;
    if (radius < 50.0f) radius = 50.0f;
    defaultNodePositions.clear();
    std::vector<sf::Vector2f> positions(vertices.size());
    for (size_t i = 0; i < vertices.size(); ++i) {
        if (vertices.size() == 1) {
            positions[i] = {cx, cy};
        } else {
            float angle = i * (2.0f * M_PI / vertices.size()) - M_PI / 2.0f;
            positions[i] = {cx + radius * std::cos(angle), cy + radius * std::sin(angle)};
        }
        defaultNodePositions[i] = positions[i]; // Lưu vị trí lý thuyết

        if (customNodePositions.find(i) != customNodePositions.end()) {
            positions[i] = customNodePositions[i]; // Ghi đè bằng vị trí được kéo thả
        }
    }

    sf::Vector2f nodeSize = getNodeSize();
    const auto& hEdges = frame->getHighlightEdges();
    const auto& hIndices = frame->getHighlightIndices();

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
