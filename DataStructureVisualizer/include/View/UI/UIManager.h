#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System/Clock.hpp>

#include "imgui.h"
#include "imgui-SFML.h"
#include "View/Widgets/Sfml/Button.h"
#include "View/Core/Theme.h"
#include "View/Panels/CodePanel.h"
#include "View/Panels/InputMenu.h"
#include "View/Widgets/Sfml/Slider.h"
#include "View/Screens/NavigationMenu.h"

class UIManager {
private:
    // state values
    bool initialized = false;
    bool isshowingPlay = true; // Determine if it's play or pause button, initially play
    Theme theme = Theme::getDefaultTheme();
    bool isMainMenu = true; // Main menu is visible?
    bool isDarkMode = false;
    bool themeToggleRequested = false;
    bool themeScaleChanged = false;
    bool backToMenuClicked = false;

    // playback memory states
    bool lastIsPlaying = false;
    bool lastIsAtBeginning = true;
    bool lastIsAtEnd = true;
    bool lastIsEmpty = true;

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
    int currentDS=-1;

    sf::SoundBuffer clickBuffer;
    sf::Sound clickSound{clickBuffer};

public:
    UIManager() = default;
    // Setup and teardown.
    // Initializes ImGui, SFML, and UI widgets.
    bool init(sf::RenderWindow&, const Theme& theme);

    // Event and layout handling.
    // Routes an SFML event into the UI layer.
    void processEvent(sf::RenderWindow&, const sf::Event&);

    // Updates UI layout for a resize event.
    void resize(const sf::RenderWindow& window);

    // Per-frame update and draw.
    // Updates UI state for the current frame.
    void update(sf::RenderWindow&, const sf::Time&);

    // Renders the UI layer.
    void render(sf::RenderWindow&);

    // Applies a theme to the UI.
    bool applyTheme(const Theme& selectedTheme);
    // Consumes a pending theme toggle request.
    bool consumeThemeToggleRequest();
    // Consumes a pending theme scaling request.
    bool consumeThemeScaleRequest();

    // Shuts down ImGui and related resources.
    void shutdown();

    // --- Interact Navigation Menu ---
    // Resets the UI to its default state.
    void reset();
    // Returns the selected data structure id.
    int getSelectedDS();
    void resetDSSelection(); 
    // Shows or hides the main menu.
    void setShowMainMenu(bool show);
    // Returns whether the main menu is visible.
    bool isShowingMainMenu() const { return isMainMenu; }
    // Returns the navigation menu state as an integer.
    int getNavMenuState() const { return static_cast<int>(navMenu.getCurrentState()); }
    // Returns the current navigation menu entries.
    std::vector<std::string> getNavMenuNames() const { return navMenu.getCurrentNames(); }
    // Returns the current navigation menu title.
    std::string getNavMenuTitle() const { return navMenu.getCurrentTitle(); }
    // Returns whether the user clicked Back to Menu.
    bool checkBackToMenuClicked();

    // Returns whether the mouse is currently over UI elements.
    bool isMouseOverUI() const;
    // Returns whether the UI is currently capturing keyboard input.
    bool isKeyboardCapturedByUI() const;

    // --- Interact InputMenu ---
    // Each DS has different input menu:
    /*
    SINGLY LINKED LIST: (0)
    1/Init: 
    2/Insert:
    3/Search
    4/Delete
    5/Update
    6/Clear

    HEAP:(1)- Min Heap (2)- Max Heap
    1/Init:
    2/Insert:
    3/ExtractTop:
    4/Peek:
    5/Search:
    6/Delete:
    7/Update:
    8/Clear:

    AVL TREE:(3)
    1/Init: 
    2/Insert:
    3/Search:
    4/Delete:
    5/Update:
    6/Clear

    SPA: (4)- Grid
    1/ Init: N=(ô nhập liệu 1), M=(ô nhập liệu 2)/ File browser
    2/ Random: N=(ô nhập liệu 1), M=(ô nhập liệu 2)
    3/ Set status: i=(ô nhập liệu 1), j=(ô nhập liệu 2)
    4/ Shortest Path: from x1=(ô nhập liệu 1), y1=(ô nhập liệu 2) to x2=(ô nhập liệu 3), y2=(ô nhập liệu 4)
    5/ Clear: nút Confirm 

    SPA: (5)- Directed Graph (6)- Undirected Graph
    1/Init:
    8/Random:
    2/Create Node:
    3/Create Edge:
    4/OPSP:
    5/SPSP:
    6/APSP:
    7/Clear
*/
    // Returns the current input action.
    int getInputAction() const;
    // Returns the current input mode.
    int getInputMode() const; 
    // Returns the first input field value.
    std::string getInputString1() const;
    // Returns the second input field value.
    std::string getInputString2() const;
    // Returns the third input field value.
    std::string getInputString3() const;
    // Returns the fourth input field value.
    std::string getInputString4() const;
    // Clears the pending input action.
    void resetInputAction();

    // --- Interact CodePanel ---
    // Sets the code panel source listing.
    void setCodePanelCode(std::vector<std::string>& code);
    // Sets the highlighted code line.
    void setCodePanelHighlightedLine(int line);
    // Sets the code panel message.
    void setCodePanelMessage(const std::string& msg);
    // Clears the code panel contents.
    void clearCodePanel();
    bool isShowingCode() const { return codePanel.isShowingCode(); }

    // --- Interact Buttons ---
    
    // Returns whether the play button was clicked.
    bool checkPlayClicked();
    // Returns whether the pause button was clicked.
    bool checkPauseClicked();
    // Returns whether the step-forward button was clicked.
    bool checkStepForwardClicked();
    // Returns whether the step-backward button was clicked.
    bool checkStepBackwardClicked();
    
    // Synchronizes the playback controls with the current timeline state.
    void syncPlaybackUI(bool currentIsPlaying, bool isAtBeginning, bool isAtEnd, bool isEmpty);


    //-Interact with Slider
    // Returns the current playback speed.
    float getSpeed() const;
    // Resets the playback speed to its default value.
    void resetSpeed();
    
    
    const Theme& getTheme() const { return theme; }
};

#endif // UIMANAGER_H
