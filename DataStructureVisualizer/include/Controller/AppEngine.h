#ifndef APPENGINE_H
#define APPENGINE_H

#include "Controller/Playback.h"
#include "View/Window.h"
#include "View/Theme.h"
#include "View/Renderer.h"
#include "View/UIManager.h"

class AppEngine {
    private:
    Window window;
    bool shouldClose = false;
    Theme theme;
    Renderer renderer;
    UIManager uiManager;
    Playback playbackController;

    public:
    AppEngine();

    void run();

    void processInput(const sf::Event& event);
    void update(sf::Time deltaTime);
    void render();
};

#endif // APPENGINE_H