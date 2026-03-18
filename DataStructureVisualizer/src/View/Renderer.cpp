#include "view/Renderer.h"
Renderer::Renderer(Window& m_window,const Theme& m_theme):window(m_window),theme(m_theme),bgSprite(bgTexture){}
bool Renderer::loadAssets(){
    if(!mainFont.openFromFile(theme.fontPath)) return false;
    if(!bgTexture.loadFromFile(theme.bgImagePath)) return false;
    if(!nodeTexture.loadFromFile(theme.nodeImagePath)) return false;
    if(!arrayTexture.loadFromFile(theme.ArrayImangePath)) return false;
    bgTexture.setSmooth(true);
        bgSprite.setTexture(bgTexture,true);
        sf::Vector2u texSize = bgTexture.getSize();
        sf::Vector2u winSize = window.GetWindow().getSize();
        bgSprite.setScale({(float)winSize.x / texSize.x, (float)winSize.y / texSize.y});
    nodeTexture.setSmooth(true);
    arrayTexture.setSmooth(true);
    return true;
}
void Renderer::drawBackground(){
    window.GetWindow().draw(bgSprite);
}
void Renderer::drawImageNode(float x,float y,const std::string& text){
    //node
    sf::Sprite sprite(nodeTexture);
    sprite.setColor(theme.nodeTintColor);
    sprite.setScale({theme.nodeScale,theme.nodeScale});
    sf::FloatRect bounds=sprite.getLocalBounds();
    sprite.setOrigin({bounds.size.x/2,bounds.size.y/2});
    sprite.setPosition({x,y});
    window.GetWindow().draw(sprite);

    //text
    unsigned int textSize = static_cast<unsigned int>(40 * theme.nodeScale);
    sf::Text a(mainFont,text,textSize);
    a.setFillColor(theme.textColor);
    sf::FloatRect textRect=a.getLocalBounds();
    a.setOrigin({textRect.position.x+textRect.size.x/2,textRect.position.y+textRect.size.y-10});
    a.setPosition({x,y});
    window.GetWindow().draw(a);
}
void Renderer::drawArrayCell(float x,float y,const std::string& text){
    sf::Sprite sprite(arrayTexture);
    sprite.setColor(theme.arrayTintColor);
    sprite.setScale({theme.arrayScale,theme.arrayScale});
    sf::FloatRect bounds=sprite.getLocalBounds();
    sprite.setOrigin({bounds.size.x/2,bounds.size.y/2});
    sprite.setPosition({x,y});
    window.GetWindow().draw(sprite);

    unsigned int textSize=static_cast<unsigned int>(40*theme.arrayScale);
    sf::Text a(mainFont,text,40);
    a.setFillColor(theme.textColor);
    sf::FloatRect textRect=a.getLocalBounds();
    a.setOrigin({textRect.position.x+textRect.size.x/2,textRect.position.y+textRect.size.y/2});
    a.setPosition({x,y});
    window.GetWindow().draw(a);
}
void Renderer::drawLine(float x1, float y1, float x2, float y2, float thickness) {
    float length = std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2));
    sf::RectangleShape line({length, thickness});
    line.setFillColor(theme.arrowColor);
    line.setOrigin({0, thickness / 2.0f});
    line.setPosition({x1, y1});
    
    float angle = std::atan2(y2 - y1, x2 - x1) * 180.0f / 3.14159f;
    line.setRotation(sf::degrees(angle));

    window.GetWindow().draw(line);
}