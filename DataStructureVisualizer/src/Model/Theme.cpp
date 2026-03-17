#include "model/Theme.h"
Theme::Theme() {
    fontPath = "assets/arial.ttf";           
    bgImagePath = "assets/background.jpg"; 
    nodeImagePath = "assets/Node.png";       
    ArrayImangePath = "assets/array.png";

    textColor = sf::Color::Black;
    nodeTintColor = sf::Color::White; 
    arrayTintColor = sf::Color::White; 
    arrowColor = sf::Color::White;

    nodeScale = 1.0f;
    arrayScale = 1.0f;
}