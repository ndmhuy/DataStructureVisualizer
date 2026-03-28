#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include "imgui.h"
#include "imgui-SFML.h"
#include "Button.h"
#include "Slider.h"
#include "InputMenu.h"
#include "CodePanel.h"

class UIManager{
    private:
    //statics values
        bool isPlay=true; //Determine if it's play butt or pause butt, initially play butt
        Button play;
        Button pause;
        Button stepForward;
        Button stepBackward;

        Slider speedSlider;
        float speed;

        InputMenu inputMenu;

        CodePanel codePanel;
    public:

    UIManager()=default;//constructor

    bool init(sf::RenderWindow&); //Initial set up for ImGui+SFML

    void processEvent(sf::RenderWindow&,const sf::Event&); //Pass event from SFML to Imgui

    void resize(const sf::RenderWindow& window); //resize event

    void update(sf::RenderWindow&,const sf::Time&); //Update UI status

    void render(sf::RenderWindow&); //draw UI

    void shutdown(); //shutdown Imgui
};