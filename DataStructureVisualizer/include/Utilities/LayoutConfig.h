#ifndef LAYOUTCONFIG_H
#define LAYOUTCONFIG_H

struct LayoutConfig {
    float screenWidth = 1600.0f;
    float screenHeight = 900.0f;
    float padding = 100.0f;

    // Graph/structure viewport padding behavior.
    float layoutPaddingRatio = 0.08f;
    float layoutPaddingMin = 40.0f;
    float layoutPaddingMax = 120.0f;

    // Grid defaults and responsive viewport behavior.
    float gridTargetCellPixels = 90.0f;
    int gridDefaultRowsMin = 8;
    int gridDefaultRowsMax = 18;
    int gridDefaultColsMin = 8;
    int gridDefaultColsMax = 28;

    float gridHorizontalPaddingRatio = 0.08f;
    float gridHorizontalPaddingMin = 24.0f;
    float gridTopPaddingRatio = 0.14f;
    float gridTopPaddingMin = 80.0f;
    float gridBottomPaddingRatio = 0.10f;
    float gridBottomPaddingMin = 40.0f;
    float gridViewportMinSize = 40.0f;

    int physicsIterations = 100;
    float initialTemperature = 100.0f;
    float coolingFactor = 0.95f;
};

#endif // LAYOUTCONFIG_H