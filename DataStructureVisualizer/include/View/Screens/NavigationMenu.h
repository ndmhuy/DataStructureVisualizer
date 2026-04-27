#ifndef NAVIGATIONMENU_H
#define NAVIGATIONMENU_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "imgui.h"
#include "View/Core/Theme.h"
#include <string>
#include <vector>

class NavigationMenu {
public:
    enum class MenuState {
        Main,
        Heap,
        ShortestPath,
        GraphType,
        GraphRepr
    };

private:
    Theme theme = Theme::getDefaultTheme();
    int selectedDS = -1; // -1: None, 0: SLL, 1: MinHeap, 2: MaxHeap, 3: AVL, 4: SPAGrid, 5: SPADirected, 6: SPAUndirected
    MenuState currentState = MenuState::Main;

    std::vector<std::string> mainNames = {
        "Singly Linked List", "Heap", 
        "AVL tree", "Shortest Path Algorithm"
    };
    std::vector<std::string> heapNames = {
        "Min Heap", "Max Heap"
    };
    std::vector<std::string> spaNames = {
        "Grid", "Graph"
    };
    std::vector<std::string> graphTypeNames = {
        "Directed Graph", "Undirected Graph"
    };
    std::vector<std::string> graphReprNames = {
        "Adjacency Matrix", "Adjacency List"
    };
    bool isDirectedGraph = true;

    sf::SoundBuffer clickBuffer;
    sf::Sound clickSound{clickBuffer};

public:
    NavigationMenu() = default;
    
    void init(const Theme& theme);
    void applyTheme(const Theme& selectedTheme) { theme = selectedTheme; }
    void render(const sf::RenderWindow& window, const sf::Vector2u& actualWindowSize);
    
    int getSelectedDS() const { return selectedDS; }
    MenuState getCurrentState() const { return currentState; }
    void resetSelection() { selectedDS = -1; }
    void resetState() { currentState = MenuState::Main; }
};

#endif // NAVIGATIONMENU_H