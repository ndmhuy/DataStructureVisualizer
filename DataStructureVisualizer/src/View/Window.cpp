#include "View/Window.h"

Window::Window() : renderWindow(sf::VideoMode({1920, 1080}), "Data Structure Visualization", sf::Style::Default) {
    renderWindow.setFramerateLimit(60);
}

void Window::handleEvent() {
    while (std::optional<sf::Event> event = renderWindow.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            renderWindow.close();
        }
    }
}

void Window::clear(const sf::Color& clearColor) {
    renderWindow.clear(clearColor);
}

void Window::display() {
    renderWindow.display();
}

bool Window::isOpen() const {
    return renderWindow.isOpen();
}

sf::RenderWindow& Window::getWindow() {
    return renderWindow;
}