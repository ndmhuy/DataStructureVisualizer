#include <fstream>
#include <random>
#include <sstream>

#include "Model/GraphStructure/AdjacencyList.h"
#include "Model/GraphStructure/AdjacencyMatrix.h"
#include "Model/GraphStructure/GridGraph.h"
#include "Model/GraphStructure/IGraphStructure.h"
#include "Model/HeapStructure/IHeapStructure.h"
#include "Model/HeapStructure/MaxHeap.h"
#include "Model/HeapStructure/MinHeap.h"
#include "Model/StandardStructure/AVLTree.h"
#include "Model/StandardStructure/IStandardStructure.h"
#include "Model/StandardStructure/SinglyLinkedList.h"
#include "Utilities/LayoutConfig.h"
#include "Utilities/PseudocodeManager.h"
#include "Utilities/MathUtils.h"
#include "Controller/AppEngine.h"

namespace {

AlgorithmType resolveAlgorithmForAction(StructureType structureType, int action) {
    switch (structureType) {
        case StructureType::SinglyLinkedList:
            if (action == 1 || action == 5) {
                return AlgorithmType::SinglyLinkedListInsert;
            }
            if (action == 2) {
                return AlgorithmType::SinglyLinkedListDelete;
            }
            if (action == 3) {
                return AlgorithmType::SinglyLinkedListSearch;
            }
            if (action == 4) {
                return AlgorithmType::SinglyLinkedListUpdate;
            }
            break;
        case StructureType::GridGraph:
            if (action == 6) {
                return AlgorithmType::GraphGridBFSShortestPath;
            }
            if (action == 7) {
                return AlgorithmType::GraphAStar;
            }
            break;
        case StructureType::AdjacencyList:
        case StructureType::AdjacencyMatrix:
            if (action == 8) {
                return AlgorithmType::GraphDAGShortestPath;
            }
            if (action == 9) {
                return AlgorithmType::GraphDijkstra;
            }
            if (action == 10) {
                return AlgorithmType::GraphAStar;
            }
            if (action == 11) {
                return AlgorithmType::GraphBellmanFord;
            }
            if (action == 12) {
                return AlgorithmType::GraphFloydWarshall;
            }
            if (action == 13) {
                return AlgorithmType::GraphJohnson;
            }
            break;
        default:
            break;
    }
    
    return AlgorithmType::None;
}
            
void syncCodePanelWithCurrentFrame(UIManager& uiManager,
const Timeline& timeline) {
    const Frame* currentFrame = timeline.getCurrentFrame();
    if (!currentFrame) {
        uiManager.setCodePanelHighlightedLine(-1);
        return;
    }
    
    uiManager.setCodePanelHighlightedLine(currentFrame->getCodeLineId());
}
    
bool isGraphStructureType(StructureType structureType) {
    return structureType == StructureType::AdjacencyList ||
    structureType == StructureType::AdjacencyMatrix ||
    structureType == StructureType::GridGraph;
}

bool isPanning = false;
sf::Vector2i lastPanMousePos;

enum class UndoAction { NodeDrag, CameraPan };
std::vector<UndoAction> undoHistory;
std::vector<sf::Vector2f> viewCenterHistory;
sf::Vector2f prePanCenter;

void clearUndoHistory() {
    undoHistory.clear();
    viewCenterHistory.clear();
}

void clampView(sf::View& view, const sf::Vector2u& winSize) {
    sf::Vector2f viewCenter = view.getCenter();
    sf::Vector2f viewSize = view.getSize();
    float halfW = viewSize.x / 2.0f;
    float halfH = viewSize.y / 2.0f;
    
    float winW = static_cast<float>(winSize.x);
    float winH = static_cast<float>(winSize.y);
    float limitMinX = -winW * 1.5f;
    float limitMaxX = winW * 2.5f;
    float limitMinY = -winH * 1.5f;
    float limitMaxY = winH * 2.5f;
    
    if (viewCenter.x - halfW < limitMinX) viewCenter.x = limitMinX + halfW;
    if (viewCenter.x + halfW > limitMaxX) viewCenter.x = limitMaxX - halfW;
    if (viewCenter.y - halfH < limitMinY) viewCenter.y = limitMinY + halfH;
    if (viewCenter.y + halfH > limitMaxY) viewCenter.y = limitMaxY - halfH;

    view.setCenter(viewCenter);
}
} // namespace
            
AppEngine::AppEngine()
: window(), theme(Theme::getDefaultTheme()), renderer(window, theme),
uiManager(), playbackController() {}

AppEngine::~AppEngine() { delete activeStructure; }

StructureType AppEngine::mapMenuSelectionToStructureType(int selectedDS) {
    switch (selectedDS) {
        case 0:
        return StructureType::SinglyLinkedList;
        case 1:
        return StructureType::MinHeap;
        case 2:
        return StructureType::MaxHeap;
        case 3:
        return StructureType::AVLTree;
        case 4:
        return StructureType::GridGraph;
        case 5:
        return StructureType::AdjacencyMatrix;
        case 6:
        return StructureType::AdjacencyList;
        default:
        return StructureType::None;
    }
}

IVisualizable* AppEngine::resolveStructure(StructureType structureType) {
    switch (structureType) {
        case StructureType::SinglyLinkedList:
        return new SinglyLinkedList(LayoutConfig{});
        case StructureType::MinHeap:
        return new MinHeap(LayoutConfig{});        
        case StructureType::MaxHeap:
        return new MaxHeap(LayoutConfig{});          
        case StructureType::AVLTree:
        return new AVLTree(LayoutConfig{});
        case StructureType::AdjacencyList:
        return new AdjacencyList(LayoutConfig{});
        case StructureType::AdjacencyMatrix:
        return new AdjacencyMatrix(LayoutConfig{});
        case StructureType::GridGraph:
        return new GridGraph(10, 10);
        default:
        return nullptr;
    }
}

void AppEngine::switchActiveStructure(StructureType structureType) {
    window.getWindow().setView(window.getWindow().getDefaultView());

    if (activeStructureType == structureType) {
        return;
    }
    
    if (activeStructure) {
        delete activeStructure;
        activeStructure = nullptr;
    }
    
    activeStructureType = structureType;
    activeStructure = resolveStructure(structureType);
    renderer.resetCustomPositions();
    clearUndoHistory();

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
    const std::string input3 = uiManager.getInputString3();
    const std::string input4 = uiManager.getInputString4();
    
    const AlgorithmType algorithmType =
    resolveAlgorithmForAction(activeStructureType, action);
    if (algorithmType == AlgorithmType::None) {
        uiManager.clearCodePanel();
    } else {
        std::vector<std::string> pseudocode =
        PseudocodeManager::getPseudocode(algorithmType);
        uiManager.setCodePanelCode(pseudocode);
    }
    
    Timeline timeline;
    bool handled = false;
    
    // New switch action
    if (activeStructureType == StructureType::SinglyLinkedList || activeStructureType == StructureType::AVLTree) {
        auto* standard = dynamic_cast<IStandardStructure*>(activeStructure);
        if (!standard) return;

        switch (action) {
            case 1: { // 1. INIT
                if (mode == 0) { // Array
                    dataManager.inputFromConsole(input1);
                    if (!dataManager.getData().empty()) {
                        standard->initialize(dataManager.getData(), timeline);
                        handled = true;
                    }
                } else if (mode == 1) { // Random
                    int size = MathUtils::getRandomInRange(3, 15);
                    dataManager.randomData(size, -99, 99);
                    if (!dataManager.getData().empty()) {
                        standard->initialize(dataManager.getData(), timeline);
                        handled = true;
                    }
                } else if (mode == 2) { // File
                    dataManager.inputFromFile(input1);
                    if (!dataManager.getData().empty()) {
                        standard->initialize(dataManager.getData(), timeline);
                        handled = true;
                    }
                }
                break;
            }
            case 2: { // 2. INSERT
                if (mode == 0) { // Single
                    dataManager.inputFromConsole(input1);
                    if (!dataManager.getData().empty()) {
                        standard->insert(dataManager.getData()[0], timeline);
                        handled = true;
                    }
                } else if (mode == 1) { // Random
                    dataManager.randomData(1, -99, 99);
                    if (!dataManager.getData().empty()) {
                        standard->insert(dataManager.getData()[0], timeline);
                        handled = true;
                    }
                }
                break;
            }
            case 3: { // 3. SEARCH
                dataManager.inputFromConsole(input1);
                if (!dataManager.getData().empty()) {
                    standard->search(dataManager.getData()[0], timeline);
                    handled = true;
                }
                break;
            }
            case 4: { // 4. DELETE
                dataManager.inputFromConsole(input1);
                if (!dataManager.getData().empty()) {
                    standard->remove(dataManager.getData()[0], timeline);
                    handled = true;
                }
                break;
            }
            case 5: { // 5. UPDATE
                dataManager.inputFromConsole(input1 + " " + input2);
                if (dataManager.getData().size() >= 2) {
                    standard->remove(dataManager.getData()[0], timeline);
                    standard->insert(dataManager.getData()[1], timeline);
                    handled = true;
                }
                break;
            }
            case 6: { // 6. CLEAR
                standard->clear(timeline);
                handled = true;
                break;
            }
        }
    }
    else if (activeStructureType == StructureType::MinHeap || activeStructureType == StructureType::MaxHeap) {
        auto* heap = dynamic_cast<IHeapStructure*>(activeStructure);
        if (!heap) return;

        switch (action) {
            case 1: { // 1. INIT
                if (mode == 0) { // Array
                    dataManager.inputFromConsole(input1);
                    if (!dataManager.getData().empty()) {
                        heap->initialize(dataManager.getData(), timeline);
                        handled = true;
                    }
                } else if (mode == 1) { // Random
                    int size = MathUtils::getRandomInRange(3, 15);
                    dataManager.randomData(size, -99, 99);
                    if (!dataManager.getData().empty()) {
                        heap->initialize(dataManager.getData(), timeline);
                        handled = true;
                    }
                } else if (mode == 2) { // File
                    dataManager.inputFromFile(input1);
                    if (!dataManager.getData().empty()) {
                        heap->initialize(dataManager.getData(), timeline);
                        handled = true;
                    }
                }
                break;
            }
            case 2: { // 2. INSERT
                if (mode == 0) { // Single
                    dataManager.inputFromConsole(input1);
                    if (!dataManager.getData().empty()) {
                        heap->insert(dataManager.getData()[0], timeline);
                        handled = true;
                    }
                } else if (mode == 1) { // Random
                    dataManager.randomData(1, -99, 99);
                    if (!dataManager.getData().empty()) {
                        heap->insert(dataManager.getData()[0], timeline);
                        handled = true;
                    }
                }
                break;
            }
            case 3: { // 3. EXTRACTTOP
                heap->extractTop(timeline);
                handled = true;
                break;
            }
            case 4: { // 4. PEEK
                heap->peek(timeline);
                handled = true;
                break;
            }
            case 5: { // 5. SEARCH
                dataManager.inputFromConsole(input1);
                if (!dataManager.getData().empty()) {
                    heap->search(dataManager.getData()[0], timeline);
                    handled = true;
                }
                break;
            }
            case 6: { // 6. DELETE
                dataManager.inputFromConsole(input1);
                if (!dataManager.getData().empty()) {
                    heap->remove(dataManager.getData()[0], timeline);
                    handled = true;
                }
                break;
            }
            case 7: { // 7. UPDATE
                dataManager.inputFromConsole(input1 + " " + input2);
                if (dataManager.getData().size() >= 2) {
                    heap->remove(dataManager.getData()[0], timeline);
                    heap->insert(dataManager.getData()[1], timeline);
                    handled = true;
                }
                break;
            }
            case 8: { // 8. CLEAR
                heap->clear(timeline);
                handled = true;
                break;
            }
        }
    }
    else if (activeStructureType == StructureType::GridGraph) {
        auto* gridGraph = dynamic_cast<GridGraph*>(activeStructure);
        if (!gridGraph) return;

        switch (action) {
            case 1: { // 1. INIT
                if (mode == 0) { // Empty (String 1 = N, String 2 = M)
                    dataManager.inputFromConsoleNonNegative(input1 + " " + input2);
                    if (!dataManager.getData().size() >= 2) {
                        gridGraph->initialize(dataManager.getData()[0], dataManager.getData()[1], timeline); 
                        handled = true;
                    }
                } else if (mode == 1) { // File
                    dataManager.inputFromFileGridGraph(input1);
                    if (!dataManager.getDataGridGraph().empty()) {
                        gridGraph->initializeFromData(dataManager.getDataGridGraph(), timeline);
                        handled = true;
                    }
                }
                break;
            }
            case 2: { // 2. RANDOM (String 1 = N, String 2 = M)
                dataManager.inputFromConsoleNonNegative(input1 + " " + input2);
                if (dataManager.getData().size() >= 2) {
                    int rows = MathUtils::getRandomInRange(4, 16);
                    int columns = MathUtils::getRandomInRange(4, 16);
                    int percentage = MathUtils::getRandomInRange(30, 40);

                    dataManager.randomDataGridGraph((size_t)rows, (size_t)columns, percentage);
                    handled = true;
                }
                break;
            }
            case 3: { // 3. SET OBSTACLES (String 1 = i, String 2 = j)
                dataManager.inputFromConsoleNonNegative(input1 + " " + input2);
                
                if (dataManager.getData().size() >= 2) { // This should let user have more options about the CellState
                    gridGraph->setCellState(dataManager.getData()[0], dataManager.getData()[1], CellState::Wall, timeline);
                    handled = true; 
                }
                break;
            }
            case 4: { // 4. BFS (x1, y1, x2, y2)
                dataManager.inputFromConsoleNonNegative(input1 + " " + input2 + " " + input3 + " " + input4);
    
                if (dataManager.getData().size() >= 4) {
                    std::pair<size_t, size_t> start = {dataManager.getData()[0], dataManager.getData()[1]};
                    std::pair<size_t, size_t> end = {dataManager.getData()[2], dataManager.getData()[3]};
                    gridGraph->runBFSShortestPath(start, end, timeline);
                    handled = true;
                }
                break;
            }
            case 5: { // 5. CLEAR
                gridGraph->clear(timeline);
                handled = true;
                break;
            }
        }
    }
    else if (activeStructureType == StructureType::AdjacencyList || activeStructureType == StructureType::AdjacencyMatrix) {
        auto* graph = dynamic_cast<IGraphStructure*>(activeStructure);
        if (!graph) return;

        switch (action) {
            case 1: { // 1. INIT
                dataManager.inputFromFileGraph(input1);
                if (!dataManager.getDataGraph().empty()) {
                    graph->initialize(dataManager.getDataGraph(), timeline);
                    handled = true;
                }
                break;
            }
            // Issue: cannot put 'dataGraph.from/input1' into addVertex function
            // case 2: { // 2. CREATE NODE
            //     dataManager.inputFromConsoleGraph(input1 + " -1 1");
            //     if (!dataManager.getDataGraph().empty()) {
            //         graph->addVertex(dataManager.getDataGraph()[0].from, timeline);
            //         handled = true;
            //     }
            //     break;
            // }
            // Issue: timeline being *
            // case 3: { // 3. CREATE EDGE
            //     dataManager.inputFromConsoleGraph(input1 + " " + input2 + " " + input3);
            //     if (!dataManager.getDataGraph().empty()) {
            //         graph->addEdge(dataManager.getDataGraph()[0].from, dataManager.getDataGraph()[0].to, dataManager.getDataGraph()[0].weight, timeline);
            //         handled = true;
            //     }
            //     break;
            // }
            case 4: { // 4. SSSP
                dataManager.inputFromConsoleNonNegative(input1);
                
                if (dataManager.getData().size() >= 1) {
                    graph->runDijkstra(dataManager.getData()[0], timeline);
                    handled = true;
                }
                break;
            }
            // Issue: cannot put 'dataGraph.to'/input2
            // case 5: { // 5. OPSP
            //     dataManager.inputFromConsoleGraph(input1 + input2 + " 1");
            //     if (mode == 0) { // DAG
            //         graph->runDAGShortestPath(dataManager.getDataGraph()[0].from, timeline);
            //         handled = true;
            //     } else if (mode == 1) { // Dijkstra
            //         graph->runDijkstra(dataManager.getDataGraph()[0].from, timeline);
            //         handled = true;
            //     } else if (mode == 2) { // BellmanFord
            //         graph->runBellmanFord(dataManager.getDataGraph()[0].from, timeline);
            //         handled = true;
            //     }
            //     break;
            // }
            case 6: { // 6. APSP
                graph->runFloydWarshall(timeline);
                handled = true;
                break;
            }
            case 7: { // 7. CLEAR
                graph->clear(timeline);
                handled = true;
                break;
            }
            case 8: { // 8. RANDOM
                int vCount = MathUtils::getRandomInRange(1, 20);
                int eCount = MathUtils::getRandomInRange(0, 190);

                dataManager.randomDataGraph(vCount, eCount, 1, 99);
                graph->initialize(dataManager.getDataGraph(), timeline);
                handled = true;
            }
        }
    }
    
    if (handled && timeline.getFrameCount() > 0) {
        playbackController.setTimeline(timeline);
        playbackController.play();
    }
    
    uiManager.resetInputAction();
}
void AppEngine::handleStructureSwitchRequest() {
    const int selectedDS = uiManager.getSelectedDS();
    if (selectedDS < 0) {
        return;
    }
    
    const StructureType targetStructure =
    mapMenuSelectionToStructureType(selectedDS);
    uiManager.resetDSSelection();
    
    if (targetStructure == StructureType::None) {
        return;
    }
    
    switchActiveStructure(targetStructure);
    uiManager.setShowMainMenu(false);
}

void AppEngine::run() {
    if (!uiManager.init(window.getWindow(), theme)) {
        std::cerr << "Fatal Error: AppEngine::run failed to initialize UIManager."
        << std::endl;
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
        if (const auto* wheelScrolled = event.getIf<sf::Event::MouseWheelScrolled>()) {
        if (!uiManager.isMouseOverUI()) {
            sf::View view = window.getWindow().getView();
            const sf::Vector2i pixelPos = wheelScrolled->position;
            const sf::Vector2f worldPosBeforeZoom = window.getWindow().mapPixelToCoords(pixelPos, view);

            const float zoomFactor = 1.1f;
            float zoom = 1.0f;
            if (wheelScrolled->delta > 0) { // Zoom in
                zoom = 1.f / zoomFactor;
            } else if (wheelScrolled->delta < 0) { // Zoom out
                zoom = zoomFactor;
            }

                // Lấy kích thước hiện tại và kích thước giới hạn
                sf::Vector2f currentSize = view.getSize();
                sf::Vector2u winSize = window.getWindow().getSize();
                
                float maxWidth = static_cast<float>(winSize.x);   // Zoom out tối đa (1.0x)
                float minWidth = maxWidth * 0.1f;                 // Zoom in tối đa (10.0x)

                // Tính toán zoom an toàn
                float expectedWidth = currentSize.x * zoom;
                if (expectedWidth > maxWidth) {
                    zoom = maxWidth / currentSize.x;
                } else if (expectedWidth < minWidth) {
                    zoom = minWidth / currentSize.x;
                }

                if (zoom != 1.0f) {
                    view.zoom(zoom);
                    window.getWindow().setView(view);

                    const sf::Vector2f worldPosAfterZoom = window.getWindow().mapPixelToCoords(pixelPos, view);
                    view.move(worldPosBeforeZoom - worldPosAfterZoom);
                    
                    // Giữ View không bị trôi ra ngoài màn hình quá xa
                    clampView(view, winSize);
                    window.getWindow().setView(view);
                }
        }
    }

    if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mousePressed->button == sf::Mouse::Button::Left && !uiManager.isMouseOverUI()) {
            sf::Vector2f worldPos = window.getWindow().mapPixelToCoords(mousePressed->position);
            if (!renderer.handleMousePress(worldPos)) {
                isPanning = true;
                lastPanMousePos = mousePressed->position;
                    prePanCenter = window.getWindow().getView().getCenter();
            }
        }
    } else if (const auto* mouseMoved = event.getIf<sf::Event::MouseMoved>()) {
        if (isPanning) {
            sf::View view = window.getWindow().getView();
            sf::Vector2f oldPos = window.getWindow().mapPixelToCoords(lastPanMousePos, view);
            sf::Vector2f newPos = window.getWindow().mapPixelToCoords(mouseMoved->position, view);
            view.move(oldPos - newPos);
            
            clampView(view, window.getWindow().getSize());
            window.getWindow().setView(view);
            lastPanMousePos = mouseMoved->position;
        } else {
            sf::Vector2f worldPos = window.getWindow().mapPixelToCoords(mouseMoved->position);
            renderer.handleMouseMove(worldPos);
        }
    } else if (const auto* mouseReleased = event.getIf<sf::Event::MouseButtonReleased>()) {
        if (mouseReleased->button == sf::Mouse::Button::Left) {
            if (isPanning) {
                isPanning = false;
                    sf::Vector2f currentCenter = window.getWindow().getView().getCenter();
                    if (currentCenter != prePanCenter) {
                        viewCenterHistory.push_back(prePanCenter);
                        undoHistory.push_back(UndoAction::CameraPan);
                    }
            } else {
                    if (renderer.handleMouseRelease()) {
                        undoHistory.push_back(UndoAction::NodeDrag);
                    }
            }
        }
    } else if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->code == sf::Keyboard::Key::Z && 
           (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RControl))) {
            if (!uiManager.isKeyboardCapturedByUI()) {
                    if (!undoHistory.empty()) {
                        UndoAction action = undoHistory.back();
                        undoHistory.pop_back();
                        if (action == UndoAction::NodeDrag) {
                            renderer.undoLastDrag();
                        } else if (action == UndoAction::CameraPan) {
                            sf::Vector2f oldCenter = viewCenterHistory.back();
                            viewCenterHistory.pop_back();
                            sf::View view = window.getWindow().getView();
                            view.setCenter(oldCenter);
                            window.getWindow().setView(view);
                        }
                    }
            }
        }
    }
}
                
void AppEngine::update(sf::Time deltaTime) {
    appTime += deltaTime.asSeconds();

    uiManager.update(window.getWindow(), deltaTime);
    handleStructureSwitchRequest();
    handleDataActionRequest();
    
    if (uiManager.checkBackToMenuClicked()) {
        uiManager.reset();
        switchActiveStructure(StructureType::None);
    }

    if (uiManager.consumeThemeScaleRequest()) {
        theme.nodeScale = uiManager.getTheme().nodeScale;
        theme.arrayScale = uiManager.getTheme().arrayScale;
    }

    if (uiManager.consumeThemeToggleRequest()) {
        isDarkMode = !isDarkMode;
        float currentNodeScale = theme.nodeScale;
        float currentArrayScale = theme.arrayScale;
        theme = isDarkMode ? Theme::getDarkTheme() : Theme::getDefaultTheme();
        theme.nodeScale = currentNodeScale;
        theme.arrayScale = currentArrayScale;
        if (!uiManager.applyTheme(theme)) {
            std::cerr
            << "Warning: AppEngine::update failed to apply theme to UIManager."
            << std::endl;
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

    uiManager.syncPlaybackUI(
        playbackController.isPlayingState(),
        playbackController.getTimeline().getCurrentFrameIndex() == 0,
        playbackController.getTimeline().isAtEnd(),
        playbackController.getTimeline().getFrameCount() == 0
    );
}

void AppEngine::render() {
    window.clear(theme.windowClearColor);
    renderer.drawBackground();
    
    const Timeline& timeline = playbackController.getTimeline();
    const Frame* currentFrame = timeline.getCurrentFrame();
    if (currentFrame) {
        renderer.renderActiveState(currentFrame);
    }

    if (uiManager.isShowingMainMenu()) {
        // Vẽ Background trang trí 4 góc bằng các cấu trúc dữ liệu (Chỉ hiển thị ở Main Menu)
        DecorationPayload decPayload(appTime, sf::Vector2f(window.getWindow().getSize()));
        renderer.visit(decPayload);

        MenuAnimPayload menuPayload(appTime, uiManager.getNavMenuState(), sf::Vector2f(window.getWindow().getSize()));
        renderer.visit(menuPayload);
    }

    sf::View currentView = window.getWindow().getView();
    window.getWindow().setView(window.getWindow().getDefaultView());

    // 3. Draw the ImGui UI on top
    uiManager.render(window.getWindow());
    window.getWindow().setView(currentView);

    // 4. Push to monitor
    window.display(); 
}
