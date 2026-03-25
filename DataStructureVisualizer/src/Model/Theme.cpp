#include "model/Theme.h"
Theme::Theme() {
fontPath = "../assets/retro-pixel-thick.ttf";           
    bgImagePath = "../assets/background.png"; 
    nodeImagePath = "../assets/Node.png";       
    ArrayImangePath = "../assets/array.png";

    textColor = sf::Color({0,71,171});
    nodeTintColor = sf::Color::White; 
    arrayTintColor = sf::Color::White; 
    arrowColor = sf::Color({0,71,171});

    nodeScale = 1.0f;
    arrayScale = 1.0f;
}