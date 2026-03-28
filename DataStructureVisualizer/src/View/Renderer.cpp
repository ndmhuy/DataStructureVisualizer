#include "View/Renderer.h"

Renderer::Renderer(Window& m_window, const Theme& m_theme)
    : window(m_window), theme(m_theme), bgSprite(bgTexture) {}

bool Renderer::loadAssets() {
    if (!mainFont.openFromFile(theme.fontPath)) return false;
    if (!bgTexture.loadFromFile(theme.bgImagePath)) return false;
    if (!nodeTexture.loadFromFile(theme.nodeImagePath)) return false;
    if (!arrayTexture.loadFromFile(theme.ArrayImangePath)) return false;

    bgTexture.setSmooth(true);
    bgSprite.setTexture(bgTexture, true);

    sf::Vector2u texSize = bgTexture.getSize();
    sf::Vector2u winSize = window.GetWindow().getSize();
    bgSprite.setScale({static_cast<float>(winSize.x) / texSize.x, static_cast<float>(winSize.y) / texSize.y});

    nodeTexture.setSmooth(true);
    arrayTexture.setSmooth(true);
    return true;
}

void Renderer::drawBackground() {
    window.GetWindow().draw(bgSprite);
}

void Renderer::drawImageNode(float x, float y, const std::string& text) {
    // node
    sf::Sprite sprite(nodeTexture);
    sprite.setColor(theme.nodeTintColor);
    sprite.setScale({theme.nodeScale, theme.nodeScale});
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin({bounds.size.x / 2, bounds.size.y / 2});
    sprite.setPosition({x, y});
    window.GetWindow().draw(sprite);

    // text
    unsigned int textSize = static_cast<unsigned int>(30 * theme.nodeScale);
    sf::Text a(mainFont, text, textSize);
    a.setFillColor(theme.textColor);
    sf::FloatRect textRect = a.getLocalBounds();
    a.setOrigin({textRect.position.x + textRect.size.x / 2, textRect.position.y + textRect.size.y - 10 * theme.nodeScale});
    a.setPosition({x, y});
    window.GetWindow().draw(a);
}

void Renderer::drawArrayCell(float x, float y, const std::string& text) {
    sf::Sprite sprite(arrayTexture);
    sprite.setColor(theme.arrayTintColor);
    sprite.setScale({theme.arrayScale, theme.arrayScale});
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin({bounds.size.x / 2, bounds.size.y / 2});
    sprite.setPosition({x, y});
    window.GetWindow().draw(sprite);

    unsigned int textSize = static_cast<unsigned int>(30 * theme.arrayScale);
    sf::Text a(mainFont, text, textSize);
    a.setFillColor(theme.textColor);
    sf::FloatRect textRect = a.getLocalBounds();
    a.setOrigin({textRect.position.x + textRect.size.x / 2, textRect.position.y + textRect.size.y / 2});
    a.setPosition({x, y});
    window.GetWindow().draw(a);
}

void Renderer::drawLineWithArrow(
    float x1,
    float y1,
    float w1,
    float h1,
    ShapeType type1,
    float x2,
    float y2,
    float w2,
    float h2,
    ShapeType type2,
    float thickness,
    float arrowSize) {
    float dx = x2 - x1;
    float dy = y2 - y1;
    float distance = std::sqrt(dx * dx + dy * dy);

    float angle = std::atan2(dy, dx);
    float angleDegrees = angle * 180.0f / M_PI;

    sf::Vector2f start = getBoundaryPoint(x1, y1, w1, h1, angle, type1);
    
    sf::Vector2f end = getBoundaryPoint(x2, y2, w2, h2, angle + M_PI, type2);

    float lineLength = std::sqrt(std::pow(end.x - start.x, 2) + std::pow(end.y - start.y, 2));

    if (distance < (w1 / 2 + w2 / 2)) return;

    sf::RectangleShape line({lineLength, thickness});
    line.setFillColor(theme.arrowColor);
    line.setOrigin({0, thickness / 2.0f});
    line.setPosition(start);
    line.setRotation(sf::degrees(angleDegrees));
    window.GetWindow().draw(line);

    sf::ConvexShape arrowHead;
    arrowHead.setPointCount(3);
    arrowHead.setPoint(0, {0.0f, 0.0f});
    arrowHead.setPoint(1, {-arrowSize, -arrowSize / 1.5f});
    arrowHead.setPoint(2, {-arrowSize, arrowSize / 1.5f});

    arrowHead.setFillColor(theme.arrowColor);
    arrowHead.setPosition(end);
    arrowHead.setRotation(sf::degrees(angleDegrees));
    
    window.GetWindow().draw(arrowHead);
}

void Renderer::drawLine(float x1, float y1, float w1, float h1, ShapeType type1,float x2, float y2, float w2, float h2, ShapeType type2, float thickness) {
    float dx = x2 - x1;
    float dy = y2 - y1;
    float distance = std::sqrt(dx * dx + dy * dy);

    float angle = std::atan2(dy, dx);
    float angleDegrees = angle * 180.0f / M_PI;

    sf::Vector2f start = getBoundaryPoint(x1, y1, w1, h1, angle, type1);
    
    sf::Vector2f end = getBoundaryPoint(x2, y2, w2, h2, angle + M_PI, type2);

    float lineLength = std::sqrt(std::pow(end.x - start.x, 2) + std::pow(end.y - start.y, 2));

    if (distance < (w1 / 2 + w2 / 2)) return;

    sf::RectangleShape line({lineLength, thickness});
    line.setFillColor(theme.arrowColor);
    line.setOrigin({0, thickness / 2.0f});
    line.setPosition(start);
    line.setRotation(sf::degrees(angleDegrees));
    window.GetWindow().draw(line);
}

sf::Vector2f Renderer::getBoundaryPoint(float cx, float cy, float width, float height, float angle, ShapeType type) {
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

void Renderer::drawText(float x, float y, const std::string& text, unsigned int size, sf::Color color, TextPosition align, sf::Angle angle) {
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
    t.setPosition({x, y});
    t.setRotation(angle);

    window.GetWindow().draw(t);
}

void Renderer::drawTextUp(float cx, float cy, float objHeight, float padding, const std::string& text, unsigned int size, sf::Color color) {
    float textX = cx;
    float textY = cy - (objHeight / 2.0f) - padding;
    drawText(textX, textY, text, size, color, TextPosition::Bottom);
}

void Renderer::drawTextDown(float cx, float cy, float objHeight, float padding, const std::string& text, unsigned int size, sf::Color color) {
    float textX = cx;
    float textY = cy + (objHeight / 2.0f) + padding;
    drawText(textX, textY, text, size, color, TextPosition::Top);
}

void Renderer::drawTextOnLine(float x1, float y1, float x2, float y2, float padding, const std::string& text, unsigned int size, sf::Color color) {
    float midX = (x1 + x2) / 2.0f;
    float midY = (y1 + y2) / 2.0f;
    
    float angle = std::atan2(y2 - y1, x2 - x1);
    float perpAngle = angle - M_PI / 2.0f;
    float angleDeg = angle * 180.0f / M_PI;
    if (angleDeg > 90.0f || angleDeg < -90.0f) {
        angleDeg += 180.0f;
    }
    
    float textX = midX + std::cos(perpAngle) * padding;
    float textY = midY + std::sin(perpAngle) * padding;
    
    drawText(textX, textY, text, size, color, TextPosition::Bottom, sf::degrees(angleDeg));
}

void Renderer::drawTextTopLeft(float cx, float cy, float objWidth, float objHeight, float padding, const std::string& text, unsigned int size, sf::Color color) {
    float textX = cx - (objWidth / 2.0f) - padding;
    float textY = cy - (objHeight / 2.0f) - padding;
    drawText(textX, textY, text, size, color, TextPosition::BottomRight);
}

void Renderer::drawTextTopRight(float cx, float cy, float objWidth, float objHeight, float padding, const std::string& text, unsigned int size, sf::Color color) {
    float textX = cx + (objWidth / 2.0f) + padding;
    float textY = cy - (objHeight / 2.0f) - padding;
    drawText(textX, textY, text, size, color, TextPosition::BottomLeft);
}

void Renderer::drawTextBottomLeft(float cx, float cy, float objWidth, float objHeight, float padding, const std::string& text, unsigned int size, sf::Color color) {
    float textX = cx - (objWidth / 2.0f) - padding;
    float textY = cy + (objHeight / 2.0f) + padding;
    drawText(textX, textY, text, size, color, TextPosition::TopRight);
}

void Renderer::drawTextBottomRight(float cx, float cy, float objWidth, float objHeight, float padding, const std::string& text, unsigned int size, sf::Color color) {
    float textX = cx + (objWidth / 2.0f) + padding;
    float textY = cy + (objHeight / 2.0f) + padding;
    drawText(textX, textY, text, size, color, TextPosition::TopLeft);
}