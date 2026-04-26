#ifndef UI_ANIMATION_H
#define UI_ANIMATION_H

#include "imgui.h"
#include <SFML/Audio.hpp>

namespace UIanimation {
    // Hàm nội suy màu mượt mà
    ImVec4 LerpColor(const ImVec4& a, const ImVec4& b, float t);

    // Nút bấm nổi (Floating Button) có hiệu ứng Scale & Nhịp đập Pulsing Glow
    // Dùng cho Main Menu & Top Bar
    bool FloatButton(const char* label, const ImVec2& size_arg, ImVec4 colorNormal, ImVec4 colorHover, ImVec4 colorActive, sf::Sound* sound = nullptr);

    // Nút bấm dạng Cyberpunk (Có vạch trượt Accent và 4 góc Tech Corners)
    // Dùng cho Input Panel
    bool CyberButton(const char* label, const ImVec2& size_arg, ImVec4 colorNormal, ImVec4 colorHover, ImVec4 colorActive, sf::Sound* sound = nullptr);
}

#endif // UI_ANIMATION_H