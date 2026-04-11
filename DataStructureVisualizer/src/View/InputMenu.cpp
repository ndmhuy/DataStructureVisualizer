#include <iostream>
#include <cstdlib>
#include <cstring>

#include "View/InputMenu.h"

namespace {
    int NumberSpaceFilter(ImGuiInputTextCallbackData* data) {
        if (data->EventChar >= '0' && data->EventChar <= '9') return 0;
        if (data->EventChar == ' ') return 0;
        return 1;
    }
    int NumberOnlyFilter(ImGuiInputTextCallbackData* data) {
        if (data->EventChar >= '0' && data->EventChar <= '9') return 0;
        return 1;
    }
}

bool InputMenu::init(const Theme& theme){
    this->theme = theme;
    fileDialog.SetTitle("Select File");
    fileDialog.SetTypeFilters({ ".txt", ".*" }); // Giới hạn các file mở định dạng TXT (có thể thay đổi tuỳ thích)
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
        if (!isopenMenu) {
            currentOption = -1;
        } else {
            inputBuf1[0] = '\0';
            inputBuf2[0] = '\0';
        }
    }
    ImGui::PopStyleColor(3);

    // Temporary variables to remember where to draw the floating form
    int activeIndex = -1;
    ImVec2 activeBtnPos;

    if (isopenMenu){
        ImGui::SameLine(); 
        ImGui::BeginGroup();

        for (int i = 0; i < 5; ++i){
            bool isActive = (currentOption == i);

            ImGui::PushStyleColor(ImGuiCol_Button, isActive ? colorAccent : colorPrimary);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, colorAccent);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, colorAccent);

            if (ImGui::Button(menu[i], ImVec2(length, height))) {
                if (currentOption != i) {
                    currentOption = i;
                    inputBuf1[0] = '\0';
                    inputBuf2[0] = '\0';
                }
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

    // Gọi lệnh hiển thị file dialog bên ngoài context các popup đang chạy của giao diện
    fileDialog.Display();

    if (fileDialog.HasSelected())
    {
        std::string path = fileDialog.GetSelected().string();
        std::strncpy(inputBuf1, path.c_str(), sizeof(inputBuf1) - 1);
        inputBuf1[sizeof(inputBuf1) - 1] = '\0';
        fileDialog.ClearSelected();
    }
}

void InputMenu::renderinputform(const sf::RenderWindow& window, int cur, ImVec2 btnPos){
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
    float height = theme.inputMenuHeight;
    float length = theme.inputMenuButtonWidth;

    // Snap the form to the right edge of the active menu button
    ImGui::SetNextWindowPos(ImVec2(btnPos.x + length, btnPos.y)); 
    ImGui::SetNextWindowSizeConstraints(ImVec2(0, height), ImVec2(9999.0f, height));
    
    ImGuiWindowFlags formFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | 
                                 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
                                 ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_AlwaysAutoResize;
                                         
    ImGui::Begin("SelectFormWindow", nullptr, formFlags);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(theme.inputMenuFormSpacing, 0.0f));
    
    float textY    = (height - ImGui::GetTextLineHeight()) / 2.0f;
    float inputY   = (height - ImGui::GetFrameHeight()) / 2.0f;
    float okBtnH   = height * theme.inputMenuOkButtonHeightRatio;
    float okBtnY   = (height - okBtnH) / 2.0f;

    auto DrawButton = [&](const char* label, bool active, float w) -> bool {
        ImGui::SetCursorPosY(okBtnY);
        ImGui::PushStyleColor(ImGuiCol_Button, active ? colorAccent : colorPrimary);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, colorAccent);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, colorAccent);
        bool clicked = ImGui::Button(label, ImVec2(w, okBtnH));
        ImGui::PopStyleColor(3);
        return clicked;
    };

    auto DrawInput = [&](const char* id, char* buf, size_t buf_size, float w, ImGuiInputTextCallback filter) {
        ImGui::SetCursorPosY(inputY);
        ImGui::PushStyleColor(ImGuiCol_FrameBg, colorAccent);
        ImGui::SetNextItemWidth(w);
        if (filter != nullptr) {
            ImGui::InputText(id, buf, buf_size, ImGuiInputTextFlags_CallbackCharFilter, filter);
        } else {
            ImGui::InputText(id, buf, buf_size); // Bỏ flag callback nếu không truyền filter cho field Text
        }
        ImGui::PopStyleColor();
    };

    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + theme.inputMenuLabelXOffset);

    if (cur == 0) { // Insert
        if (DrawButton("Single", insertSubMode == 0, 60.0f)) {
            if (insertSubMode != 0) {
                insertSubMode = 0;
                inputBuf1[0] = '\0';
            }
        }
        ImGui::SameLine();
        if (DrawButton("Array", insertSubMode == 1, 60.0f)) {
            if (insertSubMode != 1) {
                insertSubMode = 1;
                inputBuf1[0] = '\0';
            }
        }
        ImGui::SameLine();
        if (DrawButton("File", insertSubMode == 2, 60.0f)) {
            if (insertSubMode != 2) {
                insertSubMode = 2;
                inputBuf1[0] = '\0';
            }
        }
        
        ImGui::SameLine(0, 15.0f);
        if (insertSubMode == 0) {
            DrawInput("##in_single", inputBuf1, sizeof(inputBuf1), 100.0f, NumberOnlyFilter);
            ImGui::SameLine();
            if (DrawButton("Add", false, 60.0f)) {
                outMode = 0; outString1 = inputBuf1; outString2 = "";
                hasAction = 1; currentOption = -1; isopenMenu = false;
            }
        } else if (insertSubMode == 1) {
            DrawInput("##in_array", inputBuf1, sizeof(inputBuf1), 200.0f, NumberSpaceFilter);
            ImGui::SameLine();
            if (DrawButton("Load", false, 60.0f)) {
                outMode = 1; outString1 = inputBuf1; outString2 = "";
                hasAction = 1; currentOption = -1; isopenMenu = false;
            }
        } else if (insertSubMode == 2) {
            DrawInput("##in_file", inputBuf1, sizeof(inputBuf1), 150.0f, nullptr);
            ImGui::SameLine();
            if (DrawButton("Browse", false, 60.0f)) {
                fileDialog.Open(); // Kích hoạt pop-up Browse
            }
            ImGui::SameLine();
            if (DrawButton("Load", false, 60.0f)) {
                outMode = 2; outString1 = inputBuf1; outString2 = "";
                hasAction = 1; currentOption = -1; isopenMenu = false;
            }
        }
    } else if (cur == 1) { // Random
        if (DrawButton("Single", false, 80.0f)) {
            outMode = 0; outString1 = ""; outString2 = "";
            hasAction = 5; currentOption = -1; isopenMenu = false;
        }
        ImGui::SameLine();
        if (DrawButton("Array", false, 80.0f)) {
            outMode = 1; outString1 = ""; outString2 = "";
            hasAction = 5; currentOption = -1; isopenMenu = false;
        }
    } else if (cur == 2 || cur == 3) { // Delete, Search
        DrawInput("##in_del_search", inputBuf1, sizeof(inputBuf1), 100.0f, NumberOnlyFilter);
        ImGui::SameLine();
        if (DrawButton("OK", false, 60.0f)) {
            outMode = 0; outString1 = inputBuf1; outString2 = "";
            hasAction = cur; currentOption = -1; isopenMenu = false;
        }
    } else if (cur == 4) { // Update
        ImGui::SetCursorPosY(textY); ImGui::Text("From"); ImGui::SameLine();
        DrawInput("##in_upd_from", inputBuf1, sizeof(inputBuf1), 80.0f, NumberOnlyFilter);
        ImGui::SameLine(0, theme.inputMenuUpdateLabelSpacing);
        ImGui::SetCursorPosY(textY); ImGui::Text("To"); ImGui::SameLine();
        DrawInput("##in_upd_to", inputBuf2, sizeof(inputBuf2), 80.0f, NumberOnlyFilter);
        ImGui::SameLine();
        if (DrawButton("OK", false, 60.0f)) {
            outMode = 0; outString1 = inputBuf1; outString2 = inputBuf2;
            hasAction = 4; currentOption = -1; isopenMenu = false;
        }
    }

    ImGui::PopStyleVar();   
    ImGui::End();
}
