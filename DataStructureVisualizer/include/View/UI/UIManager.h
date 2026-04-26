#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <SFML/Graphics.hpp>
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

    bool applyTheme(const Theme& selectedTheme);
    bool consumeThemeToggleRequest();
    bool consumeThemeScaleRequest();

    void shutdown(); // shutdown ImGui

    // --- Interact Navigation Menu ---
    void reset(); // Khôi phục toàn bộ UI về trạng thái mặc định (Gọi khi Back to Menu)
    int getSelectedDS(); // -1: none, 0: SSL, 1: Min Heap, 2:Max Heap, 3: AVL Tree, 4: Grid, 5: ADJ Matrix, 6: ADJ List
    void resetDSSelection(); 
    void setShowMainMenu(bool show);
    bool checkBackToMenuClicked(); // check if User back to Main menu

    bool isMouseOverUI() const;
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
    4/ BFS: from x1=(ô nhập liệu 1), y1=(ô nhập liệu 2) to x2=(ô nhập liệu 3), y2=(ô nhập liệu 4)
    5/ Clear: nút Confirm 

    SPA: (5)- ADJ Matrix (6)- ADJ List
    1/Init:
    8/Random:
    2/Create Node:
    3/Create Edge:
    4/OPSP:
    5/SPSP:
    6/APSP:
    7/Clear
*/
    int getInputAction() const;
    int getInputMode() const; 
    std::string getInputString1() const;
    std::string getInputString2() const;
    std::string getInputString3() const;
    std::string getInputString4() const;
    void resetInputAction(); //Call after done getAction

    // --- Interact CodePanel ---
    void setCodePanelCode(std::vector<std::string>& code); //pass codes
    void setCodePanelHighlightedLine(int line); //pass 0-indexed to highlight
    void setCodePanelMessage(const std::string& msg); //pass message text
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
