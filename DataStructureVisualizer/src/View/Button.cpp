#include <iostream>

#include "View/Button.h"

bool Button::init(const std::string& imagepath, const Theme& selectedTheme) {
    theme = selectedTheme;

    if (!texture.loadFromFile(imagepath)) {
        std::cerr << "Warning: Can't load '" << imagepath << "'. Creating dummy texture!\n";

        // If it fais, generate a 50x50 Magenta square in code
        sf::Image dummyImage;
        dummyImage.resize({50, 50}, theme.buttonFallbackColor);
        
        // Load the pink square into the texture
        if (!texture.loadFromImage(dummyImage)) return false;
    }

    texture.setSmooth(true);
    sprite.setTexture(texture, true);

    sf::Vector2u textureSize = texture.getSize();
    sprite.setOrigin({textureSize.x / 2.0f, textureSize.y / 2.0f});
    
    return true;
}

void Button::setActive(bool active) {
    isActive = active;
    if (!isActive) {
        isHovered = false;
        isPressed = false;
        sprite.setColor(theme.buttonInactiveColor);
    } else {
        sprite.setColor(theme.buttonNormalColor);
    }
}

void Button::resize(sf::Vector2f position, float radius) {
    sf::Vector2u textureSize = texture.getSize();
    // set position (is the position of thentre)
    sprite.setPosition(position);

    //Scale
    float targetDiameter = radius * 2.0f;
    sprite.setScale(
        {targetDiameter / static_cast<float>(textureSize.x),
         targetDiameter / static_cast<float>(textureSize.y)});

    pos = position;
    rad = radius;
}

bool Button::handleEvent(const sf::RenderWindow& window, const sf::Event& event) {
    if (!isActive) {
        isHovered = false;
        isPressed = false;
        return false;
    }
    sf::Vector2i mousePixelPos = sf::Mouse::getPosition(window);
    sf::Vector2f mouseWorldPos = window.mapPixelToCoords(mousePixelPos);
    float dx = mouseWorldPos.x - pos.x;
    float dy = mouseWorldPos.y - pos.y;
    isHovered = (dx * dx + dy * dy <= rad * rad);

    if (!isHovered) {
        isPressed = false;
        sprite.setColor(theme.buttonNormalColor);
        return false;
    }

    if (event.is<sf::Event::MouseButtonPressed>()) {
        const auto* mouseEvent = event.getIf<sf::Event::MouseButtonPressed>();
        if (mouseEvent && mouseEvent->button == sf::Mouse::Button::Left && isHovered) {
            isPressed = true;
            sprite.setColor(theme.buttonPressedColor);
            return false;
        }
    }

    if (event.is<sf::Event::MouseButtonReleased>()) {
        const auto* mouseEvent = event.getIf<sf::Event::MouseButtonReleased>();
        if (mouseEvent && mouseEvent->button == sf::Mouse::Button::Left && isPressed) {
            isPressed = false;
            sprite.setColor(theme.buttonHoveredColor);
            return true;
        }
    }

    if (!isPressed) {
        sprite.setColor(theme.buttonHoveredColor);
    } else {
        sprite.setColor(theme.buttonPressedColor);
    }

    return false;
}

void Button::render(sf::RenderWindow& window) {
    window.draw(sprite);
}