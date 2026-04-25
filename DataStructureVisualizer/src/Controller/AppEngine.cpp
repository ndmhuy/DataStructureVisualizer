#include "Controller/AppEngine.h"

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

namespace {
bool tryParseInt(const std::string& text, int& outValue) {
    if (text.empty()) {
        return false;
    }
    
    std::stringstream ss(text);
    int value = 0;
    ss >> value;
    if (ss.fail()) {
        return false;
    }
    
    // Reject trailing non-space content.
    std::string remaining;
    ss >> remaining;
    if (!remaining.empty()) {
        return false;
    }
    
    outValue = value;
    return true;
}
    
std::vector<int> parseIntList(const std::string& text) {
    std::vector<int> values;
    std::stringstream ss(text);
    int value = 0;
    
    while (ss >> value) {
        values.push_back(value);
    }
    
    return values;
}
    
std::vector<int> readIntListFromFile(const std::string& path) {
    std::vector<int> values;
    std::ifstream input(path);
    if (!input.is_open()) {
        return values;
    }
    
    int value = 0;
    while (input >> value) {
        values.push_back(value);
    }
    
    return values;
}
    
std::vector<Edge> parseEdgesFromInts(const std::vector<int>& values) {
    std::vector<Edge> edges;
    
    // Accept triplets (from,to,weight) first, then fallback to pairs with
    // weight=1.
    if (values.size() >= 3 && values.size() % 3 == 0) {
        for (size_t i = 0; i < values.size(); i += 3) {
            if (values[i] < 0 || values[i + 1] < 0) {
                continue;
            }
            edges.emplace_back(static_cast<size_t>(values[i]),
            static_cast<size_t>(values[i + 1]), values[i + 2]);
        }
        return edges;
    }
    
    if (values.size() >= 2 && values.size() % 2 == 0) {
        for (size_t i = 0; i < values.size(); i += 2) {
            if (values[i] < 0 || values[i + 1] < 0) {
                continue;
            }
            edges.emplace_back(static_cast<size_t>(values[i]),
            static_cast<size_t>(values[i + 1]), 1);
        }
    }
    
    return edges;
}
    
std::vector<int> generateRandomValues(int count, int minValue, int maxValue) {
    std::vector<int> values;
    values.reserve(static_cast<size_t>(count));
    
    static thread_local std::mt19937 engine(std::random_device{}());
    std::uniform_int_distribution<int> dist(minValue, maxValue);
    for (int i = 0; i < count; ++i) {
        values.push_back(dist(engine));
    }
    
    return values;
}
    
std::vector<Edge> generateRandomEdges(int vertexCount, int edgeCount, int minWeight, int maxWeight) {
    std::vector<Edge> edges;
    edges.reserve(static_cast<size_t>(edgeCount));
    
    static thread_local std::mt19937 engine(std::random_device{}());
    std::uniform_int_distribution<int> vertexDist(0, vertexCount - 1);
    std::uniform_int_distribution<int> weightDist(minWeight, maxWeight);
    
    for (int i = 0; i < edgeCount; ++i) {
        int from = vertexDist(engine);
        int to = vertexDist(engine);
        int weight = weightDist(engine);
        edges.emplace_back(static_cast<size_t>(from), static_cast<size_t>(to),
        weight);
    }
    
    return edges;
}
        
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
            
void syncCodePanelWithCurrentFrame(UIManager &uiManager,
const Timeline &timeline) {
    const Frame *currentFrame = timeline.getCurrentFrame();
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
        return StructureType::AVLTree;
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

IVisualizable *AppEngine::resolveStructure(StructureType structureType) {
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
    
    auto *standard = dynamic_cast<IStandardStructure *>(activeStructure);
    auto *heap = dynamic_cast<IHeapStructure *>(activeStructure);
    auto *graph = dynamic_cast<IGraphStructure *>(activeStructure);
    auto *gridGraph = dynamic_cast<GridGraph *>(activeStructure);
    
    switch (action) {
        case 1: { // Insert
            if (standard) {
                if (mode == 0) {
                    int value = 0;
                    if (tryParseInt(input1, value)) {
                        standard->insert(value, timeline);
                        handled = true;
                    }
                } else if (mode == 1 || mode == 2) {
                    std::vector<int> values =
                    (mode == 1) ? parseIntList(input1) : readIntListFromFile(input1);
                    if (!values.empty()) {
                        standard->initialize(values, timeline);
                        handled = true;
                    }
                }
            } else if (heap) {
                if (mode == 0) {
                    int value = 0;
                    if (tryParseInt(input1, value)) {
                        heap->insert(value, timeline);
                        handled = true;
                    }
                } else if (mode == 1 || mode == 2) {
                    std::vector<int> values =
                    (mode == 1) ? parseIntList(input1) : readIntListFromFile(input1);
                    if (!values.empty()) {
                        heap->initialize(values, timeline);
                        handled = true;
                    }
                }
            } else if (graph) {
                if (mode == 1 || mode == 2) {
                    std::vector<int> rawValues =
                    (mode == 1) ? parseIntList(input1) : readIntListFromFile(input1);
                    std::vector<Edge> edges = parseEdgesFromInts(rawValues);
                    if (!edges.empty()) {
                        graph->initialize(edges, timeline);
                        handled = true;
                    }
                }
            }
            break;
        }
        case 2: { // Delete
            if (standard) {
                int value = 0;
                if (tryParseInt(input1, value)) {
                    standard->remove(value, timeline);
                    handled = true;
                }
            } else if (heap) {
                heap->extractTop(timeline);
                handled = true;
            }
            break;
        }
        case 3: { // Search
            if (standard) {
                int value = 0;
                if (tryParseInt(input1, value)) {
                    standard->search(value, timeline);
                    handled = true;
                }
            } else if (heap) {
                heap->peek(timeline);
                handled = true;
            }
            break;
        }
        case 4: { // Update
            if (standard) {
                int fromValue = 0;
                int toValue = 0;
                if (tryParseInt(input1, fromValue) && tryParseInt(input2, toValue)) {
                    standard->update(fromValue, toValue, timeline);
                    handled = true;
                }
            }
            break;
        }
        case 5: { // Random
            if (standard || heap) {
                if (mode == 0) {
                    std::vector<int> oneValue = generateRandomValues(1, 1, 99);
                    if (standard) {
                        standard->insert(oneValue[0], timeline);
                    } else {
                        heap->insert(oneValue[0], timeline);
                    }
                    handled = true;
                } else if (mode == 1) {
                    std::vector<int> values = generateRandomValues(10, 1, 99);
                    if (standard) {
                        standard->initialize(values, timeline);
                    } else {
                        heap->initialize(values, timeline);
                    }
                    handled = true;
                }
            } else if (graph && mode == 1) {
                std::vector<Edge> edges = generateRandomEdges(8, 12, 1, 20);
                graph->initialize(edges, timeline);
                handled = true;
            }
            break;
        }
        case 6: { // BFS
            if (gridGraph) {
                gridGraph->runBFSShortestPath({0, 0}, {9, 9}, timeline);
                handled = true;
            }
            break;
        }
        case 7: { // Grid AStar
            if (gridGraph) {
                gridGraph->runAStar({0, 0}, {9, 9}, timeline);
                handled = true;
            }
            break;
        }
        case 8: { // DAG
            if (graph) {
                graph->runDAGShortestPath(0, timeline);
                handled = true;
            }
            break;
        }
        case 9: { // Dijkstra
            if (graph) {
                graph->runDijkstra(0, timeline);
                handled = true;
            }
            break;
        }
        case 10: { // AStar Graph
            if (graph) {
                graph->runAStar(0, graph->size() > 1 ? graph->size() - 1 : 0, timeline);
                handled = true;
            }
            break;
        }
        case 11: { // Bellman-Ford
            if (graph) {
                graph->runBellmanFord(0, timeline);
                handled = true;
            }
            break;
        }
        case 12: { // Floyd-Warshall
            if (graph) {
                graph->runFloydWarshall(timeline);
                handled = true;
            }
            break;
        }
        case 13: { // Johnson
            if (graph) {
                graph->runJohnson(timeline);
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
    
void AppEngine::processInput(const sf::Event &event) {
    if (event.is<sf::Event::Closed>()) {
        shouldClose = true;
        return;
    }
        
    if (const auto *resized = event.getIf<sf::Event::Resized>()) {
        sf::FloatRect visibleArea({0.f, 0.f}, {(float)resized->size.x, (float)resized->size.y});
        window.getWindow().setView(sf::View(visibleArea));
    }
        
    // Pass the event to Dear ImGui and your custom buttons
    uiManager.processEvent(window.getWindow(), event);
    if (const auto *mousePressed = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mousePressed->button == sf::Mouse::Button::Left && !uiManager.isMouseOverUI()) {
            sf::Vector2f worldPos =
            window.getWindow().mapPixelToCoords(mousePressed->position);
            renderer.handleMousePress(worldPos);
        }
    } else if (const auto *mouseMoved = event.getIf<sf::Event::MouseMoved>()) {
        sf::Vector2f worldPos =
        window.getWindow().mapPixelToCoords(mouseMoved->position);
        renderer.handleMouseMove(worldPos);
    } else if (const auto *mouseReleased = event.getIf<sf::Event::MouseButtonReleased>()) {
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
}

void AppEngine::render() {
    window.clear(theme.windowClearColor);
    renderer.drawBackground();
    
    const Timeline &timeline = playbackController.getTimeline();
    const Frame *currentFrame = timeline.getCurrentFrame();
    if (currentFrame) {
        renderer.renderActiveState(currentFrame);
    }
    
    uiManager.render(window.getWindow());
    
    window.display();
}
