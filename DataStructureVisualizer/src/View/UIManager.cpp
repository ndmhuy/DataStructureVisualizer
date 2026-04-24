#include <iostream>

#include "View/UIManager.h"

bool UIManager::init(sf::RenderWindow& window, const Theme& theme) {
    this->theme = theme;
    isDarkMode = false;
    themeToggleRequested = false;
    initialized = false;

    if (!ImGui::SFML::Init(window)) {
        std::cerr << "Warning: UIManager::init failed to initialize ImGui-SFML." << std::endl;
        return false;
    }

    // Load Custom Fonts cho UI typography
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->Clear();
    ImFont* regularFont = io.Fonts->AddFontFromFileTTF(theme.fontPath.c_str(), 18.0f); // Font thường
    ImFont* titleFont = io.Fonts->AddFontFromFileTTF(theme.fontPath.c_str(), 36.0f);   // Font tiêu đề to
    if (!regularFont || !titleFont) {
        io.Fonts->AddFontDefault();
    }
    ImGui::SFML::UpdateFontTexture();

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
    navMenu.init(theme);

    // setup size and position for buttons
    resize(window);

    play.setActive(false);
    pause.setActive(false);
    stepForward.setActive(false);
    stepBackward.setActive(false);
    initialized = true;

    return true;
}

bool UIManager::applyTheme(const Theme& selectedTheme) {
    theme = selectedTheme;

    navMenu.applyTheme(theme);
    inputMenu.applyTheme(theme);
    codePanel.applyTheme(theme);
    slider.applyTheme(theme);

    bool ok = true;
    if (!play.init(theme.playIconPath, theme)) {
        std::cerr << "Warning: UIManager::applyTheme failed to load play icon from '"
                  << theme.playIconPath << "'." << std::endl;
        ok = false;
    }
    if (!pause.init(theme.pauseIconPath, theme)) {
        std::cerr << "Warning: UIManager::applyTheme failed to load pause icon from '"
                  << theme.pauseIconPath << "'." << std::endl;
        ok = false;
    }
    if (!stepForward.init(theme.stepForwardIconPath, theme)) {
        std::cerr << "Warning: UIManager::applyTheme failed to load step-forward icon from '"
                  << theme.stepForwardIconPath << "'." << std::endl;
        ok = false;
    }
    if (!stepBackward.init(theme.stepBackwardIconPath, theme)) {
        std::cerr << "Warning: UIManager::applyTheme failed to load step-backward icon from '"
                  << theme.stepBackwardIconPath << "'." << std::endl;
        ok = false;
    }

    play.setActive(isshowingPlay);
    pause.setActive(!isshowingPlay);

    return ok;
}

bool UIManager::consumeThemeToggleRequest() {
    bool res = themeToggleRequested;
    themeToggleRequested = false;
    return res;
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

    if (isMainMenu) {
        return; //Block interact with SFML functions
    }

    if (play.handleEvent(window, event)) {
        playClicked = true;
    }

    if (pause.handleEvent(window, event)) {
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
    codePanel.resize(window);
}

void UIManager::update(sf::RenderWindow& window, const sf::Time& deltatime) {
    ImGui::SFML::Update(window, deltatime);
}

void UIManager::render(sf::RenderWindow& window) {
    ImVec2 winSize = ImGui::GetIO().DisplaySize;

    // Style dùng chung cho nút Theme và Home
    ImVec4 btnColor = ImVec4(theme.inputMenuPrimaryColor.r/255.f, theme.inputMenuPrimaryColor.g/255.f, theme.inputMenuPrimaryColor.b/255.f, 1.0f);
    ImVec4 btnHover = ImVec4(theme.inputMenuAccentColor.r/255.f, theme.inputMenuAccentColor.g/255.f, theme.inputMenuAccentColor.b/255.f, 1.0f);
    ImVec4 textColor = ImVec4(theme.inputMenuTextColor.r/255.f, theme.inputMenuTextColor.g/255.f, theme.inputMenuTextColor.b/255.f, 1.0f);

    if (isMainMenu) {
        navMenu.render(window);

        // Vẽ nút Theme đè lên góc phải của Main Menu
        ImGui::SetNextWindowPos(ImVec2(winSize.x - 110.0f, 10.0f));
        ImGui::SetNextWindowSize(ImVec2(100.0f, 50.0f));
        ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | 
                                 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | 
                                 ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar;
        ImGui::Begin("ThemeMenu", nullptr, flags);
        
        ImGui::PushStyleColor(ImGuiCol_Button, btnColor);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, btnHover);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, btnHover);
        ImGui::PushStyleColor(ImGuiCol_Text, textColor);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.0f);
        
        if (ImGui::Button(isDarkMode ? "Light Mode" : "Dark Mode", ImVec2(90.0f, 35.0f))) {
            isDarkMode = !isDarkMode;
            themeToggleRequested = true;
        }
        ImGui::PopStyleColor(4);
        ImGui::PopStyleVar();
        ImGui::End();

    } else {
        ImGuiWindowFlags panelFlags = 
            ImGuiWindowFlags_NoMove | 
            ImGuiWindowFlags_NoResize | 
            ImGuiWindowFlags_NoCollapse | 
            ImGuiWindowFlags_NoTitleBar | 
            ImGuiWindowFlags_NoBringToFrontOnFocus |
            ImGuiWindowFlags_NoBackground | // Bar trong suốt
            ImGuiWindowFlags_NoScrollbar;

        // ==========================================
        // 1. TOP CONTROL BAR (Chứa Home & Theme)
        // ==========================================
        float topBarHeight = 80.0f; 
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(winSize.x, topBarHeight));
        
        ImGui::Begin("TopControlBar", nullptr, panelFlags);
        
        ImGui::PushStyleColor(ImGuiCol_Button, btnColor);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, btnHover);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, btnHover);
        ImGui::PushStyleColor(ImGuiCol_Text, textColor);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.0f);
        
        ImGui::SetCursorPos(ImVec2(10.0f, 10.0f));
        if (ImGui::Button("Home", ImVec2(80.0f, 35.0f))) {
            backToMenuClicked = true;
            isMainMenu = true;
        }

        ImGui::SetCursorPos(ImVec2(winSize.x - 110.0f, 10.0f));
        if (ImGui::Button(isDarkMode ? "Light Mode" : "Dark Mode", ImVec2(90.0f, 35.0f))) {
            isDarkMode = !isDarkMode;
            themeToggleRequested = true;
        }
        
        ImGui::PopStyleColor(4);
        ImGui::PopStyleVar();
        ImGui::End();

        // Render Component
        inputMenu.setDS(navMenu.getSelectedDS());
        inputMenu.render(window);
        codePanel.render(window);
        slider.render(window);

        if (isshowingPlay) {
            play.render(window);
        } else {
            pause.render(window);
        }
        stepForward.render(window);
        stepBackward.render(window);
    }

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
// Interact with Navigation Menu
// ==========================================
void UIManager::reset() {
    isMainMenu = true;          // Bật lại màn hình Main Menu
    resetDSSelection();         // Xóa cờ chọn Data Structure hiện tại
    inputMenu.setDS(-1);        // Xóa menu nhập liệu của DS cũ
    inputMenu.resetState();     // Xóa toàn bộ popup, form, textbox đang hiện
    resetInputAction();         // Đặt lại action
    clearCodePanel();           // Xóa mảng mã giả
    resetSpeed();               // Trả Speed Slider về 1.0x

    // Xóa cờ các nút điều khiển
    playClicked = false;
    pauseClicked = false;
    stepForwardClicked = false;
    stepBackwardClicked = false;

    //Reset Navigation Menu
    //navMenu.resetState();

    syncPlaybackUI(false, true, true, true); // Đặt nút Playback về trạng thái vô hiệu hóa (Rỗng)
}

int UIManager::getSelectedDS() const {
    return navMenu.getSelectedDS();
}

void UIManager::resetDSSelection() {
    navMenu.resetSelection();
}

void UIManager::setShowMainMenu(bool show) {
    isMainMenu = show;
}

// ==========================================
// Interact with InputMenu
// ==========================================
int UIManager::getInputAction() const {
    return inputMenu.getAction();
}

int UIManager::getInputMode() const {
    return inputMenu.getMode();
}

std::string UIManager::getInputString1() const {
    return inputMenu.getString1();
}

std::string UIManager::getInputString2() const {
    return inputMenu.getString2();
}

std::string UIManager::getInputString3() const {
    return inputMenu.getString2();
}

std::string UIManager::getInputString4() const {
    return inputMenu.getString4();
}

void UIManager::resetInputAction() {
    inputMenu.resetAction();
}

// ==========================================
// Interact with CodePanel
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
// Interact with Playback Buttons & Slider
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

void UIManager::syncPlaybackUI(bool currentIsPlaying, bool isAtBeginning, bool isAtEnd, bool isEmpty) {
    if (isEmpty) {
        isshowingPlay = true;
        play.setActive(false);
        pause.setActive(false);
        stepForward.setActive(false);
        stepBackward.setActive(false);
        return;
    }

    isshowingPlay = !currentIsPlaying;
    play.setActive(isshowingPlay && !isAtEnd); // Nếu đã đến end thì disable Play
    pause.setActive(!isshowingPlay);           // Nếu đang chạy thì Pause active
    stepForward.setActive(!isAtEnd && !currentIsPlaying);           // Không thể forward nếu đã ở cuối
    stepBackward.setActive(!isAtBeginning && !currentIsPlaying);    // Không thể backward nếu ở đầu
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

void UIManager::resetSpeed() {
    slider.setValue(1.0f);
}

bool UIManager::checkBackToMenuClicked() {
    bool res = backToMenuClicked;
    backToMenuClicked = false;
    return res;
}
