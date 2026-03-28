#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <vector>
#include <string>
#include "imgui.h"
#include "imgui-SFML.h"
#include "../../include/View/CodePanel.h"

void CodePanel::setCode(std::vector<std::string>& listCode){
    listofCodes=listCode;
}

void CodePanel::setHighlightedLine(int line){
    highlightedline=line;
}

void CodePanel::clearCode(){
    highlightedline=-1;
    listofCodes.clear();
}

void CodePanel::render(const sf::RenderWindow& window){
    if (listofCodes.empty()) return; // Don't draw anything if there's no code

    // 1. Calculate dynamic window size
    float panelWidth = 450.0f;
    float headerHeight = 35.0f; // Height of the title section
    float padding = 15.0f;
    float lineHeight = ImGui::GetTextLineHeightWithSpacing();
    
    // Total height = header + top padding + (lines * line height) + bottom padding
    float panelHeight = headerHeight + padding + (listofCodes.size() * lineHeight) + padding;

    // 2. Position the panel at the TOP RIGHT of the screen
    float windowX = window.getSize().x;
    ImGui::SetNextWindowPos(ImVec2(windowX - panelWidth - 30.0f, 30.0f), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(panelWidth, panelHeight));

    // 3. Remove default ImGui window styling
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | 
                             ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | 
                             ImGuiWindowFlags_NoMove;
    
    ImGui::Begin("CustomCodePanel", nullptr, flags);

    // --- START CUSTOM DRAWING ---
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 p = ImGui::GetWindowPos();   
    ImVec2 s = ImGui::GetWindowSize();  
    
    // Design parameters matching the requested image
    float rounding = 6.0f;         // Slightly rounded corners 
    float borderThickness = 2.0f;  // Solid thin border

    // Layer 1: Main Background
    draw_list->AddRectFilled(p, ImVec2(p.x + s.x, p.y + s.y), bgColor, rounding);

    // Layer 2: Separator Line (between header and code body)
    draw_list->AddLine(ImVec2(p.x, p.y + headerHeight), ImVec2(p.x + s.x, p.y + headerHeight), borderColor, borderThickness);

    // Layer 3: Outer Border
    draw_list->AddRect(p, ImVec2(p.x + s.x, p.y + s.y), borderColor, rounding, 0, borderThickness);

    // --- RENDER HEADER TITLE ---
    const char* title = "Code Panel";
    ImVec2 textSize = ImGui::CalcTextSize(title);
    // Center the title perfectly inside the header box
    ImGui::SetCursorPos(ImVec2((s.x - textSize.x) / 2.0f, (headerHeight - textSize.y) / 2.0f));
    
    ImGui::PushStyleColor(ImGuiCol_Text, titleColor);
    ImGui::Text("%s", title);
    ImGui::PopStyleColor();

    // --- RENDER CODE TEXT ---
    // Move cursor below the header to start printing the code
    ImGui::SetCursorPos(ImVec2(padding, headerHeight + padding)); 
    
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 4.0f)); // Space between lines

    for (size_t i = 0; i < listofCodes.size(); ++i) {
        if (static_cast<int>(i) == highlightedline) {
            // Highlighted line: Vivid color and arrow indicator
            ImGui::PushStyleColor(ImGuiCol_Text, highlightColor);
            ImGui::Text("-> %s", listofCodes[i].c_str());
            ImGui::PopStyleColor();
        } else {
            // Normal line: Dark grey text, indented to align with the arrow
            ImGui::PushStyleColor(ImGuiCol_Text, textColor);
            ImGui::Text("   %s", listofCodes[i].c_str());
            ImGui::PopStyleColor();
        }
    }

    ImGui::PopStyleVar();
    ImGui::End();
}