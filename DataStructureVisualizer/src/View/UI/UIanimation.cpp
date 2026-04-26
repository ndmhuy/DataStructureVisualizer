#define IMGUI_DEFINE_MATH_OPERATORS
#include "View/UI/UIanimation.h"
#include "imgui_internal.h"
#include <unordered_map>
#include <cmath>

namespace UIanimation {

    ImVec4 LerpColor(const ImVec4& a, const ImVec4& b, float t) {
        return ImVec4(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t, a.z + (b.z - a.z) * t, a.w + (b.w - a.w) * t);
    }

    bool FloatButton(const char* label, const ImVec2& size_arg, ImVec4 colorNormal, ImVec4 colorHover, ImVec4 colorActive, sf::Sound* sound) {
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
        
        float scale = 1.0f - (cEase * 0.05f); 
        ImVec2 center = bb.GetCenter();
        ImVec2 half_size = ImVec2(size.x * scale * 0.5f, size.y * scale * 0.5f);
        ImRect render_bb(center - half_size, center + half_size);

        // Hiệu ứng MỚI: Nhịp đập ánh sáng (Pulsing Glow) theo đồ thị Sin
        if (hEase > 0.0f) {
            float time = (float)ImGui::GetTime();
            float pulse = 0.5f + 0.5f * std::sin(time * 6.0f); 
            float glow_size = 8.0f * hEase + 6.0f * pulse * hEase; 
            
            ImVec4 glowColor = colorHover;
            glowColor.w = hEase * (0.2f + 0.3f * pulse); // Alpha nhấp nháy mượt
            
            draw_list->AddRectFilled(render_bb.Min - ImVec2(glow_size, glow_size), 
                                     render_bb.Max + ImVec2(glow_size, glow_size), 
                                     ImGui::GetColorU32(glowColor), 
                                     style.FrameRounding + glow_size);
        }

        draw_list->AddRectFilled(render_bb.Min, render_bb.Max, ImGui::GetColorU32(current_color), style.FrameRounding);
        draw_list->AddRect(render_bb.Min, render_bb.Max, ImGui::GetColorU32(ImVec4(1,1,1, hEase * 0.6f)), style.FrameRounding, 0, 2.0f);

        ImVec2 text_pos = center - ImVec2(label_size.x * 0.5f, label_size.y * 0.5f);
        text_pos.y -= hEase * 3.0f;
        
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 1, 1));
        draw_list->AddText(text_pos, ImGui::GetColorU32(ImGuiCol_Text), label);
        ImGui::PopStyleColor();

        if (pressed && sound) sound->play();
        return pressed;
    }

    bool CyberButton(const char* label, const ImVec2& size_arg, ImVec4 cNormal, ImVec4 cHover, ImVec4 cActive, sf::Sound* sound) {
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

        static std::unordered_map<ImGuiID, float> hAnim, cAnim;
        float dt = g.IO.DeltaTime;
        hAnim[id] = ImClamp(hAnim[id] + (hovered ? dt * 10.0f : -dt * 5.0f), 0.0f, 1.0f);
        cAnim[id] = ImClamp(cAnim[id] + (held ? dt * 15.0f : -dt * 10.0f), 0.0f, 1.0f);

        float hEase = 1.0f - (1.0f - hAnim[id]) * (1.0f - hAnim[id]);
        ImVec4 current_color = LerpColor(cNormal, cHover, hEase);
        if (held) current_color = cActive;

        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        
        float scale = 1.0f - (cAnim[id] * 0.05f); 
        ImVec2 center = bb.GetCenter();
        ImVec2 half_size = ImVec2(size.x * scale * 0.5f, size.y * scale * 0.5f);
        ImRect render_bb(center - half_size, center + half_size);

        draw_list->AddRectFilled(render_bb.Min, render_bb.Max, ImGui::GetColorU32(current_color), style.FrameRounding);
        
        // Hiệu ứng MỚI: Vạch sáng trượt dọc và 4 góc cơ khí (Tech Corners)
        if (hEase > 0.01f) {
            float time = (float)ImGui::GetTime();
            float slide = std::abs(std::sin(time * 3.0f)); // Trượt lướt lên xuống
            float barHeight = (render_bb.Max.y - render_bb.Min.y) * hEase * 0.8f;
            float midY = render_bb.Min.y + (render_bb.Max.y - render_bb.Min.y) * slide;
            
            float startY = ImClamp(midY - barHeight * 0.5f, render_bb.Min.y, render_bb.Max.y);
            float endY = ImClamp(midY + barHeight * 0.5f, render_bb.Min.y, render_bb.Max.y);

            draw_list->AddRectFilled(ImVec2(render_bb.Min.x, startY), 
                                     ImVec2(render_bb.Min.x + 4.0f, endY), 
                                     ImGui::GetColorU32(cActive), style.FrameRounding);
            
            float corner = 8.0f * hEase;
            ImU32 cornerCol = ImGui::GetColorU32(ImVec4(cActive.x, cActive.y, cActive.z, hEase));
            draw_list->AddLine(render_bb.Min, render_bb.Min + ImVec2(corner, 0), cornerCol, 2.0f);
            draw_list->AddLine(render_bb.Min, render_bb.Min + ImVec2(0, corner), cornerCol, 2.0f);
            draw_list->AddLine(render_bb.Max, render_bb.Max - ImVec2(corner, 0), cornerCol, 2.0f);
            draw_list->AddLine(render_bb.Max, render_bb.Max - ImVec2(0, corner), cornerCol, 2.0f);
        }

        ImVec2 text_pos = center - ImVec2(label_size.x * 0.5f, label_size.y * 0.5f);
        text_pos.x += hEase * 6.0f;
        draw_list->AddText(text_pos, ImGui::GetColorU32(ImGuiCol_Text), label);

        if (pressed && sound) sound->play();
        return pressed;
    }
}