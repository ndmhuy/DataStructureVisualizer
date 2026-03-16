#ifndef APPENGINE_H
#define APPENGINE_H

#include "Controller/Playback.h"
#include "View/Window.h"

class AppEngine {
    private:
    Playback playbackController;
    Window appWindow;

    public:
    AppEngine();

    void run();

    void processInput();
    void update(float deltaTime);
    void render();
};

#endif // APPENGINE_H