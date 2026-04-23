#ifndef POSITION_H
#define POSITION_H

struct Position {
    float x;
    float y;

    Position() : x(0.0f), y(0.0f) {}
    Position(float x, float y) : x(x), y(y) {}
};

#endif // POSITION_H
