#include "view/Renderer.h"
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