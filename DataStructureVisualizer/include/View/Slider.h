#ifndef SLIDER_H
#define SLIDER_H

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>

#include "View/Theme.h"

class Slider {
private:
    float* speed; // Range 0.5x -> 4.0x.
    Theme theme = Theme::getDefaultTheme();

    float start; // Start point of the slider.
    float end; // End point of the slider.
    float radius; // Radius of the circle shapes below.

    bool isDragging;

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

    void init(float* value, const Theme& theme);
    void applyTheme(const Theme& selectedTheme);
    void setValue(float val);
    void resize(const sf::RenderWindow& window);
    void handleEvent(const sf::RenderWindow& window, const sf::Event& event);
    void render(sf::RenderWindow& window);
};

// When the speed changes, update the displayed text.
void update(float speed, sf::Text& text);

#endif // SLIDER_H