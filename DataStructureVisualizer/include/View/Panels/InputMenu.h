#ifndef INPUTMENU_H
#define INPUTMENU_H

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <string>

#include "imgui-SFML.h"
#include "imgui.h"
#include "imfilebrowser.h"

#include "View/Core/Theme.h"

class InputMenu {
private:
    bool isopenMenu = false; // Is the menu visible or not.
    int hasAction = 0; // 0: no action, 1: Insert, 2: Delete, 3: Search, 4: Update, 5: Random.
    int currentOption = -1; // -1: Close, 0: Insert, 1: Delete, 2: Search, 3: Update, 4: Random.

    Theme theme = Theme::getDefaultTheme();
    bool backToMenuClicked = false;

    const char* menu[5] = {"Insert", "Random", "Delete", "Search", "Update"};

    int insertSubMode = 0; // 0: Single, 1: Array, 2: File

    int outMode = -1;
    std::string outString1 = "";
    std::string outString2 = "";

    char inputBuf1[256] = "";
    char inputBuf2[256] = "";
    
    ImGui::FileBrowser fileDialog;

public:
    InputMenu() = default;

    bool init(const Theme& theme);
    void render(const sf::RenderWindow& window);
    void renderinputform(const sf::RenderWindow& window, int cur, ImVec2 currentbttnPos);

    void applyTheme(const Theme& selectedTheme) { theme = selectedTheme; }
    int getAction() const { return hasAction; }
    int getMode() const { return outMode; }
    std::string getString1() const { return outString1; }
    std::string getString2() const { return outString2; }
    void resetAction() { hasAction = 0; }
};

#endif // INPUTMENU_H