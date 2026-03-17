#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
class Renderer{
    private:
    sf::RenderWindow& Window;
    sf::Font fontchar;
    public:
    Renderer(sf::RenderWindow& Window,const std::string& path);
    bool loadfont(std::string& path);
    void drawNode(float x,float y, float radius,const std::string& text, sf::Color color);
    void drawArray(float x,float y,float height, float width,std::string& text,sf::Color color);
    void drawArrow(sf::Vector2f start,sf::Vector2f end,sf::Color color);
};