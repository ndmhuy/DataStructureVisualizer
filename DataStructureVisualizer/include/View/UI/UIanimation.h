#ifndef UI_ANIMATION_H
#define UI_ANIMATION_H

#include "imgui.h"
#include <SFML/Audio.hpp>

namespace UIanimation {
    // Hàm nội suy màu mượt mà
    // Linearly interpolates between two colors.
    ImVec4 LerpColor(const ImVec4& a, const ImVec4& b, float t);

    // Nút bấm nổi (Floating Button) có hiệu ứng Scale & Nhịp đập Pulsing Glow
    // Dùng cho Main Menu & Top Bar
    // Draws a floating animated button.
    bool FloatButton(const char* label, const ImVec2& size_arg, ImVec4 colorNormal, ImVec4 colorHover, ImVec4 colorActive, sf::Sound* sound = nullptr);

    // Nút bấm dạng Cyberpunk (Có vạch trượt Accent và 4 góc Tech Corners)
    // Dùng cho Input Panel
    // Draws a cyberpunk-styled button.
    bool CyberButton(const char* label, const ImVec2& size_arg, ImVec4 colorNormal, ImVec4 colorHover, ImVec4 colorActive, sf::Sound* sound = nullptr);

    // Nút bấm Glitch (Nhiễu sóng kênh màu RGB) phong cách Hacker
    // Draws a glitch-styled button.
    bool GlitchButton(const char* label, const ImVec2& size_arg, ImVec4 colorNormal, ImVec4 colorHover, ImVec4 colorActive, sf::Sound* sound = nullptr);

    // Nút bấm Particle (Bắn hạt ánh sáng nổ tung khi Click)
    // Draws a particle-explosion button.
    bool ParticleButton(const char* label, const ImVec2& size_arg, ImVec4 colorNormal, ImVec4 colorHover, ImVec4 colorActive, sf::Sound* sound = nullptr);

    // Thanh trượt (Slider) có hiệu ứng phình to, rung lắc khi kéo và giật nảy số liệu
    // Thay thế trực tiếp cho ImGui::SliderFloat truyền thống
    // Draws an animated slider control.
    bool JuicySliderFloat(const char* label, float* v, float v_min, float v_max, const char* format, ImVec4 cNormal, ImVec4 cHover, ImVec4 cActive);

    // Khung Nền Hologram Viễn Tưởng (Tia quét Radar & Lưới điện)
    // Draws the hologram background.
    void DrawHoloBackground(const ImVec2& pMin, const ImVec2& pMax, ImVec4 baseColor);

    // Khung Nền Cyberpunk (Matrix Rain & 3D Grid) dùng cho Main Menu
    // Draws the cyberpunk main-menu background.
    void DrawCyberpunkBackground(const ImVec2& winSize, float time, const ImVec2& mousePos, ImVec4 baseColor, ImVec4 hoverColor);
}

#endif // UI_ANIMATION_H