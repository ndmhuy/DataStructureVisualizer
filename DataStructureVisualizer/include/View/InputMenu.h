#ifndef INPUTMENU_H
#define INPUTMENU_H

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <string>

#include "imgui-SFML.h"
#include "imgui.h"
#include "imfilebrowser.h"
#include <vector>

#include "View/Theme.h"

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

public:
    InputMenu() = default;

    bool init(const Theme& theme);
    void render(const sf::RenderWindow& window);
    void renderinputform(const sf::RenderWindow& window, int cur, ImVec2 currentbttnPos);

    void applyTheme(const Theme& selectedTheme) { theme = selectedTheme; }
    void setDS(int DS) { currentDS=DS; }
    int getAction() const { return hasAction; }
    int getMode() const { return outMode; }
    std::string getString1() const { return outString1; }
    std::string getString2() const { return outString2; }
    std::string getString3() const { return outString3; }
    std::string getString4() const { return outString4; }
    void resetAction() { hasAction = 0; }
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