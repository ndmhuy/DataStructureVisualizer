#define IMGUI_DEFINE_MATH_OPERATORS
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <filesystem>

#include "View/UI/UIManager.h"
#include "imgui_internal.h"
#include "View/UI/UIanimation.h"


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
    
    // Kiểm tra file font tồn tại để tránh ImGui crash (Assertion failed)
    if (std::filesystem::exists(theme.fontPath)) {
        ImFont* regularFont = io.Fonts->AddFontFromFileTTF(theme.fontPath.c_str(), 18.0f); // Font thường
        ImFont* titleFont = io.Fonts->AddFontFromFileTTF(theme.fontPath.c_str(), 36.0f);   // Font tiêu đề to
        if (!regularFont || !titleFont) {
            io.Fonts->AddFontDefault();
        }
    } else {
        std::cerr << "Warning: Font file not found at " << theme.fontPath << ". Using default ImGui font.\n";
        io.Fonts->AddFontDefault();
    }
    if (!ImGui::SFML::UpdateFontTexture()){
        std::cerr<<"Warning: UIManager::init failed to update font";
        ImGui::SFML::Shutdown();
        return false;
    }

    // --- MASSIVE UI STYLE OVERHAUL (CINEMATIC UX) ---
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowPadding = ImVec2(16.0f, 16.0f);
    style.WindowRounding = 12.0f;
    style.FramePadding = ImVec2(20.0f, 10.0f);
    style.FrameRounding = 8.0f;
    style.ItemSpacing = ImVec2(15.0f, 12.0f);
    style.ItemInnerSpacing = ImVec2(12.0f, 8.0f);
    style.ScrollbarSize = 14.0f;
    style.ScrollbarRounding = 14.0f;
    style.GrabMinSize = 16.0f;
    style.GrabRounding = 8.0f;
    style.WindowBorderSize = 0.0f; // Bỏ viền cứng, dùng bóng mờ đổ xuống
    style.PopupBorderSize = 1.0f;
    style.TabRounding = 8.0f;
    style.AntiAliasedLines = true;
    style.AntiAliasedFill = true;

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

    if (clickBuffer.loadFromFile(theme.clickSoundPath)) {
        clickSound.setBuffer(clickBuffer);
        clickSound.setVolume(60.0f);
    }

    // setup size and position for buttons
    resize(window);

    syncPlaybackUI(false, true, true, true);
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

    syncPlaybackUI(lastIsPlaying, lastIsAtBeginning, lastIsAtEnd, lastIsEmpty);

    return ok;
}

bool UIManager::consumeThemeToggleRequest() {
    bool res = themeToggleRequested;
    themeToggleRequested = false;
    return res;
}

bool UIManager::consumeThemeScaleRequest() {
    bool res = themeScaleChanged;
    themeScaleChanged = false;
    return res;
}

void UIManager::processEvent(sf::RenderWindow& window, const sf::Event& event) {
    ImGui::SFML::ProcessEvent(window, event);

    if (event.is<sf::Event::Resized>()) {
        resize(window);
    }

    // If the mouse is in the Menu of ImGUI -> ignore this click
    if (ImGui::GetIO().WantCaptureMouse) {
        return;
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
        navMenu.render(window, window.getSize());

        // Vẽ nút Theme đè lên góc phải của Main Menu
        ImGui::SetNextWindowPos(ImVec2(winSize.x - 120.0f, 10.0f));
        ImGui::SetNextWindowSize(ImVec2(120.0f, 60.0f));
        ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | 
                                 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | 
                                 ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar;
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("ThemeMenu", nullptr, flags);
        
        if (ImGui::InvisibleButton("ThemeBtn", ImVec2(100.0f, 35.0f))) {
            isDarkMode = !isDarkMode;
            themeToggleRequested = true;
            clickSound.play();
        }
        ImGui::End();
        ImGui::PopStyleVar();

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
        
        ImGui::SetCursorScreenPos(ImVec2(10.0f, 10.0f));
        if (ImGui::InvisibleButton("HomeBtn", ImVec2(80.0f, 35.0f))) {
            backToMenuClicked = true;
            isMainMenu = true;
            navMenu.resetState();
            clickSound.play();
        }

        ImGui::SetCursorScreenPos(ImVec2(340.0f, 10.0f));
        // ImGui::SetNextItemWidth(180.0f);
        // if (ImGui::SliderFloat("##ScaleApp", &theme.nodeScale, 0.3f, 2.0f, "Size %.1f")) {
        //     theme.arrayScale = theme.nodeScale;
        //     themeScaleChanged = true;
        // }
        // ImGui::SliderFloat("##speed", &speed, 0.1f, 10.0f, "Speed %.1fx");
        
        ImGui::SetCursorScreenPos(ImVec2(110.0f, 10.0f));
        if (ImGui::InvisibleButton("ThemeBtnTop", ImVec2(100.0f, 35.0f))) {
            isDarkMode = !isDarkMode;
            themeToggleRequested = true;
            clickSound.play();
        }
        
        ImGui::SetCursorScreenPos(ImVec2(230.0f, 10.0f));
        if (ImGui::InvisibleButton("CodeBtnTop", ImVec2(90.0f, 35.0f))) {
            codePanel.toggleShowCode();
            clickSound.play();
        }
        ImGui::End();

        // Render Component
        inputMenu.setDS(currentDS);
        inputMenu.render(window);
        codePanel.render(window);
        // slider.render(window);

        float winW = (float)window.getSize().x;
        float winH = (float)window.getSize().y;

        // Dòng quan trọng nhất: Pivot (0.5f, 0.5f) sẽ khóa TÂM cửa sổ vào giữa màn hình
        ImGui::SetNextWindowPos(ImVec2(winW * 0.5f, winH - 120.0f), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
        ImGui::Begin("PlaybackControls", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBackground);

        // 1. THANH SPEED NẰM TRÊN (CENTERED)
        float sliderWidth = 300.0f; 
        // Tính toán lề để đẩy cái slider vào giữa lòng cửa sổ ImGui
        float windowInnerWidth = ImGui::GetWindowSize().x;
        ImGui::SetCursorPosX((windowInnerWidth - sliderWidth) * 0.5f);

        ImGui::PushItemWidth(sliderWidth);
        ImGui::SliderFloat("##speed", &speed, 0.1f, 10.0f, "Speed %.1fx");
        ImGui::PopItemWidth();

        ImGui::Spacing(); // Cách ra một đoạn

        // 2. CỤM NÚT NẰM DƯỚI (CENTERED)
        // Dùng Group để gom 3 nút lại rồi đẩy cả Group vào giữa
        ImGui::BeginGroup();
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(40.0f, 40.0f)); 

            // Nút Backward
            ImGui::BeginDisabled(lastIsAtBeginning || lastIsPlaying); 
            if (ImGui::Button(" |<< ")) { stepBackwardClicked = true; }
            ImGui::EndDisabled();

            ImGui::SameLine(0, 15);

            // Nút Play / Pause
            if (isshowingPlay) {
                ImGui::BeginDisabled(lastIsAtEnd || lastIsEmpty);
                if (ImGui::Button("  >  ")) { playClicked = true; }
                ImGui::EndDisabled();
            } else {
                if (ImGui::Button(" || ")) { pauseClicked = true; }
            }

            ImGui::SameLine(0, 15);

            // Nút Forward
            ImGui::BeginDisabled(lastIsAtEnd || lastIsPlaying);
            if (ImGui::Button(" >>| ")) { stepForwardClicked = true; }
            ImGui::EndDisabled();

            ImGui::PopStyleVar();
        ImGui::EndGroup();

        // Căn giữa cái Group nút so với chiều rộng cửa sổ
        float groupWidth = ImGui::GetItemRectSize().x;
        ImGui::SetCursorPosX((ImGui::GetWindowSize().x - groupWidth) * 0.5f);

        ImGui::End();
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
    currentDS=-1;

    // Xóa cờ các nút điều khiển
    playClicked = false;
    pauseClicked = false;
    stepForwardClicked = false;
    stepBackwardClicked = false;

    //Reset Navigation Menu
    navMenu.resetState();

    syncPlaybackUI(false, true, true, true); // Đặt nút Playback về trạng thái vô hiệu hóa (Rỗng)
}

int UIManager::getSelectedDS() {
    if (navMenu.getSelectedDS() != -1) currentDS = navMenu.getSelectedDS();
    return currentDS;
}

void UIManager::resetDSSelection() {
    navMenu.resetSelection();
}

void UIManager::setShowMainMenu(bool show) {
    isMainMenu = show;
    if (show) {
        navMenu.resetState();
    }
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
    return inputMenu.getString3();
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

void UIManager::setCodePanelMessage(const std::string& msg) {
    codePanel.setMessage(msg);
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
    lastIsPlaying = currentIsPlaying;
    lastIsAtBeginning = isAtBeginning;
    lastIsAtEnd = isAtEnd;
    lastIsEmpty = isEmpty;

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

bool UIManager::isMouseOverUI() const {
    return ImGui::GetIO().WantCaptureMouse;
}

bool UIManager::isKeyboardCapturedByUI() const {
    return ImGui::GetIO().WantCaptureKeyboard;
}