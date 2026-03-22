#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include "imgui.h"
#include "imgui-SFML.h"

class Button{
    private:
        //statics
        bool isActive=true; //If the button is active (dim/light)
        bool isPressed=false; //If the button is clicked
        bool isHovered=false; // If the mouse is in the button

        //image
        sf::Texture texture;
        sf::Sprite sprite;

        //Color
        sf::Color normal{255,255,255,255};
        sf::Color hovered{200,200,200,255};
        sf::Color pressed{150,150,150,255};
        sf::Color notActive{150,150,150,100};
    
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