#include "View/Core/Theme.h"

#include <filesystem>
#include <iostream>

namespace {
std::string detectAssetsRoot() {
    namespace fs = std::filesystem;
    const std::string candidates[] = {
        "assets",
        "../assets",
        "../../assets",
        "../../../assets",
        "DataStructureVisualizer/assets",
        "resources",
        "../resources",
        "../../resources",
        "../../../resources",
        "../DataStructureVisualizer/assets",
        "../DataStructureVisualizer/resources",
        "DataStructureVisualizer/assets",
        "DataStructureVisualizer/resources"
    };

    for (const auto& candidate : candidates) {
        if (fs::exists(candidate) && fs::is_directory(candidate)) {
            return candidate;
        }
    }

    std::cerr << "Warning: Could not locate assets directory from current working directory. "
              << "Using fallback relative path './assets'.\n";
    return "assets";
}
}

std::string Theme::s_assetsRoot;

const std::string& Theme::getAssetsRoot() {
    if (!s_assetsRoot.empty()) {
        return s_assetsRoot;
    }

    s_assetsRoot = detectAssetsRoot();
    return s_assetsRoot;
}

Theme Theme::getDefaultTheme() {
    Theme t;

    const std::string& assetsRoot = getAssetsRoot();

    t.fontPath = assetsRoot + "/retro-pixel-thick.ttf";
    t.bgImagePath = assetsRoot + "/background.png";
    t.nodeImagePath = assetsRoot + "/node.png";
    t.arrayImagePath = assetsRoot + "/array.png";

    t.playIconPath = assetsRoot + "/playbutton.png";
    t.pauseIconPath = assetsRoot + "/pausebutton.png";
    t.stepForwardIconPath = assetsRoot + "/stepforward.png";
    t.stepBackwardIconPath = assetsRoot + "/stepbackward.png";
    t.clickSoundPath = assetsRoot + "/click.wav"; 
    t.textColor = sf::Color({0, 71, 171});
    t.nodeTintColor = sf::Color::White; 
    t.arrayTintColor = sf::Color::White; 
    t.arrowColor = sf::Color({0, 71, 171});
    t.highlightColor = sf::Color(255, 215, 0, 255); // Màu vàng cho Node/Array được highlight
    t.accentColor = sf::Color(230, 50, 80, 255);    // Màu đỏ cho Edge được highlight
    t.visitedColor = sf::Color(180, 180, 180, 255); // Màu xám cho Node đã duyệt xong
    t.successColor = sf::Color(50, 205, 50, 255);   // Màu xanh lá cho Node đích / tìm thấy
    t.startNodeColor = sf::Color(0, 191, 255, 255);  // Màu xanh dương (Deep Sky Blue) cho Node bắt đầu
    t.windowClearColor = sf::Color(40, 44, 52, 255);
    t.bgTintColor = sf::Color(255, 255, 255, 255);  // Ở giao diện sáng thì giữ nguyên màu gốc
    t.nodeTextBaseSize = 30;
    t.arrayTextBaseSize = 30;
    t.nodeTextVerticalOffset = 10.0f;

    t.codePanelBackgroundColor = sf::Color(250, 250, 250, 255);
    t.codePanelBorderColor = sf::Color(40, 40, 40, 255);
    t.codePanelTextColor = sf::Color(40, 40, 40, 255);
    t.codePanelTitleColor = sf::Color(0, 0, 0, 255);
    t.codePanelHighlightColor = sf::Color(230, 50, 80, 255);

    t.inputMenuPrimaryColor = sf::Color(79, 71, 150, 255);
    t.inputMenuAccentColor = sf::Color(255, 148, 207, 255);
    t.inputMenuTextColor = sf::Color::White;
    t.inputMenuAnchorYRatio = 7.0f / 8.0f;
    t.inputMenuAnchorHeightOffsetMultiplier = 5.0f;
    t.inputMenuOkButtonHeightRatio = 0.8f;
    t.inputMenuLabelXOffset = 8.0f;
    t.inputMenuUpdateLabelSpacing = 12.0f;

    t.sliderTrackColor = sf::Color::White;
    t.sliderKnobColor = sf::Color(30, 144, 255, 255);
    t.sliderTextColor = sf::Color::Black;
    t.sliderRadiusYDivisor = 90.0f;
    t.sliderStartXRatio = 3.0f / 5.0f;
    t.sliderEndXRatio = 9.0f / 10.0f;
    t.sliderCenterXRatio = 3.0f / 4.0f;
    t.sliderCenterYRatio = 15.0f / 16.0f;
    t.sliderTextXRatio = 19.0f / 20.0f;

    t.uiButtonRadiusYDivisor = 10.0f;
    t.uiButtonRadiusXDivisor = 12.0f;
    t.uiMainButtonXRatio = 1.0f / 4.0f;
    t.uiStepForwardButtonXRatio = 5.0f / 12.0f;
    t.uiStepBackwardButtonXRatio = 1.0f / 12.0f;
    t.uiButtonsYRatio = 9.0f / 10.0f;

    t.buttonNormalColor = sf::Color(255, 255, 255, 255);
    t.buttonHoveredColor = sf::Color(200, 200, 200, 255);
    t.buttonPressedColor = sf::Color(150, 150, 150, 255);
    t.buttonInactiveColor = sf::Color(150, 150, 150, 100);
    t.buttonFallbackColor = sf::Color::Magenta;

    t.nodeScale = 1.0f;
    t.arrayScale = 1.0f;

    return t;
}

Theme Theme::getDarkTheme() {
    // Kế thừa các đường dẫn file, kích thước, toạ độ từ Default Theme
    Theme t = getDefaultTheme(); 

    // Ghi đè (Override) các màu sắc sang tông tối
    t.textColor = sf::Color(220, 220, 255);
    t.arrowColor = sf::Color(200, 200, 255);
    t.windowClearColor = sf::Color(20, 22, 28, 255); // Nền xám đen

    // Tự động tìm ảnh nền tối nếu có, nếu không thì tự động làm tối ảnh hiện tại
    if (std::filesystem::exists(getAssetsRoot() + "/background_dark.png")) {
        t.bgImagePath = getAssetsRoot() + "/background_dark.png";
        t.bgTintColor = sf::Color(255, 255, 255, 255);
    } else if (std::filesystem::exists(getAssetsRoot() + "/background_dark.jpg")) {
        t.bgImagePath = getAssetsRoot() + "/background_dark.jpg";
        t.bgTintColor = sf::Color(255, 255, 255, 255);
    } else {
        t.bgTintColor = sf::Color(60, 60, 70, 255); // Phủ lớp màu tối lên ảnh gốc để chống chói
    }

    t.codePanelBackgroundColor = sf::Color(30, 30, 35, 255);
    t.codePanelBorderColor = sf::Color(80, 80, 90, 255);
    t.codePanelTextColor = sf::Color(220, 220, 220, 255);
    t.codePanelTitleColor = sf::Color(255, 255, 255, 255);
    t.codePanelHighlightColor = sf::Color(255, 80, 100, 255); // Đỏ nổi bật

    t.inputMenuPrimaryColor = sf::Color(45, 40, 85, 255); // Tím sẫm hơn
    t.inputMenuAccentColor = sf::Color(200, 100, 160, 255);
    t.inputMenuTextColor = sf::Color(240, 240, 240, 255);

    t.sliderTrackColor = sf::Color(120, 120, 120, 255);
    t.sliderKnobColor = sf::Color(60, 160, 255, 255);
    t.sliderTextColor = sf::Color::White;

    t.buttonNormalColor = sf::Color(180, 180, 180, 255);
    t.buttonHoveredColor = sf::Color(220, 220, 220, 255);
    t.buttonPressedColor = sf::Color(100, 100, 100, 255);
    t.visitedColor = sf::Color(100, 100, 110, 255); // Xám đậm cho nền tối
    t.successColor = sf::Color(0, 255, 100, 255);   // Xanh neon rực rỡ
    t.startNodeColor = sf::Color(0, 220, 255, 255);  // Xanh Cyan rực rỡ

    return t;
}