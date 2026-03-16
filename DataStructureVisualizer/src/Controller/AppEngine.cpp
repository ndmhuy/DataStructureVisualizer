#include "Controller/AppEngine.h"

AppEngine::AppEngine() : appWindow(), playbackController() {}

void AppEngine::run() {
    appWindow.init();
    while (appWindow.isOpen()) {
        float deltaTIme = appWindow.getDeltaTime();
        processInput();
        update(deltaTime);
        render();
    }
}

void AppEngine::processInput() {
    appWindow.HandleEvent();
}

void AppEngine::update(float deltaTime) {
    playbackController.update(deltaTime);
}

void AppEngine::render() {
    appWindow.clear();
    // Render current frame from playbackController's timeline
    const Timeline& timeline = playbackController.getTimeline();
    const Frame* currentFrame = timeline.getCurrentFrame();
    if (currentFrame) {
        // Render the current frame's data structure state
        // (This would involve drawing the data structure visualization based on the frame's data)
    }
    appWindow.display();
}