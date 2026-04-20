#include "Controller/AppEngine.h"

AppEngine::AppEngine() : window(), theme(Theme::getDefaultTheme()), renderer(window, theme), uiManager(), playbackController() {}

StructureType AppEngine::mapMenuSelectionToStructureType(int selectedDS) {
    switch (selectedDS) {
        case 0:
            return StructureType::SinglyLinkedList;
        case 1:
            return StructureType::MinHeap;
        case 2:
            return StructureType::AVLTree;
        case 3:
            return StructureType::AdjacencyList;
        default:
            return StructureType::None;
    }
}

IVisualizable* AppEngine::resolveStructure(StructureType structureType) {
    switch (structureType) {
        case StructureType::SinglyLinkedList:
            return new SinglyLinkedList();
        case StructureType::MinHeap:
            return new MinHeap();
        case StructureType::MaxHeap:
            return new MaxHeap();
        case StructureType::AVLTree:
            return new AVLTree();
        case StructureType::AdjacencyList:
            return new AdjacencyList();
        case StructureType::AdjacencyMatrix:
            return new AdjacencyMatrix();
        default:
            return nullptr;
    }
}

void AppEngine::switchActiveStructure(StructureType structureType) {
    if (activeStructureType == structureType) {
        return;
    }

    if (activeStructure) {
        delete activeStructure;
        activeStructure = nullptr;
    }

    activeStructureType = structureType;
    activeStructure = resolveStructure(structureType);

    Timeline timeline;
    if (activeStructure) {
        activeStructure->clear(timeline);
    } else {
        timeline.clear();
    }

    playbackController.setTimeline(timeline);
    playbackController.pause();
    uiManager.resetSpeed();
    uiManager.clearCodePanel();
    uiManager.resetInputAction();
}

void AppEngine::handleStructureSwitchRequest() {
    const int selectedDS = uiManager.getSelectedDS();
    if (selectedDS < 0) {
        return;
    }

    const StructureType targetStructure = mapMenuSelectionToStructureType(selectedDS);
    uiManager.resetDSSelection();

    if (targetStructure == StructureType::None) {
        return;
    }

    switchActiveStructure(targetStructure);
    uiManager.setShowMainMenu(false);
}

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
    handleStructureSwitchRequest();

    if (uiManager.checkBackToMenuClicked()) {
        uiManager.setShowMainMenu(true);
        switchActiveStructure(StructureType::None);
    }

    if (uiManager.consumeThemeToggleRequest()) {
        isDarkMode = !isDarkMode;
        theme = isDarkMode ? Theme::getDarkTheme() : Theme::getDefaultTheme();
        if (!uiManager.applyTheme(theme)) {
            std::cerr << "Warning: AppEngine::update failed to apply theme to UIManager." << std::endl;
        }
        uiManager.resize(window.getWindow());
    }

    playbackController.setSpeed(uiManager.getSpeed());

    if (uiManager.checkPlayClicked()) {
        playbackController.play();
    }
    if (uiManager.checkPauseClicked()) {
        playbackController.pause();
    }
    if (uiManager.checkStepForwardClicked()) {
        playbackController.stepForward();
    }
    if (uiManager.checkStepBackwardClicked()) {
        playbackController.stepBackward();
    }

    playbackController.update(deltaTime.asSeconds());
}

void AppEngine::render() {
    window.clear(theme.windowClearColor);
    renderer.drawBackground();
    
    const Timeline& timeline = playbackController.getTimeline();
    const Frame* currentFrame = timeline.getCurrentFrame();
    if (currentFrame) {
        renderer.drawFrame(currentFrame);
    } else {
        // TEMPORARY TEST: If no timeline exists yet, draw this to prove SFML works!
        renderer.drawImageNode(sf::Vector2f(400, 300), "5");
        renderer.drawArrayCell(sf::Vector2f(600, 300), "42");
        renderer.drawLineWithArrow(sf::Vector2f(400, 300), sf::Vector2f(50, 50), ShapeType::Circle, 
                                   sf::Vector2f(600, 300), sf::Vector2f(50, 50), ShapeType::Rectangle, 3.0f, 15.0f);
    }

    // 3. Draw the ImGui UI on top
    uiManager.render(window.getWindow());

    // 4. Push to monitor
    window.display(); 
}
