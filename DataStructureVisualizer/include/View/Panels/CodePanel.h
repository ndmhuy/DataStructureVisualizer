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
    int highlightedline = -1; // 0-based; -1 means no highlighted line.
    Theme theme = Theme::getDefaultTheme();

public:
    CodePanel() = default;

    void applyTheme(const Theme& selectedTheme);
    void setCode(std::vector<std::string>& setcodes);
    void setHighlightedLine(int numline);
    void clearCode();
    void render(const sf::RenderWindow& window);
};

#endif // CODEPANEL_H