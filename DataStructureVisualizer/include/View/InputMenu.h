#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include "imgui.h"
#include "imgui-SFML.h"

class InputMenu{
    private:
        //static variable
        bool isopenMenu=false;// is the menu visible or not

        int hasAction=0; //0: no action, 1:Add, 2:Delete, 3: Search, 4: Update

        int currentoption=-1; //-1: Close, 0: Add, 1: Delete, 2: Search, 3: Update
        
        const char* menu[4]={"Insert","Delete","Search","Update"};

        // Color
        ImVec4 colorPurple = ImVec4(0.31f, 0.28f, 0.59f, 1.0f); 
        ImVec4 colorPink   = ImVec4(1.0f, 0.58f, 0.81f, 1.0f); 
        ImVec4 colorWhite  = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

        //size
        int height=60;
        int length=180;

        //texture for randombox
        sf::Texture texture;

        //variables
        int k=0; //use for Add, delete, search, Update
        int r=0; //use for Update

    public:
        InputMenu()=default;//constructor

        //load randombox file
        bool init();

        void render(const sf::RenderWindow& window);// draw

        void renderinputform(const sf::RenderWindow& window, int cur, ImVec2 currentbttnPos);

        //functions for other branch
        int getAction() const { return hasAction; }//0: no action, 1:Add, 2:Delete, 3: Search, 4: Update
        
        int getK() const { return k; }// use for get the 1st number

        int getR() const { return r; }// use for get the second number (for Updating)

        void resetAction() { hasAction = 0; } //Call after get the Action
};