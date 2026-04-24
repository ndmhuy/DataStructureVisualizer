#include "Controller/AppEngine.h"

#include <fstream>
#include <random>
#include <sstream>

#include "Model/GraphStructure/AdjacencyList.h"
#include "Model/GraphStructure/AdjacencyMatrix.h"
#include "Model/GraphStructure/IGraphStructure.h"
#include "Model/HeapStructure/MaxHeap.h"
#include "Model/HeapStructure/IHeapStructure.h"
#include "Model/HeapStructure/MinHeap.h"
#include "Model/StandardStructure/AVLTree.h"
#include "Model/StandardStructure/SinglyLinkedList.h"
#include "Model/StandardStructure/IStandardStructure.h"
#include "Utilities/PseudocodeManager.h"
#include "Utilities/MathUtils.h"

namespace {

AlgorithmType resolveAlgorithmForAction(StructureType structureType, int action) {
    switch (structureType) {
        case StructureType::SinglyLinkedList:
            if (action == 1 || action == 5) {
                return AlgorithmType::SinglyLinkedListInsert;
            }
            if (action == 2 || action == 4) {
                return AlgorithmType::SinglyLinkedListDelete;
            }
            if (action == 3) {
                return AlgorithmType::SinglyLinkedListSearch;
            }
            break;
        default:
            break;
    }

    return AlgorithmType::None;
}

void syncCodePanelWithCurrentFrame(UIManager& uiManager, const Timeline& timeline) {
    const Frame* currentFrame = timeline.getCurrentFrame();
    if (!currentFrame) {
        uiManager.setCodePanelHighlightedLine(-1);
        return;
    }

    uiManager.setCodePanelHighlightedLine(currentFrame->getCodeLineId());
}


bool isGraphStructureType(StructureType structureType) {
    return structureType == StructureType::AdjacencyList || structureType == StructureType::AdjacencyMatrix;
}
} // namespace

AppEngine::AppEngine() : window(), theme(Theme::getDefaultTheme()), renderer(window, theme), uiManager(), playbackController() {}

AppEngine::~AppEngine() {
    delete activeStructure;
}

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
    uiManager.setPlaybackControlsEnabled(structureType != StructureType::None);
    uiManager.resetSpeed();
    uiManager.clearCodePanel();
    uiManager.resetInputAction();
}

void AppEngine::handleDataActionRequest() {
    const int action = uiManager.getInputAction();
    if (action == 0) {
        return;
    }

    if (!activeStructure) {
        uiManager.resetInputAction();
        return;
    }

    const int mode = uiManager.getInputMode();
    const std::string input1 = uiManager.getInputString1();
    const std::string input2 = uiManager.getInputString2();

    const AlgorithmType algorithmType = resolveAlgorithmForAction(activeStructureType, action);
    if (algorithmType == AlgorithmType::None) {
        uiManager.clearCodePanel();
    } else {
        std::vector<std::string> pseudocode = PseudocodeManager::getPseudocode(algorithmType);
        uiManager.setCodePanelCode(pseudocode);
    }

    Timeline timeline;
    bool handled = false;

    auto* standard = dynamic_cast<IStandardStructure*>(activeStructure);
    auto* heap = dynamic_cast<IHeapStructure*>(activeStructure);
    auto* graph = dynamic_cast<IGraphStructure*>(activeStructure);

    switch (action) {
        case 1: { // Insert
            if (standard) {
                if (mode == 0) {
                    dataManager.inputFromConsole(input1);
                    if (!dataManager.getData().empty()) {
                        standard->insert(dataManager.getData()[0], timeline);
                        handled = true;
                    }
                } else if (mode == 1 || mode == 2) {
                    (mode == 1) ? dataManager.inputFromConsole(input1) : dataManager.inputFromFile(input1);
                    if (!dataManager.getData().empty()) {
                        standard->initialize(dataManager.getData(), timeline);
                        handled = true;
                    }
                }
            } else if (heap) {
                if (mode == 0) {
                    dataManager.inputFromConsole(input1);
                    if (!dataManager.getData().empty()) {
                        heap->insert(dataManager.getData()[0], timeline);
                        handled = true;
                    }
                } else if (mode == 1 || mode == 2) {
                    (mode == 1) ? dataManager.inputFromConsole(input1) : dataManager.inputFromFile(input1);
                    if (!dataManager.getData().empty()) {
                        heap->initialize(dataManager.getData(), timeline);
                        handled = true;
                    }
                }
            } else if (graph) {
                if (mode == 1 || mode == 2) {
                    (mode == 1) ? dataManager.inputFromConsoleGraph(input1) : dataManager.inputFromFileGraph(input1);
                    if (!dataManager.getDataGraph().empty()) {
                        graph->initialize(dataManager.getDataGraph(), timeline);
                        handled = true;
                    }
                }
            }
            break;
        }
        case 2: { // Delete
            if (standard) {
                dataManager.inputFromConsole(input1);
                if (!dataManager.getData().empty()) {
                    standard->remove(dataManager.getData()[0], timeline);
                    handled = true;
                }
            } else if (heap) {
                // heap->extractTop(timeline);
                // handled = true;
                dataManager.inputFromConsole(input1);
                if (!dataManager.getData().empty()) {
                    heap->remove(dataManager.getData()[0], timeline);
                    handled = true;
                }
            }
            break;
        }
        case 3: { // Search
            if (standard) {
                dataManager.inputFromConsole(input1);
                if (!dataManager.getData().empty()) {
                    standard->search(dataManager.getData()[0], timeline);
                    handled = true;
                }
            } else if (heap) {
                // heap->peek(timeline);
                // handled = true;
                dataManager.inputFromConsole(input1);
                if (!dataManager.getData().empty()) {
                    heap->search(dataManager.getData()[0], timeline);
                    handled = true;
                }
            }
            break;
        }
        case 4: { // Update
            if (standard) {
                dataManager.inputFromConsole(input1 + " " + input2);
                // UI needs to make input1 and input2 are both in single mode
                if (dataManager.getData().size() >= 2) {
                    standard->remove(dataManager.getData()[0], timeline);
                    standard->insert(dataManager.getData()[1], timeline);
                    handled = true;
                }
            }
            break;
        }
        case 5: { // Random
            if (standard || heap) {
                if (mode == 0) {
                    dataManager.randomData(1, -99, 99);
                    if (standard) {
                        standard->insert(dataManager.getData()[0], timeline);
                    } else {
                        heap->insert(dataManager.getData()[0], timeline);
                    }
                    handled = true;
                } else if (mode == 1) {
                    int size = MathUtils::getRandomInRange(3, 15);
                    dataManager.randomData(size, -99, 99);
                    if (standard) {
                        standard->initialize(dataManager.getData(), timeline);
                    } else {
                        heap->initialize(dataManager.getData(), timeline);
                    }
                    handled = true;
                }
            } else if (graph && mode == 1) {
                dataManager.randomDataGraph(8, 12, 1, 20);
                graph->initialize(dataManager.getDataGraph(), timeline);
                handled = true;
            }
            break;
        }
        default:
            break;
    }

    if (handled && timeline.getFrameCount() > 0) {
        playbackController.setTimeline(timeline);
    }

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
    if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mousePressed->button == sf::Mouse::Button::Left && !uiManager.isMouseOverUI()) {
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
}

void AppEngine::update(sf::Time deltaTime) {
    uiManager.update(window.getWindow(), deltaTime);
    handleStructureSwitchRequest();
    handleDataActionRequest();

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

    syncCodePanelWithCurrentFrame(uiManager, playbackController.getTimeline());
}

void AppEngine::render() {
    window.clear(theme.windowClearColor);
    renderer.drawBackground();
    
    const Timeline& timeline = playbackController.getTimeline();
    const Frame* currentFrame = timeline.getCurrentFrame();
    if (currentFrame) {
        renderer.renderActiveState(currentFrame);
    }

    // 3. Draw the ImGui UI on top
    uiManager.render(window.getWindow());

    // 4. Push to monitor
    window.display(); 
}
