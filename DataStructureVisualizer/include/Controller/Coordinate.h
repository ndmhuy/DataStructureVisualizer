#ifndef COORDINATE_HPP
#define COORDINATE_HPP

#include <SFML/System/Vector2.hpp>
#include <vector>

int orientation(sf::Vector2f p, sf::Vector2f q, sf::Vector2f r);

bool onSegment(sf::Vector2f p, sf::Vector2f q, sf::Vector2f r);

bool doIntersect(std::vector<sf::Vector2f>& pointSet);

#endif