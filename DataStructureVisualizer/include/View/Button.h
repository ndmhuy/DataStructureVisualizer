#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>

class Button{
    private:
        //status
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
        sf::Color notActive{100,100,100,255};

        //statistic
        sf::Vector2f pos; //centre pos
        float rad; //radius
    
    public:

    Button() : sprite(texture) {}

    //create a button with image
    bool init(const std::string& imagepath);

    //determine if the Button is active or not
    void setActive(bool);

    //resize event
    void resize(sf::Vector2f position, float radius);

    //Address if the button is clicked
    bool handleEvent(const sf::RenderWindow&, const sf::Event& );

    //draw
    void render(sf::RenderWindow&);


};