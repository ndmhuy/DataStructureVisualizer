#ifndef WINDOW_H
#define WINDOW_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <optional>

class Window {
private:
    sf::RenderWindow renderWindow;

public:
    // Lifecycle and access.
    // Initializes the window with a size, title, and framerate limit.
    Window();
    // Returns the underlying SFML window.
    sf::RenderWindow& getWindow();

    // Frame loop helpers.
    // Polls and handles pending window events.
    void handleEvent();
    // Clears the window using the provided color.
    void clear(const sf::Color& clearColor);
    // Presents the rendered frame.
    void display();

    // Window state.
    // Returns whether the window is currently open.
    bool isOpen() const;
};

#endif // WINDOW_H