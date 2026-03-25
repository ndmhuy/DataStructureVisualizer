#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include <cmath>
#include "View/Window.h"
#include "Model/Theme.h"
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

    bool loadAssets();
    void drawBackground();
    void drawImageNode(float x, float y, const std::string& text);
    void drawArrayCell(float x, float y, const std::string& text);
    sf::Vector2f getBoundaryPoint(float cx, float cy, float width, float height, float angle, ShapeType type);
    void drawLineWithArrow(float x1, float y1, float w1, float h1, ShapeType type1,float x2, float y2, float w2, float h2, ShapeType type2, float thickness, float arrowSize=12.0f);
    void drawLine(float x1, float y1, float w1, float h1, ShapeType type1,float x2, float y2, float w2, float h2, ShapeType type2, float thickness);
    sf::Vector2f getNodeSize() const;//in main.cpp have to use to access nodeW 
    sf::Vector2f getArraySize() const;//in main.cpp have to use to access nodeH
    void drawText(float x, float y, const std::string& text, unsigned int size, sf::Color color, TextPosition align = TextPosition::TopLeft,sf::Angle angle=sf::degrees(0.f));
    void drawTextUp(float cx, float cy, float objHeight, float padding, const std::string& text, unsigned int size, sf::Color color);
    void drawTextDown(float cx, float cy, float objHeight, float padding, const std::string& text, unsigned int size, sf::Color color);
    // draw on the arrow or line
    void drawTextOnLine(float x1, float y1, float x2, float y2, float padding, const std::string& text, unsigned int size, sf::Color color);
    void drawTextTopLeft(float cx, float cy, float objWidth, float objHeight, float padding, const std::string& text, unsigned int size, sf::Color color);
    void drawTextTopRight(float cx, float cy, float objWidth, float objHeight, float padding, const std::string& text, unsigned int size, sf::Color color);
    void drawTextBottomLeft(float cx, float cy, float objWidth, float objHeight, float padding, const std::string& text, unsigned int size, sf::Color color);
    void drawTextBottomRight(float cx, float cy, float objWidth, float objHeight, float padding, const std::string& text, unsigned int size, sf::Color color);
};
