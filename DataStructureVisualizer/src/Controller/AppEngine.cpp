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

bool isInitialState = true;

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
    if (!currentFrame || isInitialState) {
        uiManager.setCodePanelHighlightedLine(-1);
        uiManager.setCodePanelMessage("");
        return;
    }
    
    uiManager.setCodePanelHighlightedLine(currentFrame->getCodeLineId());
    uiManager.setCodePanelMessage(currentFrame->getMessage());
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
    sf::Vector2u winSize = window.getWindow().getSize();
    LayoutConfig layoutConfig{};
    layoutConfig.screenWidth = static_cast<float>(winSize.x);
    layoutConfig.screenHeight = static_cast<float>(winSize.y);
    float minDimension = std::min(layoutConfig.screenWidth, layoutConfig.screenHeight);
    layoutConfig.padding = std::clamp(
        minDimension * layoutConfig.layoutPaddingRatio,
        layoutConfig.layoutPaddingMin,
        layoutConfig.layoutPaddingMax);

    switch (structureType) {
        case StructureType::SinglyLinkedList:
        return new SinglyLinkedList(layoutConfig);
        case StructureType::MinHeap:
        return new MinHeap(layoutConfig);
        case StructureType::MaxHeap:
        return new MaxHeap(layoutConfig);
        case StructureType::AVLTree:
        return new AVLTree(layoutConfig);
        case StructureType::AdjacencyList:
        return new AdjacencyList(layoutConfig);
        case StructureType::AdjacencyMatrix:
        return new AdjacencyMatrix(layoutConfig);
        case StructureType::GridGraph: {
        const int defaultRowsInt = std::clamp(
            static_cast<int>(layoutConfig.screenHeight / layoutConfig.gridTargetCellPixels),
            layoutConfig.gridDefaultRowsMin,
            layoutConfig.gridDefaultRowsMax);
        const float aspect = (winSize.y == 0) ? 1.0f : static_cast<float>(winSize.x) / static_cast<float>(winSize.y);
        const int defaultColsInt = std::clamp(
            static_cast<int>(defaultRowsInt * aspect),
            layoutConfig.gridDefaultColsMin,
            layoutConfig.gridDefaultColsMax);
        return new GridGraph(static_cast<size_t>(defaultRowsInt), static_cast<size_t>(defaultColsInt));
        }
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
    renderer.resetAnimations();
    clearUndoHistory();

    isInitialState = true;

    Timeline timeline;
    if (activeStructure) {
        activeStructure->clear(timeline);
        timeline.goToLastFrame();
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
        uiManager.resetInputAction();
        return;
    }
    
    if (!activeStructure) {
        uiManager.resetInputAction();
        return;
    }
    
    const int mode = uiManager.getInputMode();
    int selectedDS = uiManager.getSelectedDS();
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
    
    switch (selectedDS) {
        case 0: { // SINGLY LINKED LIST
            auto* sll = dynamic_cast<IStandardStructure*>(activeStructure);
            if (!sll) {
                uiManager.resetInputAction();
                return;
            }
            
            switch (action) {
                case 1: { // 1. INIT
                    if (mode == 0) { // Array
                        dataManager.inputFromConsole(input1);
                        if (!dataManager.getData().empty()) {
                            sll->initialize(dataManager.getData(), timeline);
                            handled = true;
                        }
                    } else if (mode == 1) { // Random
                        int size = MathUtils::getRandomInRange(3, 15);
                        dataManager.randomData(size, -99, 99);
                        if (!dataManager.getData().empty()) {
                            sll->initialize(dataManager.getData(), timeline);
                            handled = true;
                        }
                    } else if (mode == 2) { // File
                        dataManager.inputFromFile(input1);
                        if (!dataManager.getData().empty()) {
                            sll->initialize(dataManager.getData(), timeline);
                            handled = true;
                        }
                    }
                    break;
                }
                case 2: { // 2. INSERT
                    if (mode == 0) { // Single
                        dataManager.inputFromConsole(input1);
                        if (!dataManager.getData().empty()) {
                            sll->insert(dataManager.getData()[0], timeline);
                            handled = true;
                        }
                    } else if (mode == 1) { // Random
                        dataManager.randomData(1, -99, 99);
                        if (!dataManager.getData().empty()) {
                            sll->insert(dataManager.getData()[0], timeline);
                            handled = true;
                        }
                    }
                    break;
                }
                case 3: { // 3. SEARCH
                    dataManager.inputFromConsole(input1);
                    if (!dataManager.getData().empty()) {
                        sll->search(dataManager.getData()[0], timeline);
                        handled = true;
                    }
                    break;
                }
                case 4: { // 4. DELETE
                    dataManager.inputFromConsole(input1);
                    if (!dataManager.getData().empty()) {
                        sll->remove(dataManager.getData()[0], timeline);
                        handled = true;
                    }
                    break;
                }
                case 5: { // 5. UPDATE
                    dataManager.inputFromConsole(input1 + " " + input2);
                    if (dataManager.getData().size() >= 2) {
                        sll->update(dataManager.getData()[0], dataManager.getData()[1], timeline);
                        handled = true;
                    }
                    break;
                }
                case 6: { // 6. CLEAR
                    sll->clear(timeline);
                    handled = true;
                    break;
                }
            }
            break;
        }
        case 1: { // MIN HEAP
            auto* minheap = dynamic_cast<IHeapStructure*>(activeStructure);
            if (!minheap) {
                uiManager.resetInputAction();
                return;
            }

            switch (action) {
                case 1: { // 1. INIT
                    if (mode == 0) { // Array
                        dataManager.inputFromConsole(input1);
                        if (!dataManager.getData().empty()) {
                            minheap->initialize(dataManager.getData(), timeline);
                            handled = true;
                        }
                    } else if (mode == 1) { // Random
                        int size = MathUtils::getRandomInRange(3, 15);
                        dataManager.randomData(size, -99, 99);
                        if (!dataManager.getData().empty()) {
                            minheap->initialize(dataManager.getData(), timeline);
                            handled = true;
                        }
                    } else if (mode == 2) { // File
                        dataManager.inputFromFile(input1);
                        if (!dataManager.getData().empty()) {
                            minheap->initialize(dataManager.getData(), timeline);
                            handled = true;
                        }
                    }
                    break;
                }
                case 2: { // 2. INSERT
                    if (mode == 0) { // Single
                        dataManager.inputFromConsole(input1);
                        if (!dataManager.getData().empty()) {
                            minheap->insert(dataManager.getData()[0], timeline);
                            handled = true;
                        }
                    } else if (mode == 1) { // Random
                        dataManager.randomData(1, -99, 99);
                        if (!dataManager.getData().empty()) {
                            minheap->insert(dataManager.getData()[0], timeline);
                            handled = true;
                        }
                    }
                    break;
                }
                case 3: { // 3. EXTRACTTOP
                    minheap->extractTop(timeline);
                    handled = true;
                    break;
                }
                case 4: { // 4. PEEK
                    minheap->peek(timeline);
                    handled = true;
                    break;
                }
                case 5: { // 5. SEARCH
                    dataManager.inputFromConsole(input1);
                    if (!dataManager.getData().empty()) {
                        minheap->search(dataManager.getData()[0], timeline);
                        handled = true;
                    }
                    break;
                }
                case 6: { // 6. DELETE
                    dataManager.inputFromConsole(input1);
                    if (!dataManager.getData().empty()) {
                        minheap->remove(dataManager.getData()[0], timeline);
                        handled = true;
                    }
                    break;
                }
                case 7: { // 7. UPDATE
                    dataManager.inputFromConsole(input1 + " " + input2);
                    if (dataManager.getData().size() >= 2) {
                        minheap->update(dataManager.getData()[0], dataManager.getData()[1], timeline);
                        handled = true;
                    }
                    break;
                }
                case 8: { // 8. CLEAR
                    minheap->clear(timeline);
                    handled = true;
                    break;
                }
            }
            break;
        }
        case 2: { // MAX HEAP
            auto* maxheap = dynamic_cast<IHeapStructure*>(activeStructure);
            if (!maxheap) {
                uiManager.resetInputAction();
                return;
            }

            switch (action) {
                case 1: { // 1. INIT
                    if (mode == 0) { // Array
                        dataManager.inputFromConsole(input1);
                        if (!dataManager.getData().empty()) {
                            maxheap->initialize(dataManager.getData(), timeline);
                            handled = true;
                        }
                    } else if (mode == 1) { // Random
                        int size = MathUtils::getRandomInRange(3, 15);
                        dataManager.randomData(size, -99, 99);
                        if (!dataManager.getData().empty()) {
                            maxheap->initialize(dataManager.getData(), timeline);
                            handled = true;
                        }
                    } else if (mode == 2) { // File
                        dataManager.inputFromFile(input1);
                        if (!dataManager.getData().empty()) {
                            maxheap->initialize(dataManager.getData(), timeline);
                            handled = true;
                        }
                    }
                    break;
                }
                case 2: { // 2. INSERT
                    if (mode == 0) { // Single
                        dataManager.inputFromConsole(input1);
                        if (!dataManager.getData().empty()) {
                            maxheap->insert(dataManager.getData()[0], timeline);
                            handled = true;
                        }
                    } else if (mode == 1) { // Random
                        dataManager.randomData(1, -99, 99);
                        if (!dataManager.getData().empty()) {
                            maxheap->insert(dataManager.getData()[0], timeline);
                            handled = true;
                        }
                    }
                    break;
                }
                case 3: { // 3. EXTRACTTOP
                    maxheap->extractTop(timeline);
                    handled = true;
                    break;
                }
                case 4: { // 4. PEEK
                    maxheap->peek(timeline);
                    handled = true;
                    break;
                }
                case 5: { // 5. SEARCH
                    dataManager.inputFromConsole(input1);
                    if (!dataManager.getData().empty()) {
                        maxheap->search(dataManager.getData()[0], timeline);
                        handled = true;
                    }
                    break;
                }
                case 6: { // 6. DELETE
                    dataManager.inputFromConsole(input1);
                    if (!dataManager.getData().empty()) {
                        maxheap->remove(dataManager.getData()[0], timeline);
                        handled = true;
                    }
                    break;
                }
                case 7: { // 7. UPDATE
                    dataManager.inputFromConsole(input1 + " " + input2);
                    if (dataManager.getData().size() >= 2) {
                        maxheap->update(dataManager.getData()[0], dataManager.getData()[1], timeline);
                        handled = true;
                    }
                    break;
                }
                case 8: { // 8. CLEAR
                    maxheap->clear(timeline);
                    handled = true;
                    break;
                }
            }
            break;
        }
        case 3: { // AVL TREE
            auto* avl = dynamic_cast<IStandardStructure*>(activeStructure);
            if (!avl) {
                uiManager.resetInputAction();
                return;
            }

            switch (action) {
                case 1: { // 1. INIT
                    if (mode == 0) { // Array
                        dataManager.inputFromConsole(input1);
                        if (!dataManager.getData().empty()) {
                            avl->initialize(dataManager.getData(), timeline);
                            handled = true;
                        }
                    } else if (mode == 1) { // Random
                        int size = MathUtils::getRandomInRange(3, 15);
                        dataManager.randomData(size, -99, 99);
                        if (!dataManager.getData().empty()) {
                            avl->initialize(dataManager.getData(), timeline);
                            handled = true;
                        }
                    } else if (mode == 2) { // File
                        dataManager.inputFromFile(input1);
                        if (!dataManager.getData().empty()) {
                            avl->initialize(dataManager.getData(), timeline);
                            handled = true;
                        }
                    }
                    break;
                }
                case 2: { // 2. INSERT
                    if (mode == 0) { // Single
                        dataManager.inputFromConsole(input1);
                        if (!dataManager.getData().empty()) {
                            avl->insert(dataManager.getData()[0], timeline);
                            handled = true;
                        }
                    } else if (mode == 1) { // Random
                        dataManager.randomData(1, -99, 99);
                        if (!dataManager.getData().empty()) {
                            avl->insert(dataManager.getData()[0], timeline);
                            handled = true;
                        }
                    }
                    break;
                }
                case 3: { // 3. SEARCH
                    dataManager.inputFromConsole(input1);
                    if (!dataManager.getData().empty()) {
                        avl->search(dataManager.getData()[0], timeline);
                        handled = true;
                    }
                    break;
                }
                case 4: { // 4. DELETE
                    dataManager.inputFromConsole(input1);
                    if (!dataManager.getData().empty()) {
                        avl->remove(dataManager.getData()[0], timeline);
                        handled = true;
                    }
                    break;
                }
                case 5: { // 5. UPDATE
                    dataManager.inputFromConsole(input1 + " " + input2);
                    if (dataManager.getData().size() >= 2) {
                        avl->update(dataManager.getData()[0], dataManager.getData()[1], timeline);
                        handled = true;
                    }
                    break;
                }
                case 6: { // 6. CLEAR
                    avl->clear(timeline);
                    handled = true;
                    break;
                }
            }
            break;
        }
        case 4: { // GRID GRAPH
            auto* gridGraph = dynamic_cast<GridGraph*>(activeStructure);
            if (!gridGraph) {
                uiManager.resetInputAction();
                return;
            }

            switch (action) {
                case 1: { // 1. INIT
                    if (mode == 0) { // Empty (String 1 = N, String 2 = M)
                        dataManager.inputFromConsoleNonNegative(input1 + " " + input2);
                        if (dataManager.getData().size() >= 2) {
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
                        int percentage = MathUtils::getRandomInRange(30, 40);

                        dataManager.randomDataGridGraph(dataManager.getData()[0], dataManager.getData()[1], percentage);
                        gridGraph->initializeFromData(dataManager.getDataGridGraph(), timeline);
                        handled = true;
                    }
                    break;
                }
                case 3: { // 3. SET OBSTACLES (String 1 = i, String 2 = j)
                    dataManager.inputFromConsoleNonNegative(input1 + " " + input2);
                    if (dataManager.getData().size() >= 2) { // This should let user have more options about the CellState
                        if (mode == 0) {
                            gridGraph->setCellState(dataManager.getData()[0], dataManager.getData()[1], CellState::Empty, timeline);
                        } else if (mode == 1) {
                            gridGraph->setCellState(dataManager.getData()[0], dataManager.getData()[1], CellState::Wall, timeline);
                        }
                        handled = true; 
                    }
                    break;
                }
                case 4: { // 4. BFS (x1, y1, x2, y2)
                    dataManager.inputFromConsoleNonNegative(input1 + " " + input2 + " " + input3 + " " + input4);
        
                    if (dataManager.getData().size() >= 4) {
                        std::pair<size_t, size_t> start = {dataManager.getData()[0], dataManager.getData()[1]};
                        std::pair<size_t, size_t> end = {dataManager.getData()[2], dataManager.getData()[3]};
                        if (mode == 0) {
                            gridGraph->runAStar(start, end, timeline);
                        } else if (mode == 1) {
                            gridGraph->runBFSShortestPath(start, end, timeline);
                        }
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
            break;
        }
        case 5: { // DIRECTED GRAPH
            auto* directedgraph = dynamic_cast<IGraphStructure*>(activeStructure);
            if (!directedgraph) {
                uiManager.resetInputAction();
                return;
            }

            switch (action) {
                case 1: { // 1. INIT
                    dataManager.inputFromFileGraph(input1);
                    if (!dataManager.getDataGraph().empty()) {
                        directedgraph->initialize(dataManager.getDataGraph(), timeline);
                        handled = true;
                    }
                    break;
                }
                case 2: { // 2. CREATE NODE
                    directedgraph->addVertex(&timeline);
                    handled = true;
                    break;
                }
                case 3: { // 3. CREATE EDGE
                    dataManager.inputFromConsoleGraph(input1 + " " + input2 + " " + input3);
                    if (!dataManager.getDataGraph().empty()) {
                        Edge addEdge = dataManager.getDataGraph()[0];
                        directedgraph->addEdge(addEdge.from, addEdge.to, addEdge.weight, &timeline);
                        handled = true;
                    }
                    break;
                }
                case 4: { // 4. OPSP
                    dataManager.inputFromConsoleNonNegative(input1);
                    
                    if (dataManager.getData().size() >= 1) {
                        directedgraph->runAStar(dataManager.getData()[0], dataManager.getData()[1], timeline);
                        handled = true;
                    }
                    break;
                }
                case 5: { // 5. SPSP
                    dataManager.inputFromConsoleNonNegative(input1);
                    if (!dataManager.getData().empty()) {
                        if (mode == 0) { // DAG
                            directedgraph->runDAGShortestPath(dataManager.getDataGraph()[0].from, timeline);
                            handled = true;
                        } else if (mode == 1) { // Dijkstra
                            directedgraph->runDijkstra(dataManager.getDataGraph()[0].from, timeline);
                            handled = true;
                        } else if (mode == 2) { // BellmanFord
                            directedgraph->runBellmanFord(dataManager.getDataGraph()[0].from, timeline);
                            handled = true;
                        }
                    }
                    break;
                }
                case 6: { // 6. APSP
                    if (mode == 0) {
                        directedgraph->runFloydWarshall(timeline);
                        handled = true;
                    } else if (mode == 1) {
                        directedgraph->runJohnson(timeline);
                        handled = true;
                    }
                    break;
                }
                case 7: { // 7. CLEAR
                    directedgraph->clear(timeline);
                    handled = true;
                    break;
                }
                case 8: { // 8. RANDOM
                    dataManager.inputFromConsole(input1 + " " + input2);
                    if (dataManager.getData().size() >= 2) {
                        int vCount = MathUtils::getRandomInRange(3, 20);
                        
                        int minWeight = dataManager.getData()[0];
                        int maxWeight = dataManager.getData()[1];

                        if (mode == 0) {
                            int eCount = MathUtils::getRandomInRange(0, 190);
                            dataManager.randomDataDAG(vCount, eCount, minWeight, maxWeight);
                        }
                        else if (mode == 1) {
                            sf::Vector2u winSize = window.getWindow().getSize();
                            // dataManager.randomDataGraph(vCount, eCount, minWeight, maxWeight);
                            dataManager.randomDataPlanarGraph(vCount, minWeight, maxWeight, (float)winSize.x, (float)winSize.y);
                        }

                        if (!dataManager.getDataGraph().empty()) {
                            directedgraph->initialize(dataManager.getDataGraph(), timeline);
                            handled = true;
                        }
                    }
                    break;
                }
            }
            break;
        }
        case 6: { // UNDIRECTED GRAPH
            auto* undirectedgraph = dynamic_cast<IGraphStructure*>(activeStructure);
            if (!undirectedgraph) {
                uiManager.resetInputAction();
                return;
            }

            switch (action) {
                case 1: { // 1. INIT
                    dataManager.inputFromFileGraph(input1);
                    if (!dataManager.getDataGraph().empty()) {
                        undirectedgraph->initialize(dataManager.getDataGraph(), timeline);
                        handled = true;
                    }
                    break;
                }
                case 2: { // 2. CREATE NODE
                    undirectedgraph->addVertex(&timeline);
                    handled = true;
                    break;
                }
                case 3: { // 3. CREATE EDGE
                    dataManager.inputFromConsoleGraph(input1 + " " + input2 + " " + input3);
                    if (!dataManager.getDataGraph().empty()) {
                        Edge addEdge = dataManager.getDataGraph()[0];
                        undirectedgraph->addEdge(addEdge.from, addEdge.to, addEdge.weight, &timeline);
                        handled = true;
                    }
                    break;
                }
                case 4: { // 4. OPSP
                    dataManager.inputFromConsoleNonNegative(input1);
                    
                    if (dataManager.getData().size() >= 1) {
                        undirectedgraph->runAStar(dataManager.getData()[0], dataManager.getData()[1], timeline);
                        handled = true;
                    }
                    break;
                }
                case 5: { // 5. SPSP
                    dataManager.inputFromConsoleNonNegative(input1);
                    if (!dataManager.getData().empty()) {
                        if (mode == 0) { // Dijkstra
                            undirectedgraph->runDijkstra(dataManager.getDataGraph()[0].from, timeline);
                            handled = true;
                        } else if (mode == 1) { // BellmanFord
                            undirectedgraph->runBellmanFord(dataManager.getDataGraph()[0].from, timeline);
                            handled = true;
                        }
                    }
                    break;
                }
                case 6: { // 6. APSP
                    if (mode == 0) {
                        undirectedgraph->runFloydWarshall(timeline);
                        handled = true;
                    } else if (mode == 1) {
                        undirectedgraph->runJohnson(timeline);
                        handled = true;
                    }
                    break;
                }
                case 7: { // 7. CLEAR
                    undirectedgraph->clear(timeline);
                    handled = true;
                    break;
                }
                case 8: { // 8. RANDOM
                    dataManager.inputFromConsole(input1 + " " + input2);
                    if (dataManager.getData().size() >= 2) {
                        int vCount = MathUtils::getRandomInRange(3, 20);
                        
                        int minWeight = dataManager.getData()[0];
                        int maxWeight = dataManager.getData()[1];

                        sf::Vector2u winSize = window.getWindow().getSize();
                        // dataManager.randomDataGraph(vCount, eCount, minWeight, maxWeight);
                        dataManager.randomDataPlanarGraph(vCount, minWeight, maxWeight, (float)winSize.x, (float)winSize.y);
                        
                        if (!dataManager.getDataGraph().empty()) {
                            undirectedgraph->initialize(dataManager.getDataGraph(), timeline);
                            handled = true;
                        }
                    }
                    break;
                }
            }
            break;
        }
    }

    if (handled && timeline.getFrameCount() > 0) {
        isInitialState = false;
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

        if (auto* graph = dynamic_cast<IGraphStructure*>(activeStructure)) {
            LayoutConfig resizedConfig = graph->getLayoutConfig();
            resizedConfig.screenWidth = static_cast<float>(resized->size.x);
            resizedConfig.screenHeight = static_cast<float>(resized->size.y);
            float minDimension = std::min(resizedConfig.screenWidth, resizedConfig.screenHeight);
            resizedConfig.padding = std::clamp(
                minDimension * resizedConfig.layoutPaddingRatio,
                resizedConfig.layoutPaddingMin,
                resizedConfig.layoutPaddingMax);
            graph->setLayoutConfig(resizedConfig);
        }
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
        renderer.updateAnimations(deltaTime.asSeconds());


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
        renderer.reloadBackground();
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

        MenuAnimPayload menuPayload(
            appTime, 
            uiManager.getNavMenuState(), 
            isDarkMode, 
            sf::Vector2f(window.getWindow().getSize()), 
            uiManager.getNavMenuNames(), 
            uiManager.getNavMenuTitle()
        );
        renderer.visit(menuPayload);
    } else {
        TopBarPayload tbPayload(isDarkMode, uiManager.isShowingCode(), sf::Vector2f(window.getWindow().getSize()));
        renderer.visit(tbPayload);
    }

    sf::View currentView = window.getWindow().getView();
    window.getWindow().setView(window.getWindow().getDefaultView());

    // 3. Draw the ImGui UI on top
    uiManager.render(window.getWindow());
    window.getWindow().setView(currentView);

    // 4. Push to monitor
    window.display(); 
}
