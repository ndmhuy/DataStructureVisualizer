#define IMGUI_DEFINE_MATH_OPERATORS
#include "View/Screens/NavigationMenu.h"
#include "imgui_internal.h"
#include <unordered_map>
#include <cmath>

namespace {
    ImVec4 LerpColor(const ImVec4& a, const ImVec4& b, float t) {
        return ImVec4(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t, a.z + (b.z - a.z) * t, a.w + (b.w - a.w) * t);
    }

    bool AnimatedMenuButton(const char* label, const ImVec2& size_arg, ImVec4 colorNormal, ImVec4 colorHover, ImVec4 colorActive, sf::Sound* sound = nullptr) {
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        if (window->SkipItems) return false;

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;
        const ImGuiID id = window->GetID(label);
        const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

        ImVec2 pos = window->DC.CursorPos;
        ImVec2 size = ImGui::CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

        const ImRect bb(pos, pos + size);
        ImGui::ItemSize(size, style.FramePadding.y);
        if (!ImGui::ItemAdd(bb, id)) return false;

        bool hovered, held;
        bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held);

        static std::unordered_map<ImGuiID, float> hoverAnim;
        static std::unordered_map<ImGuiID, float> clickAnim;
        
        float dt = g.IO.DeltaTime;
        hoverAnim[id] = ImClamp(hoverAnim[id] + (hovered ? dt * 8.0f : -dt * 5.0f), 0.0f, 1.0f);
        clickAnim[id] = ImClamp(clickAnim[id] + (held ? dt * 15.0f : -dt * 10.0f), 0.0f, 1.0f);

        float hEase = 1.0f - (1.0f - hoverAnim[id]) * (1.0f - hoverAnim[id]); // Ease out quad
        float cEase = clickAnim[id];

        ImVec4 current_color = LerpColor(colorNormal, colorHover, hEase);
        if (held) current_color = colorActive;

        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        
        // Hiệu ứng co rút khi click
        float scale = 1.0f - (cEase * 0.05f); 
        ImVec2 center = bb.GetCenter();
        ImVec2 half_size = ImVec2(size.x * scale * 0.5f, size.y * scale * 0.5f);
        ImRect render_bb(center - half_size, center + half_size);

        // Hiệu ứng đổ bóng phát sáng
        if (hEase > 0.0f) {
            float glow_size = 12.0f * hEase;
            draw_list->AddRectFilled(render_bb.Min - ImVec2(glow_size, glow_size), 
                                     render_bb.Max + ImVec2(glow_size, glow_size), 
                                     ImGui::GetColorU32(ImVec4(colorHover.x, colorHover.y, colorHover.z, hEase * 0.4f)), 
                                     style.FrameRounding + glow_size);
        }

        draw_list->AddRectFilled(render_bb.Min, render_bb.Max, ImGui::GetColorU32(current_color), style.FrameRounding);
        draw_list->AddRect(render_bb.Min, render_bb.Max, ImGui::GetColorU32(ImVec4(1,1,1, hEase * 0.6f)), style.FrameRounding, 0, 2.0f);

        // Shift text nhẹ lên trên để tạo pop effect
        ImVec2 text_pos = center - ImVec2(label_size.x * 0.5f, label_size.y * 0.5f);
        text_pos.y -= hEase * 3.0f;
        
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 1, 1));
        draw_list->AddText(text_pos, ImGui::GetColorU32(ImGuiCol_Text), label);
        ImGui::PopStyleColor();

        if (pressed && sound) {
            sound->play();
        }

        return pressed;
    }
}

void NavigationMenu::init(const Theme& theme) {
    this->theme = theme;
    selectedDS = -1;
    currentState = MenuState::Main;

    if (clickBuffer.loadFromFile(theme.clickSoundPath)) {
        clickSound.setBuffer(clickBuffer);
        clickSound.setVolume(60.0f);
    }
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
    
    ImVec4 btnColor = ImVec4(theme.inputMenuPrimaryColor.r/255.f, theme.inputMenuPrimaryColor.g/255.f, theme.inputMenuPrimaryColor.b/255.f, 1.0f);
    ImVec4 btnHover = ImVec4(theme.inputMenuAccentColor.r/255.f, theme.inputMenuAccentColor.g/255.f, theme.inputMenuAccentColor.b/255.f, 1.0f);
    ImVec4 btnActive = ImVec4(theme.inputMenuAccentColor.r/255.f, theme.inputMenuAccentColor.g/255.f, theme.inputMenuAccentColor.b/255.f, 1.0f);

    // --- ANIMATED PARALLAX BACKGROUND ---
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    float time = (float)ImGui::GetTime();
    ImVec2 mousePos = ImGui::GetMousePos();
    
    for (int i = 0; i < 60; ++i) {
        float px = std::fmod(winSize.x * 0.05f * i + time * 15.0f * (i % 3 + 1), winSize.x);
        float py = std::fmod(winSize.y * 0.9f - time * 25.0f * (i % 4 + 1) + i * 45.0f, winSize.y);
        if (py < 0) py += winSize.y;
        
        // Mouse repel physics
        float dx = px - mousePos.x;
        float dy = py - mousePos.y;
        float dist = std::sqrt(dx*dx + dy*dy);
        if (dist < 200.0f) {
            float force = (200.0f - dist) / 200.0f;
            px += (dx / dist) * force * 80.0f;
            py += (dy / dist) * force * 80.0f;
        }

        float radius = 1.5f + (i % 3);
        float alpha = 0.1f + 0.3f * std::sin(time * 3.0f + i);
        draw_list->AddCircleFilled(ImVec2(px, py), radius, ImColor(theme.inputMenuAccentColor.r, theme.inputMenuAccentColor.g, theme.inputMenuAccentColor.b, static_cast<int>(alpha * 255)));
    }

    bool hasTitleFont = ImGui::GetIO().Fonts->Fonts.Size > 1;
    if (hasTitleFont) {
        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]); // Dùng font số 2 (size 36)
    }

    // --- add back button ---
    if (currentState != MenuState::Main) {
        ImGui::SetCursorPos(ImVec2(10.0f, 10.0f));
        if (AnimatedMenuButton("Back", ImVec2(80.0f, 35.0f), btnColor, btnHover, btnActive, &clickSound)) {
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
    
    // Glowing Title Animation
    float titleX = (winSize.x - titleSize.x) * 0.5f;
    float titleY = winSize.y * 0.22f;
    float glowAlpha = 0.6f + 0.4f * std::sin(time * 2.5f);
    ImU32 titleColor32 = ImGui::GetColorU32(ImVec4(theme.inputMenuTextColor.r/255.f, theme.inputMenuTextColor.g/255.f, theme.inputMenuTextColor.b/255.f, 1.0f));
    draw_list->AddText(ImVec2(titleX, titleY) + ImVec2(0, 3), ImGui::GetColorU32(ImVec4(btnHover.x, btnHover.y, btnHover.z, glowAlpha * 0.7f)), title);
    draw_list->AddText(ImVec2(titleX, titleY), titleColor32, title);

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
        
        // Hiệu ứng trồi lên (Slide-up entrance) lần lượt từng dòng
        static std::unordered_map<int, float> entranceAnim;
        float dt = ImGui::GetIO().DeltaTime;
        entranceAnim[i] = ImLerp(entranceAnim[i], 1.0f, dt * (10.0f - row * 2.0f));
        
        float yOffset = (1.0f - entranceAnim[i]) * 100.0f;
        float alpha = entranceAnim[i];

        ImGui::SetCursorPos(ImVec2(startX + col * (btnWidth + spacing), startY + row * (btnHeight + spacing) + yOffset));
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
        
        if (AnimatedMenuButton((*currentNames)[i].c_str(), ImVec2(btnWidth, btnHeight), btnColor, btnHover, btnActive, &clickSound)) {
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
            entranceAnim.clear(); // Reset anim cho màn sau
        }
        ImGui::PopStyleVar();
    }

    ImGui::PopStyleVar();
    ImGui::End();
}