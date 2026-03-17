#include "view/Renderer.h"
Renderer::Renderer(Window& m_window,const Theme& m_theme):window(m_window),theme(m_theme),bgSprite(bgTexture){}
bool Renderer::loadAssets(){
    if(!mainFont.openFromFile(theme.fontPath)) return false;
    if(!bgTexture.loadFromFile(theme.bgImagePath)) return false;
    if(!nodeTexture.loadFromFile(theme.nodeImagePath)) return false;
    if(!arrayTexture.loadFromFile(theme.ArrayImangePath)) return false;
    bgTexture.setSmooth(true);
        bgSprite.setTexture(bgTexture);
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
    sf::FloatRect bounds=sprite.getGlobalBounds();
    sprite.setOrigin({bounds.size.x/2,bounds.size.y/2});
    sprite.setPosition({x,y});
    window.GetWindow().draw(sprite);

    //text
    sf::Text a(mainFont,text,20);
    a.setFillColor(theme.textColor);
    sf::FloatRect textRect=a.getLocalBounds();
    a.setOrigin({bounds.size.x,bounds.size.y});
    a.setPosition({x,y});
    window.GetWindow().draw(a);
}