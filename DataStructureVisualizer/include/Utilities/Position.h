#ifndef POSITION_H
#define POSITION_H

#include <cmath>

struct Position {
  float x;
  float y;

  // Creates a position at the origin.
  Position() : x(0.0f), y(0.0f) {}
  // Creates a position from x and y coordinates.
  Position(float x, float y) : x(x), y(y) {}

  // Returns the Euclidean distance between two positions.
  static float getDistance(const Position &a, const Position &b) {
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    return std::sqrt(dx * dx + dy * dy);
  }

  // Adds two positions component-wise.
  Position operator+(const Position &other) const {
    return Position(x + other.x, y + other.y);
  }

  // Adds another position in place and returns the result.
  Position operator+=(const Position &other) {
    x += other.x;
    y += other.y;
    return *this;
  }

  // Subtracts another position component-wise.
  Position operator-(const Position &other) const {
    return Position(x - other.x, y - other.y);
  }

  // Subtracts another position in place and returns the result.
  Position operator-=(const Position &other) {
    x -= other.x;
    y -= other.y;
    return *this;
  }

  // Multiplies both coordinates by a scalar.
  Position operator*(float scalar) const {
    return Position(x * scalar, y * scalar);
  }

  // Multiplies both coordinates in place and returns the result.
  Position operator*=(float scalar) {
    x *= scalar;
    y *= scalar;
    return *this;
  }
};

#endif // POSITION_H
