#ifndef INPUTMENU_H
#define INPUTMENU_H

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>

#include "imgui-SFML.h"
#include "imgui.h"

#include "View/Theme.h"

class InputMenu {
private:
    bool isopenMenu = false; // Is the menu visible or not.
    int hasAction = 0; // 0: no action, 1: Add, 2: Delete, 3: Search, 4: Update.
    int currentOption = -1; // -1: Close, 0: Add, 1: Delete, 2: Search, 3: Update.

    Theme theme = Theme::getDefaultTheme();
    bool backToMenuClicked = false;

    const char* menu[4] = {"Insert", "Delete", "Search", "Update"};

    sf::Texture texture;

    int k = 0; // Use for add, delete, search, update.
    int r = 0; // Use for update.

public:
    InputMenu() = default;

    bool init(const Theme& theme);
    void render(const sf::RenderWindow& window);
    void renderinputform(const sf::RenderWindow& window, int cur, ImVec2 currentbttnPos);

    void applyTheme(const Theme& selectedTheme) { theme = selectedTheme; }
    int getAction() const { return hasAction; }
    int getK() const { return k; }
    int getR() const { return r; }
    void resetAction() { hasAction = 0; }
};

#endif // INPUTMENU_H