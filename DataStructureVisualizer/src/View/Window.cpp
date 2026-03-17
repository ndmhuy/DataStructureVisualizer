#include "view/Window.h"
Window::Window() :RenderWindow(sf::VideoMode({1280,720}),"Data Structure Visualization",sf::Style::Default){
RenderWindow.setFramerateLimit(60);
}
void Window::HandleEvent(){
    while(const std::optional<sf::Event> a=RenderWindow.pollEvent()){
        if(a->is<sf::Event::Closed>()){
            RenderWindow.close();
        }
    }
}
void Window::clear(){
    RenderWindow.clear(sf::Color({40,44,52}));
}
void Window::display(){
    RenderWindow.display();
}
bool Window::IsOpen() const{
    return RenderWindow.isOpen();
}
sf::RenderWindow& Window::GetWindow(){
    return RenderWindow;
}