#include <iostream>
#include <cstdlib>

#include "View/InputMenu.h"

bool InputMenu::init(const Theme& theme){
    this->theme = theme;

    // Load randombox file
    if (!texture.loadFromFile(theme.randomboxPath, true)){
        std::cerr << "Warning: InputMenu::init failed to load randombox texture from '"
                  << theme.randomboxPath << "'." << std::endl;
        return false;
    }
    texture.setSmooth(true);
    return true;
}

void InputMenu::render(const sf::RenderWindow& window){
    ImVec4 colorPrimary = ImVec4(
        static_cast<float>(theme.inputMenuPrimaryColor.r) / 255.0f,
        static_cast<float>(theme.inputMenuPrimaryColor.g) / 255.0f,
        static_cast<float>(theme.inputMenuPrimaryColor.b) / 255.0f,
        static_cast<float>(theme.inputMenuPrimaryColor.a) / 255.0f
    );
    ImVec4 colorAccent = ImVec4(
        static_cast<float>(theme.inputMenuAccentColor.r) / 255.0f,
        static_cast<float>(theme.inputMenuAccentColor.g) / 255.0f,
        static_cast<float>(theme.inputMenuAccentColor.b) / 255.0f,
        static_cast<float>(theme.inputMenuAccentColor.a) / 255.0f
    );
    ImVec4 colorText = ImVec4(
        static_cast<float>(theme.inputMenuTextColor.r) / 255.0f,
        static_cast<float>(theme.inputMenuTextColor.g) / 255.0f,
        static_cast<float>(theme.inputMenuTextColor.b) / 255.0f,
        static_cast<float>(theme.inputMenuTextColor.a) / 255.0f
    );
    float height = theme.inputMenuHeight;
    float length = theme.inputMenuButtonWidth;

    // Flattening UI for all upcoming windows
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f)); 
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    ImGui::PushStyleColor(ImGuiCol_WindowBg, colorPrimary);
    ImGui::PushStyleColor(ImGuiCol_Text, colorText);

    // Set position for menu
    int y = window.getSize().y;
    ImGui::SetNextWindowPos(
        ImVec2(0, theme.inputMenuAnchorYRatio * y - theme.inputMenuAnchorHeightOffsetMultiplier * height),
        ImGuiCond_Always
    );

    ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | 
                             ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | 
                             ImGuiWindowFlags_NoBringToFrontOnFocus;
    
    // Begin Main Menu Window
    ImGui::Begin("FlatMenu", nullptr, flags);

    // Open menu button
    ImGui::PushStyleColor(ImGuiCol_Button, colorPrimary);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, colorAccent);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, colorAccent);

    if (ImGui::Button(isopenMenu ? "<" : ">", ImVec2(theme.inputMenuToggleWidth, height))) {
        isopenMenu = !isopenMenu;
        if (!isopenMenu) currentOption = -1;
    }
    ImGui::PopStyleColor(3);

    // Temporary variables to remember where to draw the floating form
    int activeIndex = -1;
    ImVec2 activeBtnPos;

    if (isopenMenu){
        ImGui::SameLine(); 
        ImGui::BeginGroup();

        for (int i = 0; i < 4; ++i){
            bool isActive = (currentOption == i);

            ImGui::PushStyleColor(ImGuiCol_Button, isActive ? colorAccent : colorPrimary);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, colorAccent);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, colorAccent);

            if (ImGui::Button(menu[i], ImVec2(length, height))){
                currentOption = i;
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
    ImVec4 colorAccent = ImVec4(
        static_cast<float>(theme.inputMenuAccentColor.r) / 255.0f,
        static_cast<float>(theme.inputMenuAccentColor.g) / 255.0f,
        static_cast<float>(theme.inputMenuAccentColor.b) / 255.0f,
        static_cast<float>(theme.inputMenuAccentColor.a) / 255.0f
    );
    float height = theme.inputMenuHeight;
    float length = theme.inputMenuButtonWidth;

    // 1. Calculate the form width dynamically to ensure enough space
    float formWidth = (cur != 3)
        ? (length * theme.inputMenuFormWidthMultiplierDefault)
        : (length * theme.inputMenuFormWidthMultiplierUpdate);
    
    // 2. Snap the form to the right edge of the active menu button
    ImGui::SetNextWindowPos(ImVec2(btnPos.x + length, btnPos.y)); 
    ImGui::SetNextWindowSize(ImVec2(formWidth, height));
    
    ImGuiWindowFlags formFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | 
                                 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
                                 ImGuiWindowFlags_NoBringToFrontOnFocus;
                                         
    ImGui::Begin("SelectFormWindow", nullptr, formFlags);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(theme.inputMenuFormSpacing, 0.0f));
    
    //Centre
    float textY    = (height - ImGui::GetTextLineHeight()) / 2.0f;
    float inputY   = (height - ImGui::GetFrameHeight()) / 2.0f;
    float iconSize = height * theme.inputMenuIconScale;
    float iconY    = (height - iconSize) / 2.0f;
    float okBtnH   = height * theme.inputMenuOkButtonHeightRatio;
    float okBtnY   = (height - okBtnH) / 2.0f;

    if (cur != 3){ // Form for Insert, Delete, Search
        // 1. K text
        ImGui::SetCursorPos(ImVec2(theme.inputMenuLabelXOffset, textY)); 
        ImGui::Text("K=");
                
        // 2. Input box
        ImGui::SameLine();   
        ImGui::SetCursorPosY(inputY); 
        ImGui::PushStyleColor(ImGuiCol_FrameBg, colorAccent);       
        ImGui::SetNextItemWidth(theme.inputMenuInputWidth);
        ImGui::InputInt("##k_input", &k, 0, 0);             
        ImGui::PopStyleColor();

        // 3. Random box
        ImGui::SameLine();
        ImGui::SetCursorPosY(iconY); 
        ImGui::PushStyleColor(ImGuiCol_Button, colorAccent);
        if (ImGui::ImageButton("btn_random", texture, sf::Vector2f(iconSize, iconSize))) {
            k = std::rand() % 100;
        }
                
        // 4. OK Button
        ImGui::SameLine();
        ImGui::SetCursorPosY(okBtnY);
        if (ImGui::Button("OK", ImVec2(theme.inputMenuInputWidth, okBtnH))) {
            hasAction = cur + 1; 
            currentOption = -1;
            isopenMenu=false;  
        }
        ImGui::PopStyleColor(); // Pop for both Random and OK Button
    }
    else { // Form for Update (From and To)
        // 1. From text
        ImGui::SetCursorPos(ImVec2(theme.inputMenuLabelXOffset, textY)); 
        ImGui::Text("From");
                
        // 2. Input box
        ImGui::SameLine();   
        ImGui::SetCursorPosY(inputY); 
        ImGui::PushStyleColor(ImGuiCol_FrameBg, colorAccent);
        ImGui::SetNextItemWidth(theme.inputMenuInputWidth);
        ImGui::InputInt("##k_input", &k, 0, 0);             
        ImGui::PopStyleColor();

        // 3. Random box
        ImGui::SameLine();
        ImGui::SetCursorPosY(iconY); 
        ImGui::PushStyleColor(ImGuiCol_Button, colorAccent);
        if (ImGui::ImageButton("btn_random_1", texture, sf::Vector2f(iconSize, iconSize))) {
            k = std::rand() % 100;
        }
        ImGui::PopStyleColor();

        // 4. To text
        ImGui::SameLine(0, theme.inputMenuUpdateLabelSpacing);
        ImGui::SetCursorPosY(textY); 
        ImGui::Text("To");
                
        // 5. Input box
        ImGui::SameLine();   
        ImGui::SetCursorPosY(inputY); 
        ImGui::PushStyleColor(ImGuiCol_FrameBg, colorAccent);
        ImGui::SetNextItemWidth(theme.inputMenuInputWidth);
        ImGui::InputInt("##r_input", &r, 0, 0);             
        ImGui::PopStyleColor();

        // 6. Random box
        ImGui::SameLine();
        ImGui::SetCursorPosY(iconY); 
        ImGui::PushStyleColor(ImGuiCol_Button, colorAccent);
        if (ImGui::ImageButton("btn_random_2", texture, sf::Vector2f(iconSize, iconSize))) {
            r = std::rand() % 100; 
        }
                
        // 7. OK Button
        ImGui::SameLine();
        ImGui::SetCursorPosY(okBtnY);
        if (ImGui::Button("OK", ImVec2(theme.inputMenuInputWidth, okBtnH))) {
            hasAction = cur + 1; 
            currentOption = -1; 
            isopenMenu = false;
        }
        ImGui::PopStyleColor(); 
    }

    ImGui::PopStyleVar();   
    ImGui::End();
}
