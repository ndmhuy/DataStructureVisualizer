#ifndef LAYOUTCONFIG_H
#define LAYOUTCONFIG_H

struct LayoutConfig {
    float screenWidth = 1600.0f;
    float screenHeight = 900.0f;
    float padding = 100.0f;

    int physicsIterations = 100;
    float initialTemperature = 100.0f;
    float coolingFactor = 0.95f;
};

#endif // LAYOUTCONFIG_H