#include <iostream>
#include <sstream>
#include <algorithm>
#include <queue>
#include <limits>

#include "Controller/DataManager.h"
#include "Utilities/MathUtils.h"

// File input
void DataManager::inputFromFile(const std::string& filePath) {
    std::ifstream fileIn(filePath);

    if (!fileIn.is_open()) {
        std::cerr << "Error: DataManager::inputFromFile cannot open input file '"
                  << filePath << "'." << std::endl;
        return;
    }

    std::string line;
    data.clear();
    bool hasAnyValid = false;

    while (std::getline(fileIn, line)) {
        if (line.empty() || (line.find_first_not_of(" \t\n\v\f\r") == std::string::npos)) {
            continue;
        }

        std::stringstream ss(line);
        int value;
        bool lineHasValid = false;

        while (true) {
            if (ss >> value) {
                data.push_back(value);
                hasAnyValid = true;
                lineHasValid = true;
            } else if (ss.eof()) {
                break;
            } else {
                // Clear fail state and skip invalid token
                ss.clear();
                ss.ignore(1);
            }
        }

        if (!lineHasValid) {
            std::cout << "Invalid data input text file! No valid integers found on current line; line ignored." << std::endl;
        }
    }

    fileIn.close();

    if (!hasAnyValid) {
        std::cout << "There is not any valid integer in this file!" << std::endl;
    }
}

void DataManager::inputFromFileGraph(const std::string& filePath) {
    std::ifstream fileIn(filePath);

    if (!fileIn.is_open()) {
        std::cerr << "Error: DataManager::inputFromFileGraph cannot open input file '"
                  << filePath << "'." << std::endl;
        return;
    }

    std::string line;
    dataGraph.clear();
    bool hasAnyValid = false;

    while (std::getline(fileIn, line)) {
        if (line.empty() || (line.find_first_not_of(" \t\n\v\f\r") == std::string::npos)) {
            continue;
        }

        std::stringstream ss(line);
        int v1, v2, w;
        bool lineHasValid = false;
    
        while (true) {
            if (ss >> v1) {
                if (ss >> v2 && ss >> w) {
                    bool added = true;
                    // w can be negative
                    if (v1 >= 0 && v2 >= 0) {
                        dataGraph[v1].push_back({v2, w});
                        
                        dataGraph[v2]; // Set empty if v2 not exist or do nothing if exist
                    }
                    else if (v1 >= 0 && v2 == -1) {
                        dataGraph[v1]; 
                    }
                    else if (v1 == -1 && v2 >= 0) {
                        dataGraph[v2];
                    }
                    else {
                        added = false;
                    }

                    if (added) {
                        hasAnyValid = true;
                        lineHasValid = true;
                    }
                }
                else {
                    ss.clear();
                    ss.ignore(std::numeric_limits<std::streamsize>::max(), ' ');
                }
            } 
            else if (ss.eof()) {
                break; 
            } 
            else {
                ss.clear();
                ss.ignore(1); 
            }
        }

        if (!lineHasValid) {
            std::cout << "Invalid data input text file! No valid integers found on current line; line ignored." << std::endl;
        }
    }

    fileIn.close();

    if (!hasAnyValid) {
        std::cout << "There is not any valid integer in this file!" << std::endl;
    }
}

// Stream input
void DataManager::inputFromStream(std::istream& in) {
    data.clear();
    int value;

    while (true) {
        if (in >> value) {
            data.push_back(value);
        } else if (in.eof()) {
            break;
        } else {
            // Clear fail state, skip one char
            in.clear();
            in.ignore(1);
        }
    }
}

void DataManager::inputFromStreamGraph(std::istream& in) {
    dataGraph.clear();

    int v1, v2, w;

    while (true) {
        if (in >> v1) {
            if (in >> v2 && in >> w) {
                // w can be negative
                if (v1 >= 0 && v2 >= 0) {
                    dataGraph[v1].push_back({v2, w});
                    
                    dataGraph[v2];
                }
                else if (v1 >= 0 && v2 == -1) {
                    dataGraph[v1]; 
                }
                else if (v1 == -1 && v2 >= 0) {
                    dataGraph[v2];
                }
            }
            else {
                in.clear();
                in.ignore(std::numeric_limits<std::streamsize>::max(), ' ');
            }
        } 
        else if (in.eof()) {
            break; 
        } 
        else {
            in.clear();
            in.ignore(1); 
        }
    }
}

// Manual input
void DataManager::inputFromConsole(const std::string& text) {
    std::stringstream ss(text);
    inputFromStream(ss);
}

void DataManager::inputFromConsoleGraph(const std::string& text) {
    std::stringstream ss(text);
    inputFromStreamGraph(ss);
}

// File output
void DataManager::outputToFile(const std::string& filePath) const { // not changing class value
    std::ofstream fileOut(filePath);

    if (!fileOut.is_open()) {
        std::cerr << "Error: DataManager::outputToFile cannot open output file '"
                  << filePath << "'." << std::endl;

        return;
    }

    for (int value : data) {
        fileOut << value << " ";
    }
    
    fileOut.close();
}

void DataManager::outputToFileGraph(const std::string& filePath) const { // not changing class value
    std::ofstream fileOut(filePath);

    if (!fileOut.is_open()) {
        std::cerr << "Error: DataManager::outputToFileGraph cannot open output file '"
                  << filePath << "'." << std::endl;

        return;
    }

    for (auto& next : dataGraph) {
        for (std::pair<int, int> value : next.second) {
            std::cout << next.first << " -> " << value.first << " : " << value.second;
        }
    }
    
    fileOut.close();
}

// Manual output
void DataManager::outputToConsole() const { // not changing class value 
    for (int value : data) {
        std::cout << value << " ";
    }

    std::cout << std::endl;
}

void DataManager::outputToConsoleGraph() const { // not changing class value 
    for (auto& next : dataGraph) {
        for (std::pair<int, int> value : next.second) {
            std::cout << next.first << " -> " << value.first << " : " << value.second;
        }
    }

    std::cout << std::endl;
}

// Randomizer
void DataManager::randomData(int n, int minValue, int maxValue) {
    if (n <= 0) {
        std::cerr << "Error: DataManager::randomData received invalid size n="
                  << n << "." << std::endl;

        return;
    }
    if (minValue > maxValue) {
        std::cerr << "Error: DataManager::randomData received invalid range ["
                  << minValue << ", " << maxValue << "]." << std::endl;

        return;
    }

    data.clear();

    for (int i = 0; i < n; i++) {
        data.push_back(MathUtils::getRandomInRange(minValue, maxValue));
    }
}

void DataManager::randomDataGraph(int n, int minValue, int maxValue, float screenWidth, float screenHeight) {
    if (n <= 0) {
        std::cerr << "Error: DataManager::randomDataGraph received invalid vertice number n="
                  << n << "." << std::endl;

        return;
    }
    if (minValue > maxValue) {
        std::cerr << "Error: DataManager::randomDataGraph received invalid range ["
                  << minValue << ", " << maxValue << "]." << std::endl;

        return;
    }

    dataGraph.clear();
    nodePositions.clear();

    // Growing tree
    // std::vector<int> prim(n);
    // for (int i = 0; i < n; ++i)
    //     prim[i] = i;

    // int ptr = 1;
    // while (ptr < n) {
    //     int currIdx = MathUtils::getRandomInRange(0, ptr-1);
    //     int nextIdx = MathUtils::getRandomInRange(ptr, n-1);
    //     int weight = MathUtils::getRandomInRange(minValue, maxValue);

    //     dataGraph[prim[currIdx]].push_back({prim[nextIdx], weight});
    //     std::swap(prim[currIdx], prim[nextIdx]);
    //     ptr++;
    // }

    // New random
    // priority_queue cannot compare x and y in sf::Vector2f
    std::priority_queue<std::vector<int>> que;
    for (int i = 0; i < n; ++i) {
        // Range should be (padding, screen - padding)
        int x = MathUtils::getRandomInRange(0, static_cast<int>(screenWidth));
        int y = MathUtils::getRandomInRange(0, static_cast<int>(screenHeight));
        nodePositions[i] = {x, y};
        que.push({x, y, i});
    }

    std::vector<int> initialVertice = que.top();
    que.pop();
    while (!que.empty()) {
        std::vector<int> nextVertice = que.top();
        que.pop();

        int weight = MathUtils::getRandomInRange(minValue, maxValue);
        dataGraph[initialVertice[2]].push_back({nextVertice[2], weight});

        initialVertice = nextVertice;
    }

    // Adding remaining edges

    // Shuffle buffer
    std::vector<std::pair<int, int>> nodeBuffer;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i == j) continue;
            
            bool edgeExists = false;
            for (const auto& edge : dataGraph[i]) {
                if (edge.first == j) {
                    edgeExists = true;
                    break;
                }
            }

            if (!edgeExists) {
                nodeBuffer.push_back({i, j});
            }
        } 
    }

    int edge_addition = MathUtils::getRandomInRange(0, 2*n-5) ; // (3n-6) - (n-1)
    while (edge_addition > 0 && !nodeBuffer.empty()) {
        int randIdx = MathUtils::getRandomInRange(0, nodeBuffer.size() - 1);
        bool hasIntersect = false;

        int idx1 = nodeBuffer[randIdx].first, idx2 = nodeBuffer[randIdx].second;
        for (auto const& [u, edges] : dataGraph) { // u là ID đỉnh nguồn
            for (auto const& edge : edges) {       // duyệt từng cạnh của đỉnh u
                int v = edge.first;

                // Same vertice
                if (idx1 == u || idx1 == v || idx2 == u || idx2 == v) 
                    continue;

                if (MathUtils::doIntersect(nodePositions[idx1], nodePositions[idx2], nodePositions[u], nodePositions[v])) {
                    hasIntersect = true;
                    break;
                }
            }
        }

        if (!hasIntersect) {
            int weight = MathUtils::getRandomInRange(minValue, maxValue);
            dataGraph[idx1].push_back({idx2, weight});
            edge_addition--; 
        }

        // The graph may not store more edge_addition edges
        nodeBuffer[randIdx] = nodeBuffer.back();
        nodeBuffer.pop_back();
    }
}

// Data getter
const std::vector<int>& DataManager::getData() const {
    // Neither changing vector data nor changing class value
    return data; 
}

const DataManager::GraphData& DataManager::getDataGraph() const {
    return dataGraph;
}

const std::unordered_map<int, sf::Vector2f>& DataManager::getNodePositions() const {
    return nodePositions;
}
