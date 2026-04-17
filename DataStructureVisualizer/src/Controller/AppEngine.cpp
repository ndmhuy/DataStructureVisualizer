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
    if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mousePressed->button == sf::Mouse::Button::Left) {
            sf::Vector2f worldPos = window.getWindow().mapPixelToCoords(mousePressed->position);
            renderer.handleMousePress(worldPos);
        }
    } else if (const auto* mouseMoved = event.getIf<sf::Event::MouseMoved>()) {
        sf::Vector2f worldPos = window.getWindow().mapPixelToCoords(mouseMoved->position);
        renderer.handleMouseMove(worldPos);
    } else if (const auto* mouseReleased = event.getIf<sf::Event::MouseButtonReleased>()) {
        if (mouseReleased->button == sf::Mouse::Button::Left) {
            renderer.handleMouseRelease();
        }
    }

    // Pass the event to Dear ImGui and your custom buttons
    uiManager.processEvent(window.getWindow(), event);
}

void AppEngine::update(sf::Time deltaTime) {
    uiManager.update(window.getWindow(), deltaTime);

    if (uiManager.consumeThemeToggleRequest()) {
        isDarkMode = !isDarkMode;
        theme = isDarkMode ? Theme::getDarkTheme() : Theme::getDefaultTheme();
        if (!uiManager.applyTheme(theme)) {
            std::cerr << "Warning: AppEngine::update failed to apply theme to UIManager." << std::endl;
        }
        uiManager.resize(window.getWindow());
    }

    playbackController.update(deltaTime.asSeconds());
}

void AppEngine::render() {
    window.clear(theme.windowClearColor);
    renderer.drawBackground();
    
    const Timeline& timeline = playbackController.getTimeline();
    const Frame* currentFrame = timeline.getCurrentFrame();
    if (currentFrame) {
        // We will add logic here later to tell the Renderer to draw Arrays/Graphs
        renderer.drawFrame(currentFrame);
    } else {
        // TEMPORARY TEST: If no timeline exists yet, draw this to prove SFML works!
        renderer.drawImageNode(sf::Vector2f(400, 300), "5");
        renderer.drawArrayCell(sf::Vector2f(600, 300), "42");
        renderer.drawLineWithArrow(sf::Vector2f(400, 300), sf::Vector2f(50, 50), ShapeType::Circle, 
                                   sf::Vector2f(600, 300), sf::Vector2f(50, 50), ShapeType::Rectangle, 3.0f, 15.0f);
        // TEMPORARY TEST: Tạo một Frame đồ thị giả (dummy) để test kéo thả node!
        Frame dummyFrame({0, 1, 2}, {{0, 1, 1}, {1, 2, 1}, {2, 0, 1}}, {}, {}, 0, "Testing Drag & Drop");
        renderer.drawFrame(&dummyFrame);
    }

    // 3. Draw the ImGui UI on top
    uiManager.render(window.getWindow());

    // 4. Push to monitor
    window.display(); 
}
