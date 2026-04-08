#ifndef RENDERER_H
#define RENDERER_H

#include <iostream>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <string>

#include "View/Window.h"
#include "View/Theme.h"

enum class TextPosition {
    TopLeft,
    TopRight,
    BottomLeft,
    BottomRight,
    Top,
    Bottom,
};
enum class ShapeType {
    Circle,
    Rectangle
};

struct Frame; // Forward declaration

class Renderer {
private:
    Window& window;
    const Theme& theme;

    sf::Font mainFont;
    sf::Texture bgTexture;
    sf::Texture nodeTexture;
    sf::Texture arrayTexture;

    sf::Sprite bgSprite;

public:
    Renderer(Window& m_window, const Theme& m_theme);

    // Asset lifecycle and scene background.
    bool loadAssets();
    void drawBackground();

    // Node and array element rendering.
    void drawImageNode(sf::Vector2f pos, const std::string& text, bool isHighlighted = false);
    void drawArrayCell(sf::Vector2f pos, const std::string& text, bool isHighlighted = false);

    // Edge/connector rendering helpers.
    sf::Vector2f getBoundaryPoint(sf::Vector2f center, sf::Vector2f size, float angle, ShapeType type);
    void drawLineWithArrow(sf::Vector2f p1, sf::Vector2f size1, ShapeType type1, sf::Vector2f p2, sf::Vector2f size2, ShapeType type2, float thickness, float arrowSize = 12.0f, bool isHighlighted = false);
    void drawLine(sf::Vector2f p1, sf::Vector2f size1, ShapeType type1, sf::Vector2f p2, sf::Vector2f size2, ShapeType type2, float thickness, bool isHighlighted = false);

    void drawFrame(const Frame* frame);
    void drawArrayData(const Frame* frame);
    void drawGraphData(const Frame* frame);

    // Texture-based size queries.
    sf::Vector2f getNodeSize() const;  // in main.cpp have to use to access nodeW
    sf::Vector2f getArraySize() const; // in main.cpp have to use to access nodeH

    // Generic and positional text helpers.
    void drawText(
        sf::Vector2f pos,
        const std::string& text,
        unsigned int size,
        sf::Color color,
        TextPosition align = TextPosition::TopLeft,
        sf::Angle angle = sf::degrees(0.f));

    void drawTextUp(sf::Vector2f center, float objHeight, float padding, const std::string& text, unsigned int size, sf::Color color);
    void drawTextDown(sf::Vector2f center, float objHeight, float padding, const std::string& text, unsigned int size, sf::Color color);

    // draw on the arrow or line
    void drawTextOnLine(sf::Vector2f p1, sf::Vector2f p2, float padding, const std::string& text, unsigned int size, sf::Color color);

    void drawTextTopLeft(sf::Vector2f center, sf::Vector2f objSize, float padding, const std::string& text, unsigned int size, sf::Color color);
    void drawTextTopRight(sf::Vector2f center, sf::Vector2f objSize, float padding, const std::string& text, unsigned int size, sf::Color color);
    void drawTextBottomLeft(sf::Vector2f center, sf::Vector2f objSize, float padding, const std::string& text, unsigned int size, sf::Color color);
    void drawTextBottomRight(sf::Vector2f center, sf::Vector2f objSize, float padding, const std::string& text, unsigned int size, sf::Color color);
};

#endif // RENDERER_H