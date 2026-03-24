#include <cmath>
#include <sstream>
#include <iomanip>
#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include "../../include/View/Slider.h"

void Slider::init(float* value){
    speed=value;
    isDragging=false;
    track.setFillColor(trackColor);
    semiCircle1.setFillColor(trackColor);
    semiCircle2.setFillColor(trackColor);
    knob.setFillColor(knobColor);
    //shown text
    *speed=round(10*(*speed))/10;
    if (!font.openFromFile("../assets/retro-pixel-thick.ttf")){
        std::cerr<<"Can't load font for speed text!";
        return;
    }
    text.setFont(font);
    text.setFillColor(sf::Color::Black);
    // set initial string
    std::stringstream ss;
    ss << std::fixed << std::setprecision(1) << *speed << "x";
    text.setString(ss.str());
}

void Slider::resize(const sf::RenderWindow& window){
    //calculate radius, points..
    int x=window.getSize().x;
    int y=window.getSize().y;
    radius=std::round(y/90.f);
    start=3*x/5.f;
    end=9*x/10.f;

    //set size
    track.setSize(sf::Vector2f{(end-start),radius*2});
    semiCircle1.setRadius(radius);
    semiCircle2.setRadius(radius);
    knob.setRadius(radius);
    text.setCharacterSize(radius*2);

    //set origin
    track.setOrigin(sf::Vector2f{(end-start)/2.f,radius});
    semiCircle1.setOrigin(sf::Vector2f{radius,radius});
    semiCircle2.setOrigin(sf::Vector2f{radius,radius});
    knob.setOrigin(sf::Vector2f{radius,radius});

    // set origin for text
    sf::FloatRect textRect = text.getLocalBounds();

    text.setOrigin(sf::Vector2f{
        textRect.position.x + textRect.size.x / 2.0f,
        textRect.position.y + textRect.size.y / 2.0f
    });

    //set position
    track.setPosition(sf::Vector2f{3*x/4.f,9*y/10.f});
    semiCircle1.setPosition(sf::Vector2f{start,9*y/10.f});
    semiCircle2.setPosition(sf::Vector2f{end,9*y/10.f});
    text.setPosition(sf::Vector2f{19*x/20.f,9*y/10.f});
    // update position of knob based on speed
    knobpos.y=9*y/10.f;
    knobpos.x=start+(*speed-0.5)/3.5*(end-start);
    knob.setPosition(knobpos);
}

void Slider::handleEvent(const sf::RenderWindow& window, const sf::Event& event){
    //when released the mouse
    if (isDragging&&!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)){
        isDragging=false;
    }
    
    sf::Vector2i mousePixelPos = sf::Mouse::getPosition(window);
    sf::Vector2f mouseWorldPos = window.mapPixelToCoords(mousePixelPos);
    if (isDragging){
        if (event.getIf<sf::Event::MouseMoved>()){
            //logic moving mouse
            knobpos.x=mouseWorldPos.x;
            if (knobpos.x>end) knobpos.x=end;
            else if (knobpos.x<start) knobpos.x=start;
            //update position of knob
            knob.setPosition(knobpos);
            //logic update speed
            *speed=0.5+(knobpos.x-start)/(end-start)*3.5;
            //round 1 decimal place
            *speed=round(10*(*speed))/10;
            update(*speed,text);
        }
    }
    else{
        float dx=mouseWorldPos.x-knobpos.x;
        float dy=mouseWorldPos.y-knobpos.y;
        if (dx*dx+dy*dy<=radius*radius){
            const auto* mouseEvent = event.getIf<sf::Event::MouseButtonPressed>();
            if (mouseEvent&&mouseEvent->button== sf::Mouse::Button::Left) {
                isDragging=true;
            }
        }
    }
}

void Slider::render(sf::RenderWindow& window){
    window.draw(semiCircle1);
    window.draw(semiCircle2);
    window.draw(track);
    window.draw(knob);
    window.draw(text);
}






// when the speed is changed-> used this to change the text
void update(float speed, sf::Text& text){
    sf::Vector2f pos = text.getPosition();

    std::stringstream ss;
    ss << std::fixed << std::setprecision(1) << speed << "x";
    
    text.setString(ss.str());

    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(sf::Vector2f{
        textRect.position.x + textRect.size.x / 2.0f,
        textRect.position.y + textRect.size.y / 2.0f
    });
    text.setPosition(pos);
}