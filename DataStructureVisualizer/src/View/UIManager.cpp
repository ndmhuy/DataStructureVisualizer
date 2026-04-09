#include <iostream>

#include "View/UIManager.h"

bool UIManager::init(sf::RenderWindow& window, const Theme& theme) {
    this->theme = theme;
    initialized = false;

    if (!ImGui::SFML::Init(window)) {
        std::cerr << "Warning: UIManager::init failed to initialize ImGui-SFML." << std::endl;
        return false;
    }

    if (!play.init(theme.playIconPath, theme)) {
        std::cerr << "Warning: UIManager::init failed to load play icon from '"
                  << theme.playIconPath << "'." << std::endl;
        ImGui::SFML::Shutdown();
        return false;
    }
    if (!pause.init(theme.pauseIconPath, theme)) {
        std::cerr << "Warning: UIManager::init failed to load pause icon from '"
                  << theme.pauseIconPath << "'." << std::endl;
        ImGui::SFML::Shutdown();
        return false;
    }
    if (!stepForward.init(theme.stepForwardIconPath, theme)) {
        std::cerr << "Warning: UIManager::init failed to load step-forward icon from '"
                  << theme.stepForwardIconPath << "'." << std::endl;
        ImGui::SFML::Shutdown();
        return false;
    }
    if (!stepBackward.init(theme.stepBackwardIconPath, theme)) {
        std::cerr << "Warning: UIManager::init failed to load step-backward icon from '"
                  << theme.stepBackwardIconPath << "'." << std::endl;
        ImGui::SFML::Shutdown();
        return false;
    }

    inputMenu.init(theme);
    codePanel.applyTheme(theme);
    slider.init(&speed,theme);

    // setup size and position for buttons
    resize(window);

    play.setActive(false);
    pause.setActive(false);
    stepForward.setActive(false);
    stepBackward.setActive(false);
    initialized = true;

    return true;
}

void UIManager::processEvent(sf::RenderWindow& window, const sf::Event& event) {
    ImGui::SFML::ProcessEvent(window, event);

    // If the mouse is in the Menu of ImGUI -> ignore this click
    if (ImGui::GetIO().WantCaptureMouse) {
        return;
    }

    if (event.is<sf::Event::Resized>()) {
        resize(window);
    }

    if (play.handleEvent(window, event)) {
        isPlay = false;
        play.setActive(isPlay);
        pause.setActive(!isPlay);
        playClicked = true;
    }

    if (pause.handleEvent(window, event)) {
        isPlay = true;
        play.setActive(isPlay);
        pause.setActive(!isPlay);
        pauseClicked = true;
    }

    if (stepForward.handleEvent(window, event)) {
        stepForwardClicked = true;
    }

    if (stepBackward.handleEvent(window, event)) {
        stepBackwardClicked = true;
    }

    slider.handleEvent(window, event);
}

void UIManager::resize(const sf::RenderWindow& window) {
    sf::Vector2u windowsize = window.getSize();
    float x = windowsize.x;
    float y = windowsize.y;
    float rad = std::min(y / theme.uiButtonRadiusYDivisor, x / theme.uiButtonRadiusXDivisor);

    play.resize(sf::Vector2f{theme.uiMainButtonXRatio * x, theme.uiButtonsYRatio * y}, rad);
    pause.resize(sf::Vector2f{theme.uiMainButtonXRatio * x, theme.uiButtonsYRatio * y}, rad);
    stepForward.resize(sf::Vector2f{theme.uiStepForwardButtonXRatio * x, theme.uiButtonsYRatio * y}, rad);
    stepBackward.resize(sf::Vector2f{theme.uiStepBackwardButtonXRatio * x, theme.uiButtonsYRatio * y}, rad);
    
    slider.resize(window);
}

void UIManager::update(sf::RenderWindow& window, const sf::Time& deltatime) {
    ImGui::SFML::Update(window, deltatime);
}

void UIManager::render(sf::RenderWindow& window) {
    // ImGuiWindowFlags panelFlags = 
    //     ImGuiWindowFlags_NoMove | 
    //     ImGuiWindowFlags_NoResize | 
    //     ImGuiWindowFlags_NoCollapse | 
    //     ImGuiWindowFlags_NoTitleBar;
    ImGuiWindowFlags panelFlags = 
        ImGuiWindowFlags_NoMove | 
        ImGuiWindowFlags_NoResize | 
        ImGuiWindowFlags_NoCollapse | 
        ImGuiWindowFlags_NoTitleBar;

    // // Get the exact size of the user's game window
    // ImVec2 winSize = ImGui::GetIO().DisplaySize;
    ImVec2 winSize = ImGui::GetIO().DisplaySize;

    // // ==========================================
    // // 1. TOP CONTROL BAR (Input Menu)
    // // ==========================================
    // float topBarHeight = 80.0f; // Adjust this based on your Theme
    // ImGui::SetNextWindowPos(ImVec2(0, 0));
    // ImGui::SetNextWindowSize(ImVec2(winSize.x, topBarHeight));
    // ==========================================
    // 1. TOP CONTROL BAR (Input Menu & Slider)
    // ==========================================
    float topBarHeight = 80.0f; // Bạn có thể lấy từ theme.inputMenuHeight
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(winSize.x, topBarHeight));
    
    // ImGui::Begin("TopControlBar", nullptr, panelFlags);
    // // Call your InputMenu render function here!
    // // e.g., inputMenu.render(window); 
    // ImGui::End();
    ImGui::Begin("TopControlBar", nullptr, panelFlags);
    inputMenu.render(window);
    slider.render(window);
    ImGui::End();

    // // ==========================================
    // // 2. RIGHT CODE PANEL (Pseudocode)
    // // ==========================================
    // float rightPanelWidth = 450.0f; // Width of the pseudocode box
    // float bottomBarHeight = 100.0f; // Space to leave at the bottom for SFML buttons
    
    // // Position it under the Top Bar, and push it all the way to the right
    // ImGui::SetNextWindowPos(ImVec2(winSize.x - rightPanelWidth, topBarHeight));
    // // Its height stretches down to the playback bar
    // ImGui::SetNextWindowSize(ImVec2(rightPanelWidth, winSize.y - topBarHeight - bottomBarHeight));
    
    // ImGui::Begin("RightCodePanel", nullptr, panelFlags);
    // // Call your CodePanel render function here!
    // // e.g., codePanel.render(window);
    // ImGui::End();
    // ==========================================
    // 2. RIGHT CODE PANEL (Pseudocode)
    // ==========================================
    // Class CodePanel đã tự xử lý window size/pos bên trong file CodePanel.cpp
    // Bạn chỉ cần gọi hàm render:
    codePanel.render(window);

    if (isPlay) {
        play.render(window);
    } else {
        pause.render(window);
    }

    stepForward.render(window);
    stepBackward.render(window);
    ImGui::SFML::Render(window);
}

void UIManager::shutdown() {
    if (!initialized) {
        return;
    }

    ImGui::SFML::Shutdown();
    initialized = false;
}

// ==========================================
// Giao tiếp với InputMenu
// ==========================================
int UIManager::getInputAction() const {
    return inputMenu.getAction();
}

int UIManager::getInputK() const {
    return inputMenu.getK();
}

int UIManager::getInputR() const {
    return inputMenu.getR();
}

void UIManager::resetInputAction() {
    inputMenu.resetAction();
}

// ==========================================
// Giao tiếp với CodePanel
// ==========================================
void UIManager::setCodePanelCode(std::vector<std::string>& code) {
    codePanel.setCode(code);
}

void UIManager::setCodePanelHighlightedLine(int line) {
    codePanel.setHighlightedLine(line);
}

void UIManager::clearCodePanel() {
    codePanel.clearCode();
}

// ==========================================
// Giao tiếp với Playback Buttons & Slider
// ==========================================
bool UIManager::checkPlayClicked() {
    bool res = playClicked;
    playClicked = false;
    return res;
}

bool UIManager::checkPauseClicked() {
    bool res = pauseClicked;
    pauseClicked = false;
    return res;
}

bool UIManager::checkStepForwardClicked() {
    bool res = stepForwardClicked;
    stepForwardClicked = false;
    return res;
}

bool UIManager::checkStepBackwardClicked() {
    bool res = stepBackwardClicked;
    stepBackwardClicked = false;
    return res;
}

float UIManager::getSpeed() const {
    return speed;
}
