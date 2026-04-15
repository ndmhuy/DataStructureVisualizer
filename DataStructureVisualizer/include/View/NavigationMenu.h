#ifndef NAVIGATIONMENU_H
#define NAVIGATIONMENU_H

#include <SFML/Graphics.hpp>
#include "imgui.h"
#include "View/Theme.h"
#include <string>
#include <vector>

class NavigationMenu {
private:
    Theme theme = Theme::getDefaultTheme();
    int selectedDS = -1; // -1: None, 0: Singly Linked List, 1: Heap, 2: AVL Tree, 3: Shortest Path
    std::vector<std::string> dsNames = {
        "Singly Linked List", "Heap", 
        "AVL tree", "Shortest Path Algorithm"
    };

public:
    NavigationMenu() = default;
    
    void init(const Theme& theme);
    void applyTheme(const Theme& selectedTheme) { theme = selectedTheme; }
    void render(const sf::RenderWindow& window);
    
    int getSelectedDS() const { return selectedDS; }
    void resetSelection() { selectedDS = -1; }
};

#endif // NAVIGATIONMENU_H