#include "View/NavigationMenu.h"

void NavigationMenu::init(const Theme& theme) {
    this->theme = theme;
    selectedDS = -1;
    currentState = MenuState::Main;
}

void NavigationMenu::render(const sf::RenderWindow& window) {
    ImVec2 winSize = ImGui::GetIO().DisplaySize;
    
    // Thiết lập cửa sổ lấp đầy màn hình và ẩn thanh tiêu đề
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(winSize);
    
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | 
                             ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | 
                             ImGuiWindowFlags_NoBackground | 
                             ImGuiWindowFlags_NoBringToFrontOnFocus; // Tránh che khuất các nút Global

    ImGui::Begin("NavigationMenuScreen", nullptr, flags);

    // Áp dụng phong cách Flat Design và dùng màu của InputMenu cho tính đồng bộ
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(theme.inputMenuPrimaryColor.r/255.f, theme.inputMenuPrimaryColor.g/255.f, theme.inputMenuPrimaryColor.b/255.f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(theme.inputMenuAccentColor.r/255.f, theme.inputMenuAccentColor.g/255.f, theme.inputMenuAccentColor.b/255.f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(theme.inputMenuAccentColor.r/255.f, theme.inputMenuAccentColor.g/255.f, theme.inputMenuAccentColor.b/255.f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(theme.inputMenuTextColor.r/255.f, theme.inputMenuTextColor.g/255.f, theme.inputMenuTextColor.b/255.f, 1.0f));

    bool hasTitleFont = ImGui::GetIO().Fonts->Fonts.Size > 1;
    if (hasTitleFont) {
        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]); // Dùng font số 2 (size 36)
    }

    // --- add back button ---
    if (currentState != MenuState::Main) {
        ImGui::SetCursorPos(ImVec2(10.0f, 10.0f));
        if (ImGui::Button("Back", ImVec2(80.0f, 35.0f))) {
            if (currentState == MenuState::Heap) currentState = MenuState::Main;
            else if (currentState == MenuState::ShortestPath) currentState = MenuState::Main;
            else if (currentState == MenuState::Graph) currentState = MenuState::ShortestPath;
        }
    }

    // --- Vẽ Tiêu đề (Nằm ở top-center) ---
    std::string titleStr = "DATA STRUCTURE VISUALIZER";
    if (currentState == MenuState::Heap) titleStr = "SELECT HEAP TYPE";
    else if (currentState == MenuState::ShortestPath) titleStr = "SELECT ENVIRONMENT";
    else if (currentState == MenuState::Graph) titleStr = "SELECT GRAPH STRUCTURE";

    const char* title = titleStr.c_str();
    ImVec2 titleSize = ImGui::CalcTextSize(title);
    ImGui::SetCursorPos(ImVec2((winSize.x - titleSize.x) * 0.5f, winSize.y * 0.25f));
    ImGui::Text("%s", title);

    if (hasTitleFont) {
        ImGui::PopFont();
    }

    // --- Vẽ Lưới Nút Bấm ---
    float btnWidth = 350.0f;
    float btnHeight = 80.0f;
    float spacing = 30.0f;
    int cols = 2; 
    
    std::vector<std::string>* currentNames = &mainNames;
    if (currentState == MenuState::Heap) currentNames = &heapNames;
    else if (currentState == MenuState::ShortestPath) currentNames = &spaNames;
    else if (currentState == MenuState::Graph) currentNames = &graphNames;

    int totalItems = currentNames->size();
    int rows = (totalItems + cols - 1) / cols;
    
    float startY = winSize.y * 0.45f;

    for (int i = 0; i < totalItems; ++i) {
        int row = i / cols;
        int col = i % cols;
        
        // Tính toán để luôn tự động căn giữa các phần tử chưa lấp đầy dòng
        int itemsInThisRow = (row == rows - 1 && totalItems % cols != 0) ? (totalItems % cols) : cols;
        float startX = (winSize.x - (itemsInThisRow * btnWidth + (itemsInThisRow - 1) * spacing)) * 0.5f;
        
        ImGui::SetCursorPos(ImVec2(startX + col * (btnWidth + spacing), startY + row * (btnHeight + spacing)));
        if (ImGui::Button((*currentNames)[i].c_str(), ImVec2(btnWidth, btnHeight))) {
            if (currentState == MenuState::Main) {
                if (i == 0) selectedDS = 0; // SLL
                else if (i == 1) currentState = MenuState::Heap;
                else if (i == 2) selectedDS = 3; // AVL
                else if (i == 3) currentState = MenuState::ShortestPath;
            } else if (currentState == MenuState::Heap) {
                if (i == 0) selectedDS = 1; // Min Heap
                else if (i == 1) selectedDS = 2; // Max Heap
            } else if (currentState == MenuState::ShortestPath) {
                if (i == 0) selectedDS = 4; // Grid
                else if (i == 1) currentState = MenuState::Graph;
            } else if (currentState == MenuState::Graph) {
                if (i == 0) selectedDS = 5; // Adjacency Matrix
                else if (i == 1) selectedDS = 6; // Adjacency List
            }
        }
    }

    ImGui::PopStyleColor(4);
    ImGui::PopStyleVar();
    ImGui::End();
}