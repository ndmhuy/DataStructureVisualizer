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
    if (isActive){
        sprite.setColor(sf::Color(255,255,255,255));
    }
    else{
        sprite.setColor(sf::Color(150,150,150,100));
    }
}

bool Button::handleEvent(const sf::RenderWindow& window, const sf::Event& event){
    if (!isActive) return false;
    sf::Vector2i mousePixelPos = sf::Mouse::getPosition(window);
    sf::Vector2f mouseWorldPos = window.mapPixelToCoords(mousePixelPos);

    //check if the mouse is in the button
    isHovered = sprite.getGlobalBounds().contains(mouseWorldPos);

    if (isHovered){
        //dim
        sprite.setColor(sf::Color(200,200,200,255));

        if (event.is<sf::Event::MouseButtonPressed>()) {
                const auto* mouseEvent = event.getIf<sf::Event::MouseButtonPressed>();
                if (mouseEvent && mouseEvent->button == sf::Mouse::Button::Left) {
                    isPressed = true;
                    // Click: dimmer
                    sprite.setColor(sf::Color(150, 150, 150, 255)); 
                }
            } 
            else if (event.is<sf::Event::MouseButtonReleased>()) {
                const auto* mouseEvent = event.getIf<sf::Event::MouseButtonReleased>();
                if (mouseEvent && mouseEvent->button == sf::Mouse::Button::Left) {
                    if (isPressed) {
                        isPressed = false;
                        sprite.setColor(sf::Color(200, 200, 200, 255)); //return dim color
                        return true; // successfully clicked
                    }
                }
    }
    }
    else {
        sprite.setColor(sf::Color(255,255,255,255));
        isPressed = false;
    }
    return false;
}

void Button::render(sf::RenderWindow& window){
    window.draw(sprite);
}