#include "Controller/AppEngine.h"

AppEngine::AppEngine() : window(), theme(Theme::getDefaultTheme()), renderer(window, theme), uiManager(), playbackController() {}

void AppEngine::run() {
    if (!uiManager.init(window.getWindow(), theme)) {
        std::cerr << "Fatal Error: AppEngine::run failed to initialize UIManager." << std::endl;
        uiManager.shutdown();
        return;
    }

    if (!renderer.loadAssets()) {
        std::cerr << "Warning: Renderer failed to load some assets!" << std::endl;
    }

    sf::Clock deltaClock;
    while (window.isOpen()) {
        sf::Time deltaTime = deltaClock.restart();

        while (const std::optional<sf::Event> event = window.getWindow().pollEvent()) {
            // We use *event to get the actual event out of the optional
            processInput(*event); 
        }

        if (shouldClose) {
            uiManager.shutdown();
            window.getWindow().close();
            continue;
        }

        update(deltaTime);
        render();
    }
    uiManager.shutdown();
}

void AppEngine::processInput(const sf::Event& event) {
    if (event.is<sf::Event::Closed>()) {
        shouldClose = true;
        return;
    }
    
    if (const auto* resized = event.getIf<sf::Event::Resized>()) {
        sf::FloatRect visibleArea({0.f, 0.f}, {(float)resized->size.x, (float)resized->size.y});
        window.getWindow().setView(sf::View(visibleArea));
    }

    // Pass the event to Dear ImGui and your custom buttons
    uiManager.processEvent(window.getWindow(), event);
}

void AppEngine::update(sf::Time deltaTime) {
    uiManager.update(window.getWindow(), deltaTime);
    playbackController.update(deltaTime.asSeconds());
}

void AppEngine::render() {
    window.clear(theme.windowClearColor);
    renderer.drawBackground();
    
    const Timeline& timeline = playbackController.getTimeline();
    const Frame* currentFrame = timeline.getCurrentFrame();
    if (currentFrame) {
        // We will add logic here later to tell the Renderer to draw Arrays/Graphs
        // e.g., renderer.drawFrame(currentFrame);
    } else {
        // TEMPORARY TEST: If no timeline exists yet, draw this to prove SFML works!
        renderer.drawImageNode(400, 300, "5");
        renderer.drawArrayCell(600, 300, "42");
        renderer.drawLineWithArrow(400, 300, 50, 50, ShapeType::Circle, 
                                   600, 300, 50, 50, ShapeType::Rectangle, 3.0f, 15.0f);
    }

    // 3. Draw the ImGui UI on top
    uiManager.render(window.getWindow());

    // 4. Push to monitor
    window.display(); 
}
