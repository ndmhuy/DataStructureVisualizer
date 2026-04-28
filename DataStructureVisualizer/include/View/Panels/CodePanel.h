#ifndef CODEPANEL_H
#define CODEPANEL_H

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>

#include <string>
#include <vector>

#include "View/Core/Theme.h"
#include "imgui-SFML.h"
#include "imgui.h"

class CodePanel {
private:
    std::vector<std::string> listofCodes; // Store code lines.
    std::string currentMessage;           // Store frame message.
    int highlightedline = -1; // 0-based; -1 means no highlighted line.
    int previousHighlightedLine = -1;
    bool isAutoScrolling = false;
    float targetScrollY = 0.0f;
    float animHighlightY = 0.0f;
    float animHighlightAlpha = 0.0f;
    Theme theme = Theme::getDefaultTheme();
    float panelHeight = 400.0f; // Chiều cao cố định của Code Panel
    bool showCode = true;

public:
    CodePanel() = default;

    // Applies a theme to the panel.
    void applyTheme(const Theme& selectedTheme);
    // Sets the code listing to display.
    void setCode(std::vector<std::string>& setcodes);
    // Updates the highlighted line.
    void setHighlightedLine(int numline);
    // Updates the visible message.
    void setMessage(const std::string& msg);
    // Clears the current code listing.
    void clearCode();
    // Resizes the panel for the current window.
    void resize(const sf::RenderWindow& window);
    // Renders the panel.
    void render(const sf::RenderWindow& window);

    // Returns whether code is currently shown.
    bool isShowingCode() const { return showCode; }
    // Toggles code visibility.
    void toggleShowCode() { showCode = !showCode; }
};

#endif // CODEPANEL_H