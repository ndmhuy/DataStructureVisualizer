#include "View/Renderer.h"
Renderer::Renderer(sf::RenderWindow& m_Window,const std::string& path): Window(m_Window),fontchar(path){}
bool Renderer::loadfont(std::string& path){
    if(!fontchar.openFromFile(path)){
        std::cerr<<"Cannot load font from "<<path<<std::endl;
        return false;
    }
    return true;
}
void Renderer::drawNode(float x,float y, float radius,const std::string& text, sf::Color color){
    //setup a circle
    sf::CircleShape circle(radius);
    circle.setFillColor(color);
    circle.setOutlineThickness(3.0f);
    circle.setOutlineColor({0,255,255});
    circle.setOrigin({radius,radius});
    circle.setPosition({x,y});
    //write the number inside
    sf::Text a(fontchar,text,20);
    a.setFillColor(sf::Color::Black);
    sf::FloatRect textRect =a.getGlobalBounds();
    a.setOrigin({(textRect.position.x+textRect.size.x)/2.0f,(textRect.position.y+textRect.size.y)/2.0f});
    a.setPosition({sf::Vector2f(x,y)});
    Window.draw(circle);
    Window.draw(a);
}
void drawArray(float x,float y,float height, float width,std::string& text,sf::Color color);
