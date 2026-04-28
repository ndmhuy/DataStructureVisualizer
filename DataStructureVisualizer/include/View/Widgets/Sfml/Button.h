#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>

#include "View/Core/Theme.h"

class Button {
private:
    Theme theme = Theme::getDefaultTheme();
    // status
    bool isActive = true;   // If the button is active (dim/light)
    bool isPressed = false; // If the button is clicked
    bool isHovered = false; // If the mouse is in the button

    // image
    sf::Texture texture;
    sf::Sprite sprite{texture};

    // dimensions
    sf::Vector2f pos; // center position
    float rad = 0.0f;

public:
    // create a button with image
    // Loads the button texture and applies the theme.
    bool init(const std::string& imagepath, const Theme& selectedTheme);

    // determine if the button is active or not
    // Enables or disables button interaction.
    void setActive(bool);

    // resize event
    // Updates the button bounds for the new position and radius.
    void resize(sf::Vector2f position, float radius);

    // detect if the button is clicked
    // Handles mouse and keyboard events for the button.
    bool handleEvent(const sf::RenderWindow&, const sf::Event&);

    // draw
    // Renders the button.
    void render(sf::RenderWindow&);
};

#endif // BUTTON_H
