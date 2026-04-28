#ifndef INPUTMENU_H
#define INPUTMENU_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System/Clock.hpp>
#include <string>

#include "imgui-SFML.h"
#include "imgui.h"
#include "imfilebrowser.h"
#include <vector>

#include "View/Core/Theme.h"

class InputMenu {
private:
    bool isopenMenu = false; // Is the menu visible or not.
    int currentDS= -1;
    int hasAction = 0; 
    int currentOption = -1; 

    Theme theme = Theme::getDefaultTheme();
    bool backToMenuClicked = false;

    std::vector<std::string> getCurrentMenu() const;

    int insertSubMode = 0; 

    int outMode = -1;
    std::string outString1 = "";
    std::string outString2 = "";
    std::string outString3 = "";
    std::string outString4 = "";

    char inputBuf1[256] = "";
    char inputBuf2[256] = "";
    char inputBuf3[256] = "";
    char inputBuf4[256] = "";
    
    ImGui::FileBrowser fileDialog;

    sf::SoundBuffer clickBuffer;
    sf::Sound clickSound{clickBuffer};

public:
    InputMenu() = default;

    // Initializes the menu with the selected theme assets.
    bool init(const Theme& theme);
    // Renders the input menu.
    void render(const sf::RenderWindow& window);
    // Renders the mode-specific input form.
    void renderinputform(const sf::RenderWindow& window, int cur, ImVec2 currentbttnPos);

    // Applies a theme to the menu.
    void applyTheme(const Theme& selectedTheme) { theme = selectedTheme; }
    // Sets the active data structure index.
    void setDS(int DS) { currentDS=DS; }
    // Returns the pending action identifier.
    int getAction() const { return hasAction; }
    // Returns the current input mode.
    int getMode() const { return outMode; }
    // Returns the first captured string.
    std::string getString1() const { return outString1; }
    // Returns the second captured string.
    std::string getString2() const { return outString2; }
    // Returns the third captured string.
    std::string getString3() const { return outString3; }
    // Returns the fourth captured string.
    std::string getString4() const { return outString4; }
    // Resets the pending action.
    void resetAction() { hasAction = 0; }
    // Resets the input state.
    void resetState() {
        isopenMenu = false;
        currentOption = -1;
        insertSubMode = 0;
        outMode = -1;
        inputBuf1[0] = '\0';
        inputBuf2[0] = '\0';
        inputBuf3[0] = '\0';
        inputBuf4[0] = '\0';
    }
};

#endif // INPUTMENU_H