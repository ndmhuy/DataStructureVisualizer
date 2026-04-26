#define IMGUI_DEFINE_MATH_OPERATORS
#include "View/UI/UIanimation.h"
#include "imgui_internal.h"
#include <unordered_map>
#include <cmath>
#include <vector>
#include <cstdlib>

namespace {
    // Cấu trúc Hạt Particle dùng cho ParticleButton
    struct UIParticle {
        ImVec2 pos, vel;
        float alpha, size;
    };
}

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

    bool GlitchButton(const char* label, const ImVec2& size_arg, ImVec4 cNormal, ImVec4 cHover, ImVec4 cActive, sf::Sound* sound) {
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

        // Vẽ Background
        draw_list->AddRectFilled(render_bb.Min, render_bb.Max, ImGui::GetColorU32(current_color), style.FrameRounding);
        draw_list->AddRect(render_bb.Min, render_bb.Max, ImGui::GetColorU32(ImVec4(cActive.x, cActive.y, cActive.z, hEase)), style.FrameRounding, 0, 1.5f);

        ImVec2 text_pos = center - ImVec2(label_size.x * 0.5f, label_size.y * 0.5f);

        // HIỆU ỨNG GLITCH KÊNH MÀU RGB 
        float time = (float)ImGui::GetTime();
        float noise = std::sin(time * 45.0f) * std::cos(time * 15.0f); // Tạo nhiễu
        bool isGlitching = (hovered && noise > 0.5f);

        if (isGlitching) {
            float offsetStr = 3.0f * noise;
            draw_list->AddText(text_pos + ImVec2(offsetStr, 0), IM_COL32(255, 50, 50, 200), label); // Kênh Đỏ (Red)
            draw_list->AddText(text_pos + ImVec2(-offsetStr, offsetStr * 0.5f), IM_COL32(50, 255, 255, 200), label); // Kênh Xanh (Cyan)
            
            // Các sọc nhiễu vắt ngang nút
            for(int i=0; i<3; ++i) {
                float lineY = render_bb.Min.y + static_cast<float>(std::rand() % static_cast<int>(size.y));
                draw_list->AddLine(ImVec2(render_bb.Min.x, lineY), ImVec2(render_bb.Max.x, lineY), IM_COL32(255,255,255,100), 1.0f);
            }
        } else {
            draw_list->AddText(text_pos, ImGui::GetColorU32(ImGuiCol_Text), label);
        }

        if (pressed && sound) sound->play();
        return pressed;
    }

    bool ParticleButton(const char* label, const ImVec2& size_arg, ImVec4 cNormal, ImVec4 cHover, ImVec4 cActive, sf::Sound* sound) {
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
        static std::unordered_map<ImGuiID, std::vector<UIParticle>> btnParticles; // Lưu trữ hạt theo ID nút
        
        float dt = g.IO.DeltaTime;
        hAnim[id] = ImClamp(hAnim[id] + (hovered ? dt * 10.0f : -dt * 5.0f), 0.0f, 1.0f);
        cAnim[id] = ImClamp(cAnim[id] + (held ? dt * 15.0f : -dt * 10.0f), 0.0f, 1.0f);

        float hEase = 1.0f - (1.0f - hAnim[id]) * (1.0f - hAnim[id]);
        ImVec4 current_color = LerpColor(cNormal, cHover, hEase);
        if (held) current_color = cActive;

        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        ImVec2 center = bb.GetCenter();

        // Khi CLICK -> Sinh ra vụ nổ Particle
        if (pressed) {
            int numParticles = 15 + std::rand() % 10;
            for(int i = 0; i < numParticles; ++i) {
                float angle = (std::rand() % 360) * 3.14159f / 180.0f;
                float speed = 50.0f + (std::rand() % 150);
                UIParticle p;
                p.pos = center;
                p.vel = ImVec2(std::cos(angle) * speed, std::sin(angle) * speed);
                p.alpha = 1.0f;
                p.size = 1.0f + (std::rand() % 4);
                btnParticles[id].push_back(p);
            }
            if (sound) sound->play();
        }

        // Vẽ Background
        float scale = 1.0f - (cAnim[id] * 0.05f); 
        ImVec2 half_size = ImVec2(size.x * scale * 0.5f, size.y * scale * 0.5f);
        ImRect render_bb(center - half_size, center + half_size);
        
        draw_list->AddRectFilled(render_bb.Min, render_bb.Max, ImGui::GetColorU32(current_color), style.FrameRounding);
        draw_list->AddRect(render_bb.Min, render_bb.Max, ImGui::GetColorU32(ImVec4(1,1,1, hEase * 0.5f)), style.FrameRounding, 0, 1.5f);

        // Vẽ Text
        ImVec2 text_pos = center - ImVec2(label_size.x * 0.5f, label_size.y * 0.5f);
        draw_list->AddText(text_pos, ImGui::GetColorU32(ImGuiCol_Text), label);

        // HIỆU ỨNG RENDER & CẬP NHẬT CÁC HẠT (PARTICLES)
        auto& plist = btnParticles[id];
        for (size_t i = 0; i < plist.size(); ) {
            plist[i].pos.x += plist[i].vel.x * dt;
            plist[i].pos.y += plist[i].vel.y * dt;
            plist[i].vel.y += 200.0f * dt; // Thêm trọng lực nhẹ rớt xuống
            plist[i].alpha -= dt * 1.5f;   // Fade mờ dần
            
            if (plist[i].alpha <= 0.0f) {
                plist.erase(plist.begin() + i); // Xóa hạt đã chết
            } else {
                ImU32 pCol = ImGui::GetColorU32(ImVec4(cActive.x, cActive.y, cActive.z, plist[i].alpha));
                draw_list->AddCircleFilled(plist[i].pos, plist[i].size, pCol);
                
                // Vẽ đuôi sáng mờ (Glow Trail)
                draw_list->AddLine(plist[i].pos, plist[i].pos - ImVec2(plist[i].vel.x * 0.05f, plist[i].vel.y * 0.05f), 
                                   ImGui::GetColorU32(ImVec4(1,1,1, plist[i].alpha*0.5f)), plist[i].size);
                ++i;
            }
        }

        return pressed;
    }

    void DrawHoloBackground(const ImVec2& pMin, const ImVec2& pMax, ImVec4 baseColor) {
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        float time = (float)ImGui::GetTime();
        
        // Lớp 1: Đổ màu nền mờ
        ImU32 bgCol = ImGui::GetColorU32(ImVec4(baseColor.x, baseColor.y, baseColor.z, 0.1f));
        draw_list->AddRectFilled(pMin, pMax, bgCol, 8.0f);

        // Lớp 2: Vẽ lưới Radar (Grid)
        float gridSize = 30.0f;
        ImU32 gridCol = ImGui::GetColorU32(ImVec4(baseColor.x, baseColor.y, baseColor.z, 0.2f));
        
        // Dịch chuyển lưới liên tục tạo cảm giác chuyển động trôi
        float offset = std::fmod(time * 20.0f, gridSize);
        
        for (float x = pMin.x + offset; x < pMax.x; x += gridSize) {
            draw_list->AddLine(ImVec2(x, pMin.y), ImVec2(x, pMax.y), gridCol, 1.0f);
        }
        for (float y = pMin.y + offset; y < pMax.y; y += gridSize) {
            draw_list->AddLine(ImVec2(pMin.x, y), ImVec2(pMax.x, y), gridCol, 1.0f);
        }

        // Lớp 3: Tia Laser quét ngang (Scanline)
        float rectH = pMax.y - pMin.y;
        // Quét đi quét lại bằng hàm sin
        float scanY = pMin.y + rectH * 0.5f + (rectH * 0.5f) * std::sin(time * 1.5f); 
        
        ImU32 scanColCenter = ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, 0.6f));
        ImU32 scanColEdge = ImGui::GetColorU32(ImVec4(baseColor.x, baseColor.y, baseColor.z, 0.0f));
        
        // Vẽ hắt sáng dải tia quét
        draw_list->AddRectFilledMultiColor(ImVec2(pMin.x, scanY - 15.0f), ImVec2(pMax.x, scanY), scanColEdge, scanColEdge, scanColCenter, scanColCenter);
        draw_list->AddRectFilledMultiColor(ImVec2(pMin.x, scanY), ImVec2(pMax.x, scanY + 15.0f), scanColCenter, scanColCenter, scanColEdge, scanColEdge);
        draw_list->AddLine(ImVec2(pMin.x, scanY), ImVec2(pMax.x, scanY), ImGui::GetColorU32(ImVec4(baseColor.x, baseColor.y, baseColor.z, 0.8f)), 2.0f);

        // Lớp 4: Border bao bọc ngoài cùng
        draw_list->AddRect(pMin, pMax, ImGui::GetColorU32(ImVec4(baseColor.x, baseColor.y, baseColor.z, 0.5f)), 8.0f, 0, 2.0f);
    }
}