#include <iostream>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include "imgui.h"
#include "imgui-SFML.h"
#include "../../include/View/UIManager.h"
#include "../../include/View/Button.h"


bool UIManager::init(sf::RenderWindow& window){
    if (!ImGui::SFML::Init(window)) {
        std::cerr<<"Can't ImGUI::SFML::Init(window)";
        return false;
    }
    if (!play.init("../assets/playbutton.png"/*stringpath*/)){
        std::cerr<<"Can't load play image";
        return false;
    }
    if (!pause.init("../assets/pausebutton.png"/*stringpath*/)){
        std::cerr<<"Can't load pause image";
        return false;
    }
    if (!stepForward.init("../assets/stepforward.png"/*stringpath*/)){
        std::cerr<<"Can't load step forward image";
        return false;
    }
    if (!stepBackward.init("../assets/stepbackward.png"/*stringpath*/)){
        std::cerr<<"Can't load play image";
        return false;
    }
    speed=1;
    speedSlider.init(&speed);
    //setup Size and position for buttons
    resize(window);

    play.setActive(true);
    pause.setActive(false);
    stepForward.setActive(false);
    stepBackward.setActive(false);

    if (!inputMenu.init()){
        std::cerr<<"Can't load randombox from file";
        return false;
    }

    return true;
}

void UIManager::processEvent(sf::RenderWindow& window,const sf::Event& event){
    ImGui::SFML::ProcessEvent(window,event);

    // If the mouse is in the Menu of ImGUI -> ignore this click
    if (ImGui::GetIO().WantCaptureMouse) {
        return; 
    }
    
    if (play.handleEvent(window,event)){
        //do sth when play butt is clicked
        isPlay=false;
        play.setActive(isPlay);
        pause.setActive(!isPlay);
    }
    if (pause.handleEvent(window,event)){
        //do sth when pause butt iss clicked
        isPlay=true;
        play.setActive(isPlay);
        pause.setActive(!isPlay);
    }
    if (stepForward.handleEvent(window,event)){
        //stepForward logic
    }
    if (stepBackward.handleEvent(window,event)){
        //stepBackward logic
    }
    speedSlider.handleEvent(window,event);
}

void UIManager::resize(const sf::RenderWindow& window){
    sf::Vector2u windowsize=window.getSize();
    float x=windowsize.x;
    float y=windowsize.y;
    float rad=std::min(y/16,x/12);
    play.resize(sf::Vector2f{x/4,15*y/16},rad);
    pause.resize(sf::Vector2f{x/4,15*y/16},rad);
    stepForward.resize(sf::Vector2f{5*x/12,15*y/16},rad);
    stepBackward.resize(sf::Vector2f{x/12,15*y/16},rad);
    speedSlider.resize(window);
}

void UIManager::update(sf::RenderWindow& window,const sf::Time& deltatime){
    ImGui::SFML::Update(window, deltatime);
}

void UIManager::render(sf::RenderWindow& window){
    inputMenu.render(window);
    if (isPlay) play.render(window);
    else pause.render(window);
    stepForward.render(window);
    stepBackward.render(window);
    speedSlider.render(window);
    ImGui::SFML::Render(window);
}

void UIManager::shutdown(){
    ImGui::SFML::Shutdown();
}
