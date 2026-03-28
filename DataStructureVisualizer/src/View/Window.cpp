#include "View/Window.h"

Window::Window() : RenderWindow(sf::VideoMode({1920, 1080}), "Data Structure Visualization", sf::Style::Default) {
    RenderWindow.setFramerateLimit(60);
}

void Window::HandleEvent() {
    while (std::optional<sf::Event> event = RenderWindow.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            RenderWindow.close();
        }
    }
}

void Window::clear() {
    RenderWindow.clear(sf::Color({40, 44, 52}));
}

void Window::display() {
    RenderWindow.display();
}

bool Window::IsOpen() const {
    return RenderWindow.isOpen();
}

sf::RenderWindow& Window::GetWindow() {
    return RenderWindow;
}