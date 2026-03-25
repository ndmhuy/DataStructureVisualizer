#include <iostream>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include "../../include/View/Button.h"

bool Button::init(const std::string& imagepath){
    if (!texture.loadFromFile(imagepath)){
        std::cerr<<"Can't load from file!";
        return false;
    }
    texture.setSmooth(true);
    sprite.setTexture(texture);

    sf::Vector2u textureSize = texture.getSize();
    //set centre
    sprite.setOrigin({textureSize.x / 2.0f, textureSize.y / 2.0f});

    return true;
}

void Button::setActive(bool active){
    isActive = active;
    if (!isActive) {
        isHovered = false;
        isPressed = false;
        sprite.setColor(notActive); 
    } else {
        sprite.setColor(normal);  
    }
}

void Button::resize(sf::Vector2f position, float radius){
    sf::Vector2u textureSize = texture.getSize();
    // set position (is the position of the centre)
    sprite.setPosition(position);

    //Scale
    float targetDiameter = radius * 2.0f;
    sprite.setScale(
        {targetDiameter / static_cast<float>(textureSize.x), 
        targetDiameter / static_cast<float>(textureSize.y)}
    );

    pos=position;
    rad=radius;
}

bool Button::handleEvent(const sf::RenderWindow& window, const sf::Event& event){
    if (!isActive){
        isHovered = false;
        isPressed = false;
        return false;
    }
    sf::Vector2i mousePixelPos = sf::Mouse::getPosition(window);
    sf::Vector2f mouseWorldPos = window.mapPixelToCoords(mousePixelPos);
    float dx=mouseWorldPos.x-pos.x;
    float dy=mouseWorldPos.y-pos.y;
    isHovered=(dx*dx+dy*dy<=rad*rad);

    if (!isHovered){
        isPressed=false;
        sprite.setColor(normal);
        return false;
    }
    if (event.is<sf::Event::MouseButtonPressed>()) {
        const auto* mouseEvent = event.getIf<sf::Event::MouseButtonPressed>();
        if (mouseEvent && mouseEvent->button == sf::Mouse::Button::Left && isHovered) {
            isPressed = true;
            sprite.setColor(pressed);
            return false;
        }
    }
    if (event.is<sf::Event::MouseButtonReleased>()) {
        const auto* mouseEvent = event.getIf<sf::Event::MouseButtonReleased>();
        if (mouseEvent && mouseEvent->button == sf::Mouse::Button::Left&&isPressed) {
            isPressed = false;
            sprite.setColor(hovered);
            return true;
        }
    }
    if (!isPressed) sprite.setColor(hovered);
    else sprite.setColor(pressed);
    return false;
}

void Button::render(sf::RenderWindow& window){
    window.draw(sprite);
}