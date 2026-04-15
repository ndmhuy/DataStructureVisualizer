#ifndef THEME_H
#define THEME_H

#include <SFML/Graphics.hpp>
#include <string>
class Theme {
    private:
    Theme() = default;
    static std::string s_assetsRoot;
    static const std::string& getAssetsRoot();

    public:
    std::string fontPath;
    std::string nodeImagePath;
    std::string bgImagePath;
    std::string arrayImagePath;

    std::string playIconPath;
    std::string pauseIconPath;
    std::string stepForwardIconPath;
    std::string stepBackwardIconPath;

    sf::Color textColor;
    sf::Color nodeTintColor; // Use to change the color of node
    sf::Color arrayTintColor;
    sf::Color arrowColor;
    sf::Color windowClearColor;
    unsigned int nodeTextBaseSize = 30;
    unsigned int arrayTextBaseSize = 30;
    float nodeTextVerticalOffset = 10.0f;
    sf::Color highlightColor;
    sf::Color accentColor;

    // CodePanel theme values
    sf::Color codePanelBackgroundColor;
    sf::Color codePanelBorderColor;
    sf::Color codePanelTextColor;
    sf::Color codePanelTitleColor;
    sf::Color codePanelHighlightColor;

    // InputMenu theme values
    sf::Color inputMenuPrimaryColor;
    sf::Color inputMenuAccentColor;
    sf::Color inputMenuTextColor;
    float inputMenuHeight = 60.0f;
    float inputMenuButtonWidth = 180.0f;
    float inputMenuToggleWidth = 35.0f;
    float inputMenuInputWidth = 45.0f;
    float inputMenuFormSpacing = 6.0f;
    float inputMenuAnchorYRatio = 7.0f / 8.0f;
    float inputMenuAnchorHeightOffsetMultiplier = 5.0f;
    float inputMenuOkButtonHeightRatio = 0.8f;
    float inputMenuLabelXOffset = 8.0f;
    float inputMenuUpdateLabelSpacing = 12.0f;

    // Slider theme values
    sf::Color sliderTrackColor;
    sf::Color sliderKnobColor;
    sf::Color sliderTextColor;
    float sliderMinSpeed = 0.5f;
    float sliderMaxSpeed = 4.0f;
    float sliderRadiusYDivisor = 90.0f;
    float sliderStartXRatio = 3.0f / 5.0f;
    float sliderEndXRatio = 9.0f / 10.0f;
    float sliderCenterXRatio = 3.0f / 4.0f;
    float sliderCenterYRatio = 15.0f / 16.0f;
    float sliderTextXRatio = 19.0f / 20.0f;

    // Playback button layout values
    float uiButtonRadiusYDivisor = 10.0f;
    float uiButtonRadiusXDivisor = 12.0f;
    float uiMainButtonXRatio = 1.0f / 4.0f;
    float uiStepForwardButtonXRatio = 5.0f / 12.0f;
    float uiStepBackwardButtonXRatio = 1.0f / 12.0f;
    float uiButtonsYRatio = 9.0f / 10.0f;

    // Button theme values
    sf::Color buttonNormalColor;
    sf::Color buttonHoveredColor;
    sf::Color buttonPressedColor;
    sf::Color buttonInactiveColor;
    sf::Color buttonFallbackColor;

    float codePanelWidth = 450.0f;
    float codePanelHeaderHeight = 35.0f;
    float codePanelPadding = 15.0f;
    float codePanelTopOffset = 30.0f;
    float codePanelRightOffset = 30.0f;
    float codePanelRounding = 6.0f;
    float codePanelBorderThickness = 2.0f;
    float codePanelLineSpacing = 4.0f;
    std::string codePanelTitle = "Code Panel";

    float nodeScale = 1.0f;
    float arrayScale = 1.0f;
    
    static Theme getDefaultTheme();
    static Theme getDarkTheme();
};

#endif // THEME_H