#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include "imgui.h"
#include "imgui-SFML.h"
#include "../../include/View/UIManager.h"

bool UIManager::init(sf::RenderWindow& window){
    return ImGui::SFML::Init(window);
}

void UIManager::processEvent(sf::RenderWindow& window,const sf::Event& event){
    ImGui::SFML::ProcessEvent(window,event);
}

void UIManager::update(sf::RenderWindow& window,const sf::Time& deltatime){
    ImGui::SFML::Update(window, deltatime);
    //Draw sth here 
}

void UIManager::render(sf::RenderWindow& window){
    ImGui::SFML::Render(window);
}

void UIManager::shutdown(){
    ImGui::SFML::Shutdown();
}


bool Button::init(const std::string& imagepath, sf::Vector2f position, sf::Vector2f size){
    if (!texture.loadFromFile(imagepath)){
        std::cerr<<"Can't load from file!";
        return false;
    }
    texture.setSmooth(true);
    sprite.setTexture(texture);
    sprite.setPosition(position);
    
    //fixed size
    sf::Vector2u textureSize = texture.getSize();
    sprite.setScale(
        {size.x / static_cast<float>(textureSize.x), 
        size.y / static_cast<float>(textureSize.y)}
    );

    return true;
}

void Button::setActive(bool active){
    isActive=active;
}

bool Button::handleEvent(const sf::RenderWindow& window, const sf::Event& event){
    if (!isActive){
        isHovered = false;
        isPressed = false;
        sprite.setColor(notActive);
        return false;
    }
    sf::Vector2i mousePixelPos = sf::Mouse::getPosition(window);
    sf::Vector2f mouseWorldPos = window.mapPixelToCoords(mousePixelPos);
    isHovered = sprite.getGlobalBounds().contains(mouseWorldPos);

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
    sprite.setColor(hovered);
    return false;
}

void Button::render(sf::RenderWindow& window){
    window.draw(sprite);
}