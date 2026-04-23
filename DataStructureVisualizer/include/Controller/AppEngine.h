#ifndef APPENGINE_H
#define APPENGINE_H

#include "Controller/Playback.h"

#include "Model/IVisualizable.h"

#include "Utilities/StructureType.h"

#include "View/Core/Theme.h"
#include "View/Core/Window.h"
#include "View/Render/Renderer.h"
#include "View/UI/UIManager.h"

class AppEngine {
private:
    Window window;
    bool shouldClose = false;
    bool isDarkMode = false;
    Theme theme;
    Renderer renderer;
    UIManager uiManager;
    Playback playbackController;

    IVisualizable* activeStructure = nullptr;
    StructureType activeStructureType = StructureType::None;

    static StructureType mapMenuSelectionToStructureType(int selectedDS);
    IVisualizable* resolveStructure(StructureType structureType);
    void handleStructureSwitchRequest();
    void switchActiveStructure(StructureType structureType);
    void handleDataActionRequest();

public:
    AppEngine();
    ~AppEngine();

    void run();

    void processInput(const sf::Event& event);
    void update(sf::Time deltaTime);
    void render();
};

#endif // APPENGINE_H