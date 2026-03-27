#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include "imgui.h"
#include <iostream>
#include "imgui-SFML.h"
#include <cstdlib>
#include "../../include/View/InputMenu.h"

bool InputMenu::init(){
    if (!texture.loadFromFile("../assets/randombox.png",true)){
        std::cerr<<"Can't load randombox file";
        return false;
    }
    texture.setSmooth(true);
    return true;
}

void InputMenu::render(const sf::RenderWindow& window){

    //Flatting UI
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f)); 
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    ImGui::PushStyleColor(ImGuiCol_WindowBg, colorPurple);
    ImGui::PushStyleColor(ImGuiCol_Text, colorWhite);

    //Setposition for menu
    int y=window.getSize().y;
    ImGui::SetNextWindowPos(ImVec2(0,7*y/8.f-5*height ), ImGuiCond_Always);

    ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | 
                             ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove;
    
    ImGui::Begin("FlatMenu", nullptr, flags);

    //Open menu button
    ImGui::PushStyleColor(ImGuiCol_Button, colorPurple);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, colorPink);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, colorPink);

    if (ImGui::Button(isopenMenu ? "<" : ">", ImVec2(25, height))) {
        isopenMenu=!isopenMenu;
        if (!isopenMenu) currentoption=-1;
    }

    ImGui::PopStyleColor(3);

    if (isopenMenu){
        ImGui::SameLine(); 
        ImGui::BeginGroup();

        for (int i=0;i<4;++i){
            bool isActive=(currentoption==i);

            ImGui::PushStyleColor(ImGuiCol_Button, isActive ? colorPink : colorPurple);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, colorPink);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, colorPink);

            if (ImGui::Button(menu[i], ImVec2(length, height))){
                currentoption=i;
            }
            ImGui::PopStyleColor(3);

            ImVec2 currentBtnPos = ImGui::GetItemRectMin();

            if (isActive) renderinputform(window,i,currentBtnPos);
        }
    }
    ImGui::EndGroup();
}

void InputMenu::renderinputform(const sf::RenderWindow& window, int cur, ImVec2 btnPos){
    if (cur!=3){
        ImGui::SetNextWindowPos(ImVec2(btnPos.x + 120.0f, btnPos.y));
        ImGui::SetNextWindowSize(ImVec2(120, 40));
                
        ImGui::PushStyleColor(ImGuiCol_WindowBg, colorPurple);
        ImGuiWindowFlags formFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | 
                                     ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar;
                                             
        ImGui::Begin("SelectFormWindow", nullptr, formFlags);
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4.0f, 0.0f));
                
        // 1. k
        ImGui::SetCursorPos(ImVec2(4, 12)); 
        ImGui::Text("K=");
                
        // 2. Input box
        ImGui::SameLine(0, 4);   
        ImGui::SetCursorPosY(8); 
        ImGui::PushStyleColor(ImGuiCol_FrameBg, colorPink);       
        ImGui::SetNextItemWidth(30);                              
        ImGui::InputInt("##k_input", &k, 0, 0);             
        ImGui::PopStyleColor();

        // 3. Random box
        ImGui::SameLine(0, 4);
        ImGui::SetCursorPosY(5); 

        ImGui::PushStyleColor(ImGuiCol_Button, colorPink);
    
        if (ImGui::ImageButton("btn_random", texture, sf::Vector2f(30, 30))) {
            k= std::rand() % 100;  //get random number
        }
                
        // 4. OK Button
        ImGui::SameLine(0, 4);
        ImGui::SetCursorPosY(0);
        if (ImGui::Button("OK", ImVec2(34, 40))) { 
            hasAction = cur+1; 
            currentoption = -1;   // Đóng menu
        }
        ImGui::PopStyleColor(); 
        ImGui::PopStyleVar();   
        ImGui::End();
        ImGui::PopStyleColor();
    }
    else{
        ImGui::SetNextWindowPos(ImVec2(btnPos.x + 120.0f, btnPos.y));
        ImGui::SetNextWindowSize(ImVec2(240, 40));
        ImGui::PushStyleColor(ImGuiCol_WindowBg, colorPurple);
        ImGuiWindowFlags formFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | 
                                     ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar;
                                             
        ImGui::Begin("SelectFormWindow", nullptr, formFlags);
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4.0f, 0.0f));
                
        // 1. From
        ImGui::SetCursorPos(ImVec2(4, 12)); 
        ImGui::Text("From");
                
        // 2. Input box
        ImGui::SameLine(0, 4);   
        ImGui::SetCursorPosY(8); 
        ImGui::PushStyleColor(ImGuiCol_FrameBg, colorPink);       
        ImGui::SetNextItemWidth(30);                              
        ImGui::InputInt("##k_input", &k, 0, 0);             
        ImGui::PopStyleColor();

        // 3. Random box
        ImGui::SameLine(0, 4);
        ImGui::SetCursorPosY(5); 

        ImGui::PushStyleColor(ImGuiCol_Button, colorPink);
    
        if (ImGui::ImageButton("btn_random_1", texture, sf::Vector2f(30, 30))) {
            k= std::rand() % 100;  //get random number
        }
        // 4. To
        ImGui::SameLine(0, 8);
        ImGui::SetCursorPosY(12); 
        ImGui::Text("To");
                
        // 5. Input box
        ImGui::SameLine(0, 4);   
        ImGui::SetCursorPosY(8); 
        ImGui::PushStyleColor(ImGuiCol_FrameBg, colorPink);       
        ImGui::SetNextItemWidth(30);                              
        ImGui::InputInt("##r_input", &r, 0, 0);             
        ImGui::PopStyleColor();

        // 6. Random box
        ImGui::SameLine(0, 4);
        ImGui::SetCursorPosY(5); 

        ImGui::PushStyleColor(ImGuiCol_Button, colorPink);
    
        if (ImGui::ImageButton("btn_random_2", texture, sf::Vector2f(30, 30))) {
            r= std::rand() % 100;  //get random number
        }
                
        // 4. OK Button
        ImGui::SameLine(0, 4);
        ImGui::SetCursorPosY(0);
        if (ImGui::Button("OK", ImVec2(34, 40))) { 
            hasAction = cur+1; 
            currentoption = -1;   // Đóng menu
            isopenMenu=false;
        }
        ImGui::PopStyleColor(); 
        ImGui::PopStyleVar();   
        ImGui::End();
        ImGui::PopStyleColor();
    }
}