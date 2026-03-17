#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include "View/Window.h"
#include "Model/Theme.h"
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

    // Các hàm vẽ Generic (Task 3 & 4)
    void drawBackground();
    void drawImageNode(float x, float y, const std::string& text);
    void drawArrayCell(float x, float y, const std::string& text);
    void drawLine(float x1, float y1, float x2, float y2, float thickness = 3.0f);
};