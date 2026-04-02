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
    Window(); // Initializes the window with a size, title, and sets the framerate limit.
    sf::RenderWindow& getWindow();

    // Frame loop helpers.
    void handleEvent(); // Polls for events (like closing the window).
    void clear(const sf::Color& clearColor); // Clears the window screen with a specific color.
    void display();     // Displays what has been rendered to the window.

    // Window state.
    bool isOpen() const; // Checks if the window is currently open.
};

#endif // WINDOW_H