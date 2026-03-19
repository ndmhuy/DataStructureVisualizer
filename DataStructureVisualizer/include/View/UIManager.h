#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include "imgui.h"
#include "imgui-SFML.h"

class UIManager{
    private:
    //statics valuables
    public:

    bool init(sf::RenderWindow&); //Initial set up for ImGui+SFML

    void processEvent(sf::RenderWindow&,const sf::Event&); //Pass event from SFML to Imgui

    void update(sf::RenderWindow&,const sf::Time&); //Update UI status

    void render(sf::RenderWindow&); //draw UI

    void shutdown(); //shutdown Imgui

};