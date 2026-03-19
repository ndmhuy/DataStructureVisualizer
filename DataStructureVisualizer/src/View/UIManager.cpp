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
