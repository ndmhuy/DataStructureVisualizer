#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>

class Slider{
    private:

    // the changeable value using slider
        float* speed; //range 0.5x->4.0x
    
    // Some statics variable
        float start; //start point of the slider
        float end;   //end point of the slider
        float radius; // radius of all CircleShape below

    // Some status variable:
        bool isDragging;


    // track
        sf::RectangleShape track;
        sf::CircleShape semiCircle1; //faked rounded edges
        sf::CircleShape semiCircle2;
        sf::Color trackColor{sf::Color::White};

    // Knob
        sf::CircleShape knob;
        sf::Color knobColor{30, 144, 255};
        sf::Vector2f knobpos;

    // Text- demostrate speed
        sf::Font font;
        sf::Text text;

    public:

    Slider() : text(font) {}

    // init slider
    void init(float* value);

    //resize event
    void resize(const sf::RenderWindow& window);

    //Handle event
    void handleEvent(const sf::RenderWindow& window, const sf::Event& event);

    //draw
    void render(sf::RenderWindow& window);
    
};

// when the speed is changed-> used this to change the text
void update(float speed, sf::Text& text);