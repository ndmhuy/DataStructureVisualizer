#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>

#include "imgui.h"
#include "imgui-SFML.h"
#include "View/Button.h"
#include "View/Theme.h"

class UIManager {
private:
    // state values
    bool initialized = false;
    bool isPlay = true; // Determine if it's play or pause button, initially play
    Theme theme = Theme::getDefaultTheme();
    Button play;
    Button pause;
    Button stepForward;
    Button stepBackward;

public:
    UIManager() = default;
    // Setup and teardown.
    bool init(sf::RenderWindow&, const Theme& theme); // Initial set up for ImGui + SFML

    // Event and layout handling.
    void processEvent(sf::RenderWindow&, const sf::Event&); // Pass event from SFML to ImGui

    void resize(const sf::RenderWindow& window); // resize event

    // Per-frame update and draw.
    void update(sf::RenderWindow&, const sf::Time&); // Update UI status

    void render(sf::RenderWindow&); // draw UI

    void shutdown(); // shutdown ImGui
};

#endif // UIMANAGER_H
