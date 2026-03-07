#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <optional>
class Window{
    private:
    sf::RenderWindow RenderWindow;
    public:
    Window();//Initializes the window with a size, title, and sets the framerate limit.
    void HandleEvent();//Polls for events (like closing the window).
    void clear();//Clears the window screen with a specific color.
    void display();//Displays what has been rendered to the window.
    bool IsOpen() const;//Checks if the window is currently open.
};