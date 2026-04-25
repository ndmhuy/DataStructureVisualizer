#include <iostream>
#include <cstdlib>
#include <cstring>

#include "View/InputMenu.h"

namespace {
    int NumberSpaceFilter(ImGuiInputTextCallbackData* data) {
        if (data->EventChar >= '0' && data->EventChar <= '9') return 0;
        if (data->EventChar == ' ') return 0;
        if (data->EventChar =='+'&&data->EventChar=='-') return 0;
        return 1;
    }
    int NumberOnlyFilter(ImGuiInputTextCallbackData* data) {
        if (data->EventChar >= '0' && data->EventChar <= '9') return 0;
        if (data->EventChar =='+'&&data->EventChar=='-') return 0;
        return 1;
    }
    int UnsignedNumberOnlyFilter(ImGuiInputTextCallbackData* data) {
        if (data->EventChar >= '0' && data->EventChar <= '9') return 0;
        return 1;
    }
}

std::vector<std::string> InputMenu::getCurrentMenu() const {
    if (currentDS == 0) return {"Init", "Insert", "Search", "Delete", "Update", "Clear"};
    if (currentDS == 1 || currentDS == 2) return {"Init", "Insert", "ExtractTop", "Peek", "Search", "Delete", "Update", "Clear"};
    if (currentDS == 3) return {"Init", "Insert", "Search", "Delete", "Update", "Clear"};
    if (currentDS == 4) return {"Init", "Random", "Set obstacles", "BFS", "Clear"};
    if (currentDS == 5 || currentDS == 6) return {"Init", "Create Node", "Create Edge", "SSSP", "OPSP", "APSP", "Clear"};
    return {};
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
    float defaultY = theme.inputMenuAnchorYRatio * y - theme.inputMenuAnchorHeightOffsetMultiplier * height;
    
    // Tính toán chiều cao thực tế của menu để không bị tràn màn hình (vertical overflow)
    int currentItemsCount = isopenMenu ? getCurrentMenu().size() : 1;
    float totalMenuHeight = currentItemsCount * height;

    if (defaultY + totalMenuHeight > y) {
        defaultY = y - totalMenuHeight - 10.0f; // Căn lề dưới (padding bottom) một khoảng an toàn
    }

    ImGui::SetNextWindowPos(
        ImVec2(0, defaultY),
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

    // Mở rộng nút Toggle để bằng với chiều cao của menu, tạo thành một khối liền mạch
    if (ImGui::Button(isopenMenu ? "<" : ">", ImVec2(theme.inputMenuToggleWidth, isopenMenu ? totalMenuHeight : height))) {
        isopenMenu = !isopenMenu;
        if (!isopenMenu) {
            currentOption = -1;
        } else {
            inputBuf1[0] = '\0';
            inputBuf2[0] = '\0';
            inputBuf3[0] = '\0';
            inputBuf4[0] = '\0';
        }
    }
    ImGui::PopStyleColor(3);

    // Temporary variables to remember where to draw the floating form
    int activeIndex = -1;
    ImVec2 activeBtnPos;

    if (isopenMenu){
        ImGui::SameLine(); 
        ImGui::BeginGroup();

        std::vector<std::string> currentMenu = getCurrentMenu();
        int menuCount = currentMenu.size();
        for (int i = 0; i < menuCount; ++i){
            bool isActive = (currentOption == i);

            ImGui::PushStyleColor(ImGuiCol_Button, isActive ? colorAccent : colorPrimary);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, colorAccent);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, colorAccent);

            if (ImGui::Button(currentMenu[i].c_str(), ImVec2(length, height))) {
                if (currentOption != i) {
                    currentOption = i;
                    inputBuf1[0] = '\0';
                    inputBuf2[0] = '\0';
                    inputBuf3[0] = '\0';
                    inputBuf4[0] = '\0';
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

    auto DrawInitModes = [&]() {
        if (DrawButton("Array", insertSubMode == 0, 60.0f)) {
            if (insertSubMode != 0) { insertSubMode = 0; inputBuf1[0] = '\0'; }
        }
        ImGui::SameLine();
        if (DrawButton("Random", insertSubMode == 1, 60.0f)) {
            if (insertSubMode != 1) { insertSubMode = 1; inputBuf1[0] = '\0'; }
        }
        ImGui::SameLine();
        if (DrawButton("File", insertSubMode == 2, 60.0f)) {
            if (insertSubMode != 2) { insertSubMode = 2; inputBuf1[0] = '\0'; }
        }
        ImGui::SameLine(0, 15.0f);
        if (insertSubMode == 0) {
            DrawInput("##in_array", inputBuf1, sizeof(inputBuf1), 200.0f, NumberSpaceFilter);
            ImGui::SameLine();
            if (DrawButton("OK", false, 60.0f)) {
                outMode = 0; outString1 = inputBuf1; outString2 = ""; outString3 = ""; outString4 = "";
                hasAction = 1; currentOption = -1; isopenMenu = false;
            }
        } else if (insertSubMode == 1) {
            if (DrawButton("OK", false, 60.0f)) {
                outMode = 1; outString1 = ""; outString2 = ""; outString3 = ""; outString4 = "";
                hasAction = 1; currentOption = -1; isopenMenu = false;
            }
        } else if (insertSubMode == 2) {
            DrawInput("##in_file", inputBuf1, sizeof(inputBuf1), 150.0f, nullptr);
            ImGui::SameLine();
            if (DrawButton("Browse", false, 60.0f)) fileDialog.Open();
            ImGui::SameLine();
            if (DrawButton("OK", false, 60.0f)) {
                outMode = 2; outString1 = inputBuf1; outString2 = ""; outString3 = ""; outString4 = "";
                hasAction = 1; currentOption = -1; isopenMenu = false;
            }
        }
    };

    auto DrawInsertModes = [&](int actionVal) {
        if (DrawButton("Single", insertSubMode == 0, 60.0f)) {
            if (insertSubMode != 0) { insertSubMode = 0; inputBuf1[0] = '\0'; }
        }
        ImGui::SameLine();
        if (DrawButton("Random", insertSubMode == 1, 60.0f)) {
            if (insertSubMode != 1) { insertSubMode = 1; inputBuf1[0] = '\0'; }
        }
        ImGui::SameLine(0, 15.0f);
        if (insertSubMode == 0) {
            DrawInput("##in_single", inputBuf1, sizeof(inputBuf1), 100.0f, NumberOnlyFilter);
            ImGui::SameLine();
            if (DrawButton("OK", false, 60.0f)) {
                outMode = 0; outString1 = inputBuf1; outString2 = ""; outString3 = ""; outString4 = "";
                hasAction = actionVal; currentOption = -1; isopenMenu = false;
            }
        } else if (insertSubMode == 1) {
            if (DrawButton("OK", false, 60.0f)) {
                outMode = 1; outString1 = ""; outString2 = ""; outString3 = ""; outString4 = "";
                hasAction = actionVal; currentOption = -1; isopenMenu = false;
            }
        }
    };

    auto DrawSingleInput = [&](int actionVal) {
        DrawInput("##in_single", inputBuf1, sizeof(inputBuf1), 100.0f, NumberOnlyFilter);
        ImGui::SameLine();
        if (DrawButton("OK", false, 60.0f)) {
            outMode = 0; outString1 = inputBuf1; outString2 = ""; outString3 = ""; outString4 = "";
            hasAction = actionVal; currentOption = -1; isopenMenu = false;
        }
    };

    auto DrawConfirm = [&](int actionVal) {
        if (DrawButton("Confirm", false, 80.0f)) {
            outMode = 0; outString1 = ""; outString2 = ""; outString3 = ""; outString4 = "";
            hasAction = actionVal; currentOption = -1; isopenMenu = false;
        }
    };

    auto DrawUpdateInput = [&](int actionVal) {
        ImGui::SetCursorPosY(textY); ImGui::Text("From"); ImGui::SameLine();
        DrawInput("##in_upd_from", inputBuf1, sizeof(inputBuf1), 80.0f, NumberOnlyFilter);
        ImGui::SameLine(0, theme.inputMenuUpdateLabelSpacing);
        ImGui::SetCursorPosY(textY); ImGui::Text("To"); ImGui::SameLine();
        DrawInput("##in_upd_to", inputBuf2, sizeof(inputBuf2), 80.0f, NumberOnlyFilter);
        ImGui::SameLine();
        if (DrawButton("OK", false, 60.0f)) {
            outMode = 0; outString1 = inputBuf1; outString2 = inputBuf2; outString3 = ""; outString4 = "";
            hasAction = actionVal; currentOption = -1; isopenMenu = false;
        }
    };

    if (currentDS == 0) { // SLL
        if (cur == 0) DrawInitModes();
        else if (cur == 1) DrawInsertModes(2);
        else if (cur == 2) DrawSingleInput(3);
        else if (cur == 3) DrawSingleInput(4);
        else if (cur == 4) DrawUpdateInput(5);
        else if (cur == 5) DrawConfirm(6);
    } else if (currentDS == 1 || currentDS == 2) { // HEAP
        if (cur == 0) DrawInitModes();
        else if (cur == 1) DrawInsertModes(2);
        else if (cur == 2) DrawConfirm(3); // ExtractTop
        else if (cur == 3) DrawConfirm(4); // Peek
        else if (cur == 4) DrawSingleInput(5); // Search
        else if (cur == 5) DrawSingleInput(6); // Delete
        else if (cur == 6) DrawUpdateInput(7); // Update
        else if (cur == 7) DrawConfirm(8); // Clear
    } else if (currentDS == 3) { // AVL
        if (cur == 0) DrawInitModes();
        else if (cur == 1) DrawInsertModes(2);
        else if (cur == 2) DrawSingleInput(3); // Search
        else if (cur == 3) DrawSingleInput(4); // Delete
        else if (cur == 4) DrawUpdateInput(5); // Update
        else if (cur == 5) DrawConfirm(6); // Clear
    } else if (currentDS == 4) { // GRID
        if (cur == 0) { // Init
            if (DrawButton("Empty", insertSubMode == 0, 60.0f)) {
                if (insertSubMode != 0) { insertSubMode = 0; inputBuf1[0] = '\0'; inputBuf2[0] = '\0'; }
            }
            ImGui::SameLine();
            if (DrawButton("File", insertSubMode == 1, 60.0f)) {
                if (insertSubMode != 1) { insertSubMode = 1; inputBuf1[0] = '\0'; inputBuf2[0] = '\0'; }
            }
            ImGui::SameLine(0, 15.0f);
            if (insertSubMode == 0) {
                ImGui::SetCursorPosY(textY); ImGui::Text("N="); ImGui::SameLine();
                DrawInput("##in_grid_n", inputBuf1, sizeof(inputBuf1), 40.0f, UnsignedNumberOnlyFilter);
                ImGui::SameLine(0, theme.inputMenuUpdateLabelSpacing);
                ImGui::SetCursorPosY(textY); ImGui::Text("M="); ImGui::SameLine();
                DrawInput("##in_grid_m", inputBuf2, sizeof(inputBuf2), 40.0f, UnsignedNumberOnlyFilter);
                ImGui::SameLine();
                if (DrawButton("OK", false, 60.0f)) {
                    outMode = 0; outString1 = inputBuf1; outString2 = inputBuf2; outString3 = ""; outString4 = "";
                    hasAction = 1; currentOption = -1; isopenMenu = false;
                }
            } else if (insertSubMode == 1) {
                DrawInput("##in_file", inputBuf1, sizeof(inputBuf1), 150.0f, nullptr);
                ImGui::SameLine();
                if (DrawButton("Browse", false, 60.0f)) fileDialog.Open();
                ImGui::SameLine();
                if (DrawButton("OK", false, 60.0f)) {
                    outMode = 1; outString1 = inputBuf1; outString2 = ""; outString3 = ""; outString4 = "";
                    hasAction = 1; currentOption = -1; isopenMenu = false;
                }
            }
        }
        else if (cur == 1) { // Random
            ImGui::SetCursorPosY(textY); ImGui::Text("N="); ImGui::SameLine();
            DrawInput("##in_rand_n", inputBuf1, sizeof(inputBuf1), 40.0f, UnsignedNumberOnlyFilter);
            ImGui::SameLine(0, theme.inputMenuUpdateLabelSpacing);
            ImGui::SetCursorPosY(textY); ImGui::Text("M="); ImGui::SameLine();
            DrawInput("##in_rand_m", inputBuf2, sizeof(inputBuf2), 40.0f, UnsignedNumberOnlyFilter);
            ImGui::SameLine();
            if (DrawButton("OK", false, 60.0f)) {
                outMode = 0; outString1 = inputBuf1; outString2 = inputBuf2; outString3 = ""; outString4 = "";
                hasAction = 2; currentOption = -1; isopenMenu = false;
            }
        }
        else if (cur == 2) { // Set obstacles
            ImGui::SetCursorPosY(textY); ImGui::Text("i="); ImGui::SameLine();
            DrawInput("##in_obs_i", inputBuf1, sizeof(inputBuf1), 40.0f, UnsignedNumberOnlyFilter);
            ImGui::SameLine(0, theme.inputMenuUpdateLabelSpacing);
            ImGui::SetCursorPosY(textY); ImGui::Text("j="); ImGui::SameLine();
            DrawInput("##in_obs_j", inputBuf2, sizeof(inputBuf2), 40.0f, UnsignedNumberOnlyFilter);
            ImGui::SameLine();
            if (DrawButton("OK", false, 60.0f)) {
                outMode = 0; outString1 = inputBuf1; outString2 = inputBuf2; outString3 = ""; outString4 = "";
                hasAction = 3; currentOption = -1; isopenMenu = false;
            }
        }
        else if (cur == 3) { // BFS
            ImGui::SetCursorPosY(textY); ImGui::Text("from x1="); ImGui::SameLine();
            DrawInput("##in_bfs_x1", inputBuf1, sizeof(inputBuf1), 40.0f, UnsignedNumberOnlyFilter);
            ImGui::SameLine(0, theme.inputMenuUpdateLabelSpacing);
            ImGui::SetCursorPosY(textY); ImGui::Text("y1="); ImGui::SameLine();
            DrawInput("##in_bfs_y1", inputBuf2, sizeof(inputBuf2), 40.0f, UnsignedNumberOnlyFilter);
            ImGui::SameLine(0, theme.inputMenuUpdateLabelSpacing);
            ImGui::SetCursorPosY(textY); ImGui::Text("to x2="); ImGui::SameLine();
            DrawInput("##in_bfs_x2", inputBuf3, sizeof(inputBuf3), 40.0f, UnsignedNumberOnlyFilter);
            ImGui::SameLine(0, theme.inputMenuUpdateLabelSpacing);
            ImGui::SetCursorPosY(textY); ImGui::Text("y2="); ImGui::SameLine();
            DrawInput("##in_bfs_y2", inputBuf4, sizeof(inputBuf4), 40.0f, UnsignedNumberOnlyFilter);
            ImGui::SameLine();
            if (DrawButton("OK", false, 60.0f)) {
                outMode = 0; outString1 = inputBuf1; outString2 = inputBuf2; outString3 = inputBuf3; outString4 = inputBuf4;
                hasAction = 4; currentOption = -1; isopenMenu = false;
            }
        }
        else if (cur == 4) DrawConfirm(5);
    } else if (currentDS == 5 || currentDS == 6) { // SHORTEST PATH ALGORITHM
        if (cur == 0) { // Init from file
            if (DrawButton("Adj Matrix", insertSubMode == 0, 100.0f)) {
                if (insertSubMode != 0) { insertSubMode = 0; inputBuf1[0] = '\0'; }
            }
            ImGui::SameLine();
            if (DrawButton("Adj List", insertSubMode == 1, 100.0f)) {
                if (insertSubMode != 1) { insertSubMode = 1; inputBuf1[0] = '\0'; }
            }
            ImGui::SameLine(0, 15.0f);
            DrawInput("##in_file", inputBuf1, sizeof(inputBuf1), 150.0f, nullptr);
            ImGui::SameLine();
            if (DrawButton("Browse", false, 60.0f)) fileDialog.Open();
            ImGui::SameLine();
            if (DrawButton("OK", false, 60.0f)) {
                outMode = insertSubMode; outString1 = inputBuf1; outString2 = ""; outString3 = ""; outString4 = "";
                hasAction = 1; currentOption = -1; isopenMenu = false;
            }
        }
        else if (cur == 1) DrawSingleInput(2);
        else if (cur == 2) { // Create Edge
            ImGui::SetCursorPosY(textY); ImGui::Text("u="); ImGui::SameLine();
            DrawInput("##in_edge_u", inputBuf1, sizeof(inputBuf1), 50.0f, NumberOnlyFilter);
            ImGui::SameLine(0, theme.inputMenuUpdateLabelSpacing);
            ImGui::SetCursorPosY(textY); ImGui::Text("v="); ImGui::SameLine();
            DrawInput("##in_edge_v", inputBuf2, sizeof(inputBuf2), 50.0f, NumberOnlyFilter);
            ImGui::SameLine(0, theme.inputMenuUpdateLabelSpacing);
            ImGui::SetCursorPosY(textY); ImGui::Text("(u,v)="); ImGui::SameLine();
            DrawInput("##in_edge_w", inputBuf3, sizeof(inputBuf3), 50.0f, NumberOnlyFilter);
            ImGui::SameLine();
            if (DrawButton("OK", false, 60.0f)) {
                outMode = 0; outString1 = inputBuf1; outString2 = inputBuf2; outString3 = inputBuf3; outString4 = "";
                hasAction = 3; currentOption = -1; isopenMenu = false;
            }
        }
        else if (cur == 3) DrawSingleInput(4);
        else if (cur == 4) { // OPSP
            if (DrawButton("DAG", insertSubMode == 0, 60.0f)) {
                if (insertSubMode != 0) { insertSubMode = 0; inputBuf1[0] = '\0'; inputBuf2[0] = '\0'; }
            }
            ImGui::SameLine();
            if (DrawButton("Dijkstra", insertSubMode == 1, 80.0f)) {
                if (insertSubMode != 1) { insertSubMode = 1; inputBuf1[0] = '\0'; inputBuf2[0] = '\0'; }
            }
            ImGui::SameLine();
            if (DrawButton("Bellman", insertSubMode == 2, 80.0f)) {
                if (insertSubMode != 2) { insertSubMode = 2; inputBuf1[0] = '\0'; inputBuf2[0] = '\0'; }
            }
            ImGui::SameLine(0, 15.0f);
            ImGui::SetCursorPosY(textY); ImGui::Text("u="); ImGui::SameLine();
            DrawInput("##in_opsp_u", inputBuf1, sizeof(inputBuf1), 40.0f, NumberOnlyFilter);
            ImGui::SameLine(0, theme.inputMenuUpdateLabelSpacing);
            ImGui::SetCursorPosY(textY); ImGui::Text("v="); ImGui::SameLine();
            DrawInput("##in_opsp_v", inputBuf2, sizeof(inputBuf2), 40.0f, NumberOnlyFilter);
            ImGui::SameLine();
            if (DrawButton("OK", false, 60.0f)) {
                outMode = insertSubMode; outString1 = inputBuf1; outString2 = inputBuf2; outString3 = ""; outString4 = "";
                hasAction = 5; currentOption = -1; isopenMenu = false;
            }
        }
        else if (cur == 5) DrawConfirm(6); // APSP
        else if (cur == 6) DrawConfirm(7); // Clear
    }

    ImGui::PopStyleVar();   
    ImGui::End();
}
