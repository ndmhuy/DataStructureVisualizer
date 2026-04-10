#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>

#include "imgui.h"
#include "imgui-SFML.h"
#include "View/Button.h"
#include "View/Theme.h"
#include "View/CodePanel.h"
#include "View/InputMenu.h"
#include "View/Slider.h"
#include "View/NavigationMenu.h"

class UIManager {
private:
    // state values
    bool initialized = false;
    bool isPlay = true; // Determine if it's play or pause button, initially play
    Theme theme = Theme::getDefaultTheme();
    bool isMainMenu = true; // Main menu is visible?
    Button play;
    Button pause;
    Button stepForward;
    Button stepBackward;

    bool playClicked = false;
    bool pauseClicked = false;
    bool stepForwardClicked = false;
    bool stepBackwardClicked = false;

    CodePanel codePanel;
    InputMenu inputMenu;

    Slider slider;
    float speed=1.f; //init speed variable

    NavigationMenu navMenu;

public:
    UIManager() = default;
    // Setup and teardown.
    bool init(sf::RenderWindow&, const Theme& theme); // Initial set up for ImGui + SFML

    // Event and layout handling.
    void processEvent(sf::RenderWindow&, const sf::Event&); // Pass event from SFML to ImGui

    void resize(const sf::RenderWindow& window); // resize event

    // Per-frame update and draw.
    void update(sf::RenderWindow&, const sf::Time&); // Update UI status

    void render(sf::RenderWindow&); // draw UI

    void shutdown(); // shutdown ImGui

    // --- Interact Navigation Menu ---
    int getSelectedDS() const; // -1: none, 0: SSL, 1: Heap, 2: AVL, 3: SPA
    void resetDSSelection(); // Call after getSelectedDS()
    void setShowMainMenu(bool show);

    // --- Interact InputMenu ---
    int getInputAction() const; // 0: no action, 1: Add, 2: Delete, 3: Search, 4: Update.
    int getInputK() const; //Add, Delete Search need K only, Update need both K and R
    int getInputR() const;
    void resetInputAction(); //Call after done getAction

    // --- Interact CodePanel ---
    void setCodePanelCode(std::vector<std::string>& code); //pass codes
    void setCodePanelHighlightedLine(int line); //pass 0-indexed to highlight
    void clearCodePanel(); //Clean after done showing code

    // --- Interact Buttons ---
    bool checkPlayClicked();
    bool checkPauseClicked();
    bool checkStepForwardClicked();
    bool checkStepBackwardClicked();
    float getSpeed() const;
};

#endif // UIMANAGER_H
