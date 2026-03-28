#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <vector>
#include <string>
#include "imgui.h"
#include "imgui-SFML.h"

class CodePanel{
    private:
        std::vector<std::string> listofCodes;//Store Codes

        int highlightedline=-1; //0-indexbase, -1 means not highlighting

        //Color
        ImU32 bgColor     = IM_COL32(250, 250, 250, 255);    // Off-white for less eye strain
        ImU32 borderColor = IM_COL32(40, 40, 40, 255);       // Dark Grey / Almost Black
        ImU32 textColor   = IM_COL32(40, 40, 40, 255);       // Dark Grey text
        ImU32 titleColor  = IM_COL32(0, 0, 0, 255);          // Black title text
        ImU32 highlightColor = IM_COL32(230, 50, 80, 255);   // Vivid Red-Pink

    public:

        CodePanel()=default; //constructor

        void setCode(std::vector<std::string>& setcodes); //receive codes from somewhere

        void setHighlightedLine(int numline); //set line 0-index based

        //Call when finish code showing
        void clearCode();// clear memory for next showing code

        void render(const sf::RenderWindow& window); //draw
        
};