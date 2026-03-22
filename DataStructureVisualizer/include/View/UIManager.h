#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include "imgui.h"
#include "imgui-SFML.h"

class UIManager{
    private:
    //statics values

    public:

    bool init(sf::RenderWindow&); //Initial set up for ImGui+SFML

    void processEvent(sf::RenderWindow&,const sf::Event&); //Pass event from SFML to Imgui

    void update(sf::RenderWindow&,const sf::Time&); //Update UI status

    void render(sf::RenderWindow&); //draw UI

    void shutdown(); //shutdown Imgui

};

class Button{
    private:
        //statics
        bool isActive=false; //If the button is active (dim/light)
        bool isPressed=false; //If the button is clicked
        bool isHovered=false; // If the mouse is in the button

        //image
        sf::Texture texture;
        sf::Sprite sprite;
    
    public:

    //create a button with image and fixed position and size
    bool init(const std::string& , sf::Vector2f, sf::Vector2f);

    //determine if the Button is active or not
    void setActive(bool);

    //Address if the button is clicked
    bool handleEvent(const sf::RenderWindow&, const sf::Event& );

    //draw
    void render(sf::RenderWindow&);

    
};