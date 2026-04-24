#include "View/CodePanel.h"

void CodePanel::applyTheme(const Theme& selectedTheme){
    theme = selectedTheme;
}

void CodePanel::setCode(std::vector<std::string>& listCode){
    listofCodes = listCode;
}

void CodePanel::setHighlightedLine(int line){
    highlightedline = line;
}

void CodePanel::clearCode(){
    highlightedline = -1;
    listofCodes.clear();
}

void CodePanel::render(const sf::RenderWindow& window){
    if (listofCodes.empty()) return; // Don't draw anything if there's no code

    // 1. Calculate dynamic window size
    float panelWidth = theme.codePanelWidth;
    float headerHeight = theme.codePanelHeaderHeight;
    float padding = theme.codePanelPadding;
    float lineHeight = ImGui::GetTextLineHeightWithSpacing();
    ImU32 bgColor = IM_COL32(
        theme.codePanelBackgroundColor.r,
        theme.codePanelBackgroundColor.g,
        theme.codePanelBackgroundColor.b,
        theme.codePanelBackgroundColor.a
    );
    ImU32 borderColor = IM_COL32(
        theme.codePanelBorderColor.r,
        theme.codePanelBorderColor.g,
        theme.codePanelBorderColor.b,
        theme.codePanelBorderColor.a
    );
    ImU32 textColor = IM_COL32(
        theme.codePanelTextColor.r,
        theme.codePanelTextColor.g,
        theme.codePanelTextColor.b,
        theme.codePanelTextColor.a
    );
    ImU32 titleColor = IM_COL32(
        theme.codePanelTitleColor.r,
        theme.codePanelTitleColor.g,
        theme.codePanelTitleColor.b,
        theme.codePanelTitleColor.a
    );
    ImU32 highlightColor = IM_COL32(
        theme.codePanelHighlightColor.r,
        theme.codePanelHighlightColor.g,
        theme.codePanelHighlightColor.b,
        theme.codePanelHighlightColor.a
    );
    
    // Check if horizontal scrollbar will be needed
    bool needsScrollbar = false;
    for (size_t i = 0; i < listofCodes.size(); ++i) {
        std::string displayText = (static_cast<int>(i) == highlightedline) ? "-> " + listofCodes[i] : "   " + listofCodes[i];
        if (ImGui::CalcTextSize(displayText.c_str()).x > (panelWidth - 2 * padding)) {
            needsScrollbar = true;
            break;
        }
    }
    
    float scrollbarOffset = needsScrollbar ? ImGui::GetStyle().ScrollbarSize : 0.0f;

    // Total height = header + top padding + (lines * line height) + bottom padding + scrollbar offset
    float panelHeight = headerHeight + padding + (listofCodes.size() * lineHeight) + padding + scrollbarOffset;

    // 2. Position the panel at the TOP RIGHT of the screen
    float windowX = window.getSize().x;
    ImGui::SetNextWindowPos(
        ImVec2(windowX - panelWidth - theme.codePanelRightOffset, theme.codePanelTopOffset),
        ImGuiCond_Always
    );
    ImGui::SetNextWindowSize(ImVec2(panelWidth, panelHeight));

    // 3. Remove default ImGui window styling
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | 
                             ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                             ImGuiWindowFlags_NoBringToFrontOnFocus;
    
    ImGui::Begin("CustomCodePanel", nullptr, flags);

    // --- START CUSTOM DRAWING ---
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 p = ImGui::GetWindowPos();   
    ImVec2 s = ImGui::GetWindowSize();  
    
    float rounding = theme.codePanelRounding;
    float borderThickness = theme.codePanelBorderThickness;

    // Layer 1: Main Background
    draw_list->AddRectFilled(p, ImVec2(p.x + s.x, p.y + s.y), bgColor, rounding);

    // Layer 2: Separator Line (between header and code body)
    draw_list->AddLine(ImVec2(p.x, p.y + headerHeight), ImVec2(p.x + s.x, p.y + headerHeight), borderColor, borderThickness);

    // Layer 3: Outer Border
    draw_list->AddRect(p, ImVec2(p.x + s.x, p.y + s.y), borderColor, rounding, 0, borderThickness);

    // --- RENDER HEADER TITLE ---
    ImVec2 textSize = ImGui::CalcTextSize(theme.codePanelTitle.c_str());
    // Center the title perfectly inside the header box
    ImGui::SetCursorPos(ImVec2((s.x - textSize.x) / 2.0f, (headerHeight - textSize.y) / 2.0f));
    
    ImGui::PushStyleColor(ImGuiCol_Text, titleColor);
    ImGui::Text("%s", theme.codePanelTitle.c_str());
    ImGui::PopStyleColor();

    // --- RENDER CODE TEXT ---
    // Position the child window to start right after the header
    ImGui::SetCursorPos(ImVec2(0, headerHeight + borderThickness));

    // Make the child window background transparent so our custom one shows through
    ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(0, 0, 0, 0));

    // Create a child window for the scrolling code content.
    ImGui::BeginChild("CodeScrollingRegion", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

    // Move cursor to add padding inside the child window
    ImGui::SetCursorPos(ImVec2(padding, padding));
    
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, theme.codePanelLineSpacing));

    for (size_t i = 0; i < listofCodes.size(); ++i) {
        if (static_cast<int>(i) == highlightedline) {
            ImGui::PushStyleColor(ImGuiCol_Text, highlightColor);
            ImGui::Text("-> %s", listofCodes[i].c_str());
            ImGui::PopStyleColor();
        } else {
            ImGui::PushStyleColor(ImGuiCol_Text, textColor);
            ImGui::Text("   %s", listofCodes[i].c_str());
            ImGui::PopStyleColor();
        }
    }

    ImGui::PopStyleVar();
    ImGui::EndChild(); // End of scrolling region
    ImGui::PopStyleColor(); // Pop transparent child background

    ImGui::End();
}