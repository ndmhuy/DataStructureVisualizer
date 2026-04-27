#ifndef POSITION_H
#define POSITION_H

#include <cmath>

struct Position {
  float x;
  float y;

  Position() : x(0.0f), y(0.0f) {}
  Position(float x, float y) : x(x), y(y) {}

  static float getDistance(const Position &a, const Position &b) {
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    return std::sqrt(dx * dx + dy * dy);
  }

  Position operator+(const Position &other) const {
    return Position(x + other.x, y + other.y);
  }

  Position operator+=(const Position &other) {
    x += other.x;
    y += other.y;
    return *this;
  }

  Position operator-(const Position &other) const {
    return Position(x - other.x, y - other.y);
  }

  Position operator-=(const Position &other) {
    x -= other.x;
    y -= other.y;
    return *this;
  }

  Position operator*(float scalar) const {
    return Position(x * scalar, y * scalar);
  }

  Position operator*=(float scalar) {
    x *= scalar;
    y *= scalar;
    return *this;
  }
};

#endif // POSITION_H
