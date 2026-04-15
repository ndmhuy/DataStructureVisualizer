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
    bool isshowingPlay = true; // Determine if it's play or pause button, initially play
    Theme theme = Theme::getDefaultTheme();
    bool isMainMenu = true; // Main menu is visible?
    bool isDarkMode = false;
    bool backToMenuClicked = false;
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
    bool checkBackToMenuClicked(); // check if User back to Main menu

    // --- Interact InputMenu ---
    int getInputAction() const; // 0: no action, 1: Insert, 2: Delete, 3: Search, 4: Update, 5: Random.
    int getInputMode() const; //Insert: 0. Single 1. Array 2. File //Random: 0.Single 1.Array //Delete,Search,Update: 0
    std::string getInputString1() const;
    std::string getInputString2() const; //Only get if Action 4, Mode 0
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
    
    void syncPlaybackUI(bool currentIsPlaying, bool isAtBeginning, bool isAtEnd, bool isEmpty);


    //-Interact with Slider
    float getSpeed() const;
    void resetSpeed();// Call after finishing a DS
    
    
    const Theme& getTheme() const { return theme; }
};

#endif // UIMANAGER_H
