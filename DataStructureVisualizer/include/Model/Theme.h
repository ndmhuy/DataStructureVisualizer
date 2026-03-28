#ifndef THEME_H
#define THEME_H

#include <SFML/Graphics.hpp>
#include <string>
struct Theme {
    std::string fontPath;
    std::string nodeImagePath;
    std::string bgImagePath;
    std::string ArrayImangePath;

    sf::Color textColor;
    sf::Color nodeTintColor; // Use to change the color of node
    sf::Color arrayTintColor;
    sf::Color arrowColor;

    float nodeScale;//size
    float arrayScale;
    Theme();
};

#endif // THEME_H