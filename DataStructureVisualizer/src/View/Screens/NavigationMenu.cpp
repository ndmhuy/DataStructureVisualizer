#include "View/Screens/NavigationMenu.h"

void NavigationMenu::init(const Theme& theme) {
    this->theme = theme;
    selectedDS = -1;
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

    // --- Vẽ Tiêu đề (Nằm ở top-center) ---
    const char* title = "DATA STRUCTURE VISUALIZER";
    ImVec2 titleSize = ImGui::CalcTextSize(title);
    ImGui::SetCursorPos(ImVec2((winSize.x - titleSize.x) * 0.5f, winSize.y * 0.25f));
    ImGui::Text("%s", title);

    if (hasTitleFont) {
        ImGui::PopFont();
    }

    // --- Vẽ Lưới Nút Bấm 2x2 ---
    float btnWidth = 400.0f; // Mở rộng kích thước nút để chứa trọn vẹn cụm từ "Shortest Path Algorithm"
    float btnHeight = 80.0f;
    float spacing = 30.0f;
    int cols = 2; 
    
    float startX = (winSize.x - (cols * btnWidth + (cols - 1) * spacing)) * 0.5f;
    float startY = winSize.y * 0.45f;

    for (size_t i = 0; i < dsNames.size(); ++i) {
        int row = i / cols;
        int col = i % cols;
        
        ImGui::SetCursorPos(ImVec2(startX + col * (btnWidth + spacing), startY + row * (btnHeight + spacing)));
        if (ImGui::Button(dsNames[i].c_str(), ImVec2(btnWidth, btnHeight))) {
            selectedDS = static_cast<int>(i);
        }
    }

    ImGui::PopStyleColor(4);
    ImGui::PopStyleVar();
    ImGui::End();
}