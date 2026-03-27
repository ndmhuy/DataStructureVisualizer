#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include "imgui.h"
#include <iostream>
#include "imgui-SFML.h"
#include <cstdlib>
#include "../../include/View/InputMenu.h"

bool InputMenu::init(){
    // Load randombox file
    if (!texture.loadFromFile("../assets/randombox.png", true)){
        std::cerr << "Can't load randombox file";
        return false;
    }
    texture.setSmooth(true);
    return true;
}

void InputMenu::render(const sf::RenderWindow& window){

    // Flattening UI for all upcoming windows
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f)); 
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    ImGui::PushStyleColor(ImGuiCol_WindowBg, colorPurple);
    ImGui::PushStyleColor(ImGuiCol_Text, colorWhite);

    // Set position for menu
    int y = window.getSize().y;
    ImGui::SetNextWindowPos(ImVec2(0, 7 * y / 8.f - 5 * height), ImGuiCond_Always);

    ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | 
                             ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove;
    
    // Begin Main Menu Window
    ImGui::Begin("FlatMenu", nullptr, flags);

    // Open menu button
    ImGui::PushStyleColor(ImGuiCol_Button, colorPurple);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, colorPink);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, colorPink);

    // Increased button width to 35 to match the 40% scale-up
    if (ImGui::Button(isopenMenu ? "<" : ">", ImVec2(35, height))) {
        isopenMenu = !isopenMenu;
        if (!isopenMenu) currentoption = -1;
    }
    ImGui::PopStyleColor(3);

    // Temporary variables to remember where to draw the floating form
    int activeIndex = -1;
    ImVec2 activeBtnPos;

    if (isopenMenu){
        ImGui::SameLine(); 
        ImGui::BeginGroup();

        for (int i = 0; i < 4; ++i){
            bool isActive = (currentoption == i);

            ImGui::PushStyleColor(ImGuiCol_Button, isActive ? colorPink : colorPurple);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, colorPink);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, colorPink);

            if (ImGui::Button(menu[i], ImVec2(length, height))){
                currentoption = i;
            }
            ImGui::PopStyleColor(3);

            // Remember the position, but DO NOT render the form inside this group
            if (isActive) {
                activeIndex = i;
                activeBtnPos = ImGui::GetItemRectMin();
            }
        }
        ImGui::EndGroup();
    }
    ImGui::End(); // Completely close the Main Menu window
    
    // Render the floating form independently
    if (activeIndex != -1) {
        renderinputform(window, activeIndex, activeBtnPos);
    }

    // Restore Style (Applies to both Menu and Floating Form)
    ImGui::PopStyleColor(2); 
    ImGui::PopStyleVar(4);
}

void InputMenu::renderinputform(const sf::RenderWindow& window, int cur, ImVec2 btnPos){
    // 1. Calculate the form width dynamically to ensure enough space
    float formWidth = (cur != 3) ? (length * 1.05f) : (length * 1.9f); 
    
    // 2. Snap the form to the right edge of the active menu button
    ImGui::SetNextWindowPos(ImVec2(btnPos.x + length, btnPos.y)); 
    ImGui::SetNextWindowSize(ImVec2(formWidth, height));
    
    ImGuiWindowFlags formFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | 
                                 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar;
                                         
    ImGui::Begin("SelectFormWindow", nullptr, formFlags);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(6.0f, 0.0f)); // Increase spacing for breathability
    
    //Centre
    float textY    = (height - ImGui::GetTextLineHeight()) / 2.0f;
    float inputY   = (height - ImGui::GetFrameHeight()) / 2.0f;
    float iconSize = height * 0.65f; // Icon is 65% of form height
    float iconY    = (height - iconSize) / 2.0f;
    float okBtnH   = height * 0.8f;  // OK button is 80% of form height
    float okBtnY   = (height - okBtnH) / 2.0f;

    if (cur != 3){ // Form for Insert, Delete, Search
        // 1. K text
        ImGui::SetCursorPos(ImVec2(8, textY)); 
        ImGui::Text("K=");
                
        // 2. Input box
        ImGui::SameLine();   
        ImGui::SetCursorPosY(inputY); 
        ImGui::PushStyleColor(ImGuiCol_FrameBg, colorPink);       
        ImGui::SetNextItemWidth(45); // Wider input box for better visibility                              
        ImGui::InputInt("##k_input", &k, 0, 0);             
        ImGui::PopStyleColor();

        // 3. Random box
        ImGui::SameLine();
        ImGui::SetCursorPosY(iconY); 
        ImGui::PushStyleColor(ImGuiCol_Button, colorPink);
        if (ImGui::ImageButton("btn_random", texture, sf::Vector2f(iconSize, iconSize))) {
            k = std::rand() % 100;
        }
                
        // 4. OK Button
        ImGui::SameLine();
        ImGui::SetCursorPosY(okBtnY);
        if (ImGui::Button("OK", ImVec2(45, okBtnH))) { 
            hasAction = cur + 1; 
            currentoption = -1;
            isopenMenu=false;  
        }
        ImGui::PopStyleColor(); // Pop for both Random and OK Button
    }
    else { // Form for Update (From and To)
        // 1. From text
        ImGui::SetCursorPos(ImVec2(8, textY)); 
        ImGui::Text("From");
                
        // 2. Input box
        ImGui::SameLine();   
        ImGui::SetCursorPosY(inputY); 
        ImGui::PushStyleColor(ImGuiCol_FrameBg, colorPink);       
        ImGui::SetNextItemWidth(45);                              
        ImGui::InputInt("##k_input", &k, 0, 0);             
        ImGui::PopStyleColor();

        // 3. Random box
        ImGui::SameLine();
        ImGui::SetCursorPosY(iconY); 
        ImGui::PushStyleColor(ImGuiCol_Button, colorPink);
        if (ImGui::ImageButton("btn_random_1", texture, sf::Vector2f(iconSize, iconSize))) {
            k = std::rand() % 100;
        }
        ImGui::PopStyleColor();

        // 4. To text
        ImGui::SameLine(0, 12); // Extra spacing before 'To'
        ImGui::SetCursorPosY(textY); 
        ImGui::Text("To");
                
        // 5. Input box
        ImGui::SameLine();   
        ImGui::SetCursorPosY(inputY); 
        ImGui::PushStyleColor(ImGuiCol_FrameBg, colorPink);       
        ImGui::SetNextItemWidth(45);                              
        ImGui::InputInt("##r_input", &r, 0, 0);             
        ImGui::PopStyleColor();

        // 6. Random box
        ImGui::SameLine();
        ImGui::SetCursorPosY(iconY); 
        ImGui::PushStyleColor(ImGuiCol_Button, colorPink);
        if (ImGui::ImageButton("btn_random_2", texture, sf::Vector2f(iconSize, iconSize))) {
            r = std::rand() % 100; 
        }
                
        // 7. OK Button
        ImGui::SameLine();
        ImGui::SetCursorPosY(okBtnY);
        if (ImGui::Button("OK", ImVec2(45, okBtnH))) { 
            hasAction = cur + 1; 
            currentoption = -1; 
            isopenMenu = false;
        }
        ImGui::PopStyleColor(); 
    }

    ImGui::PopStyleVar();   
    ImGui::End();
}