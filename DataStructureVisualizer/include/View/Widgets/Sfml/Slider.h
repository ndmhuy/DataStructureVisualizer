#ifndef SLIDER_H
#define SLIDER_H

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>

#include "View/Core/Theme.h"

class Slider {
private:
    float* speed; // Range 0.5x -> 4.0x.
    Theme theme = Theme::getDefaultTheme();

    float start; // Start point of the slider.
    float end; // End point of the slider.
    float radius; // Radius of the circle shapes below.

    bool isDragging;
    bool isHovered = false;
    float animScale = 1.0f;
    float textScale = 1.0f;
    float lastSpeed = -1.0f;

    sf::RectangleShape track;
    sf::CircleShape semiCircle1; // Faked rounded edges.
    sf::CircleShape semiCircle2;
    sf::Color trackColor;

    sf::CircleShape knob;
    sf::Color knobColor;
    sf::Vector2f knobpos;

    sf::Font font;
    sf::Text text;

public:
    Slider() : text(font) {}

    // Initializes the slider with a value pointer and theme.
    void init(float* value, const Theme& theme);
    // Applies a new theme to the slider.
    void applyTheme(const Theme& selectedTheme);
    // Sets the current slider value.
    void setValue(float val);
    // Updates the slider geometry for the current window.
    void resize(const sf::RenderWindow& window);
    // Handles slider interaction events.
    void handleEvent(const sf::RenderWindow& window, const sf::Event& event);
    // Renders the slider.
    void render(sf::RenderWindow& window);
};

// When the speed changes, update the displayed text.
// Updates the text label that shows the current speed.
void update(float speed, sf::Text& text);

#endif // SLIDER_H