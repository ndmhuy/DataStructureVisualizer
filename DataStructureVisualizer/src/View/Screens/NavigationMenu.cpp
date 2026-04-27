#define IMGUI_DEFINE_MATH_OPERATORS
#include "View/Screens/NavigationMenu.h"
#include "imgui_internal.h"
#include "View/UI/UIanimation.h"
#include <unordered_map>
#include <cmath>

void NavigationMenu::init(const Theme& theme) {
    this->theme = theme;
    selectedDS = -1;
    currentState = MenuState::Main;

    if (clickBuffer.loadFromFile(theme.clickSoundPath)) {
        clickSound.setBuffer(clickBuffer);
        clickSound.setVolume(60.0f);
    }
}

void NavigationMenu::render(const sf::RenderWindow& window, const sf::Vector2u& actualWindowSize) {
    // Sử dụng kích thước cửa sổ SFML thực tế để đảm bảo đồng bộ
    ImVec2 winSize = ImVec2(static_cast<float>(actualWindowSize.x), static_cast<float>(actualWindowSize.y));
    
    // Thiết lập cửa sổ lấp đầy màn hình và ẩn thanh tiêu đề
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(winSize);
    
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | 
                             ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | 
                             ImGuiWindowFlags_NoBackground | 
                             ImGuiWindowFlags_NoBringToFrontOnFocus; // Tránh che khuất các nút Global

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f)); // Loại bỏ đệm mặc định để đồng bộ toạ độ 1:1 với SFML
    ImGui::Begin("NavigationMenuScreen", nullptr, flags);
    ImGui::PopStyleVar();

    // Áp dụng phong cách Flat Design và dùng màu của InputMenu cho tính đồng bộ
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
    
    ImVec4 btnColor = ImVec4(theme.inputMenuPrimaryColor.r/255.f, theme.inputMenuPrimaryColor.g/255.f, theme.inputMenuPrimaryColor.b/255.f, 1.0f);
    ImVec4 btnHover = ImVec4(theme.inputMenuAccentColor.r/255.f, theme.inputMenuAccentColor.g/255.f, theme.inputMenuAccentColor.b/255.f, 1.0f);
    ImVec4 btnActive = ImVec4(theme.inputMenuAccentColor.r/255.f, theme.inputMenuAccentColor.g/255.f, theme.inputMenuAccentColor.b/255.f, 1.0f);

    // --- ANIMATED CYBERPUNK BACKGROUND ---
    UIanimation::DrawCyberpunkBackground(winSize, (float)ImGui::GetTime(), ImGui::GetMousePos(), btnColor, btnHover);

    // --- add back button ---
    if (currentState != MenuState::Main) {
        ImGui::SetCursorScreenPos(ImVec2(10.0f, 10.0f));
        if (ImGui::InvisibleButton("BackBtn", ImVec2(80.0f, 35.0f))) {
            clickSound.play();
            if (currentState == MenuState::Heap) currentState = MenuState::Main;
            else if (currentState == MenuState::ShortestPath) currentState = MenuState::Main;
            else if (currentState == MenuState::GraphType) currentState = MenuState::ShortestPath;
            else if (currentState == MenuState::GraphRepr) currentState = MenuState::GraphType;
        }
    }


    // --- Vẽ Lưới Nút Bấm ---
    float btnWidth = 350.0f;
    float btnHeight = 80.0f;
    float spacingX = 40.0f; 
    float spacingY = 120.0f; // Tăng khoảng cách dọc để hình minh hoạ không bị nút che
    int cols = 2; 
    
    std::vector<std::string>* currentNames = &mainNames;
    if (currentState == MenuState::Heap) currentNames = &heapNames;
    else if (currentState == MenuState::ShortestPath) currentNames = &spaNames;
    else if (currentState == MenuState::GraphType) currentNames = &graphTypeNames;
    else if (currentState == MenuState::GraphRepr) currentNames = &graphReprNames;

    int totalItems = currentNames->size();
    int rows = (totalItems + cols - 1) / cols;
    
    float startY = winSize.y * 0.48f; // Cân đối lại không gian

    for (int i = 0; i < totalItems; ++i) {
        int row = i / cols;
        int col = i % cols;
        
        // Tính toán để luôn tự động căn giữa các phần tử chưa lấp đầy dòng
        int itemsInThisRow = (row == rows - 1 && totalItems % cols != 0) ? (totalItems % cols) : cols;
        float startX = (winSize.x - (itemsInThisRow * btnWidth + (itemsInThisRow - 1) * spacingX)) * 0.5f;
        
        // Hiệu ứng trồi lên (Slide-up entrance) lần lượt từng dòng
        static std::unordered_map<int, float> entranceAnim;
        float dt = ImGui::GetIO().DeltaTime;
        entranceAnim[i] = ImLerp(entranceAnim[i], 1.0f, dt * (10.0f - row * 2.0f));
        
        float yOffset = (1.0f - entranceAnim[i]) * 100.0f;
        float alpha = entranceAnim[i];

        ImGui::SetCursorScreenPos(ImVec2(startX + col * (btnWidth + spacingX), startY + row * (btnHeight + spacingY)));
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
        
        if (ImGui::InvisibleButton((*currentNames)[i].c_str(), ImVec2(btnWidth, btnHeight))) {
            clickSound.play();
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
                else if (i == 1) currentState = MenuState::GraphType;
            } else if (currentState == MenuState::GraphType) {
                if (i == 0) { isDirectedGraph = true; currentState = MenuState::GraphRepr; }
                else if (i == 1) { isDirectedGraph = false; currentState = MenuState::GraphRepr; }
            } else if (currentState == MenuState::GraphRepr) {
                if (isDirectedGraph) selectedDS = 5; // Directed Graph
                else selectedDS = 6; // Undirected Graph
            }
            entranceAnim.clear(); // Reset anim cho màn sau
        }
        ImGui::PopStyleVar();
    }

    ImGui::PopStyleVar();
    ImGui::End();
}