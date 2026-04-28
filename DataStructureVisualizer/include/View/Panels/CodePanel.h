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

    void applyTheme(const Theme& selectedTheme);
    void setCode(std::vector<std::string>& setcodes);
    void setHighlightedLine(int numline);
    void setMessage(const std::string& msg);
    void clearCode();
    void resize(const sf::RenderWindow& window);
    void render(const sf::RenderWindow& window);

    bool isShowingCode() const { return showCode; }
    void toggleShowCode() { showCode = !showCode; }
};

#endif // CODEPANEL_H