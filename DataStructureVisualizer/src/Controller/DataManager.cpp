#include <iostream>
#include <sstream>
#include <algorithm>
#include <queue>
#include <set>

#include "Controller/DataManager.h"
#include "Utilities/MathUtils.h"
#include "Utilities/GlobalConstant.h"

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
        std::vector<int> buffer;
        int value;

        while (true) {
            if (ss >> value) {
                buffer.push_back(value);
            } else if (ss.eof()) {
                break;
            } else {
                ss.clear();
                ss.ignore(1);
            }
        }

        bool lineHasValid = false;
        size_t n = buffer.size();

        if (n >= 3 && n % 3 == 0) {
            for (size_t i = 0; i < n; i += 3) {
                int u = buffer[i];
                int v = buffer[i + 1];
                int w = buffer[i + 2];

                if (u >= 0 && v >= 0) {
                    dataGraph.push_back({(size_t)u, (size_t)v, w});
                    lineHasValid = true;
                } 
                else if (u >= 0 && v == -1) {
                    dataGraph.push_back({(size_t)u, INVALID_INDEX, w});
                    lineHasValid = true;
                } 
                else if (u == -1 && v >= 0) {
                    dataGraph.push_back({(size_t)v, INVALID_INDEX, w}); 
                    lineHasValid = true;
                }
            }
        }
        else if (n >= 2 && n % 2 == 0) {
            for (size_t i = 0; i < n; i += 2) {
                int u = buffer[i];
                int v = buffer[i + 1];

                if (u >= 0 && v >= 0) {
                    dataGraph.push_back({(size_t)u, (size_t)v, 1});
                    lineHasValid = true;
                } 
                else if (u >= 0 && v == -1) {
                    dataGraph.push_back({(size_t)u, INVALID_INDEX, 1});
                    lineHasValid = true;
                } 
                else if (u == -1 && v >= 0) {
                    dataGraph.push_back({(size_t)v, INVALID_INDEX, 1});
                    lineHasValid = true;
                }
            }
        }

        if (lineHasValid) {
            hasAnyValid = true;
        }
        else {
            std::cout << "Invalid data input text file! No valid edge format found on current line; line ignored." << std::endl;
        }
    }

    fileIn.close();

    if (!hasAnyValid) {
        std::cout << "There is not any valid integer in this file!" << std::endl;
    }
}

void DataManager::inputFromFileGridGraph(const std::string& filePath) {
    std::ifstream fileIn(filePath);

    if (!fileIn.is_open()) {
        std::cerr << "Error: DataManager::inputFromFileGridGraph cannot open input file '"
                  << filePath << "'." << std::endl;
        return;
    }

    dataGridGraph.clear();
    std::vector<int> buffer;
    int value;

    while (true) {
        if (fileIn >> value) {
            buffer.push_back(value);
        } else if (fileIn.eof()) {
            break;
        } else {
            fileIn.clear();
            fileIn.ignore(1); 
        }
    }

    fileIn.close();

    if (buffer.size() < 2) {
        std::cout << "Invalid Grid Data File: Not enough data for rows and cols." << std::endl;
        return;
    }

    if (buffer[0] <= 0 || buffer[1] <= 0) {
        std::cout << "Invalid Grid Data File: Dimensions must be positive integers (> 0)." << std::endl;
        return;
    }

    size_t rows = static_cast<size_t>(buffer[0]);
    size_t cols = static_cast<size_t>(buffer[1]);

    dataGridGraph.assign(rows, std::vector<int>(cols, 0));

    size_t bufferIdx = 2; 
    for (size_t r = 0; r < rows; ++r) {
        for (size_t c = 0; c < cols; ++c) {
            if (bufferIdx < buffer.size()) {
                int val = buffer[bufferIdx++];
                
                if (val >= 0 && val <= 3) {
                    dataGridGraph[r][c] = val;
                }
            } else {
                return; 
            }
        }
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

    std::vector<int> buffer;
    int value;

    while (true) {
        if (in >> value) {
            buffer.push_back(value);
        } else if (in.eof()) {
            break;
        } else {
            in.clear(); 
            in.ignore(1);
        }
    }

    if (buffer.empty()) return;

    size_t n = buffer.size();

    if (n >= 3 && n % 3 == 0) {
        for (size_t i = 0; i < n; i += 3) {
            size_t u = buffer[i] < 0 ? INVALID_INDEX : buffer[i];
            size_t v = buffer[i + 1] < 0 ? INVALID_INDEX : buffer[i + 1];
            int w = buffer[i + 2];
            
            dataGraph.push_back({u, v, w}); 
        }
    } 
    else if (n >= 2 && n % 2 == 0) {
        for (size_t i = 0; i < n; i += 2) {
            size_t u = buffer[i] < 0 ? INVALID_INDEX : buffer[i];
            size_t v = buffer[i + 1] < 0 ? INVALID_INDEX : buffer[i + 1];
            
            dataGraph.push_back({u, v, 1}); 
        }
    }
}

void DataManager::inputFromStreamGridGraph(std::istream& in) {
    dataGridGraph.clear();

    int r, c;

    if (!(in >> r >> c) || r <= 0 || c <= 0) {
        std::cout << "Invalid Grid Data: Dimensions must be positive integers." << std::endl;
        return;
    }

    size_t rows = static_cast<size_t>(r);
    size_t cols = static_cast<size_t>(c);

    dataGridGraph.assign(rows, std::vector<int>(cols, 0));

    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            int val;
            if (in >> val) {
                if (val >= 0 && val <= 3) {
                    dataGridGraph[i][j] = val;
                }
            } else {
                return;
            }
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

void DataManager::inputFromConsoleGridGraph(const std::string& text) {
    std::stringstream ss(text);
    inputFromStreamGridGraph(ss);
}

void DataManager::inputFromConsoleNonNegative(const std::string& text) {
    inputFromConsole(text);

    for (int val : data) {
        if (val < 0) {
            std::cout << "Invalid Input: Negative numbers are not allowed for this action!" << std::endl;
            data.clear(); 
            return;
        }
    }
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

    for (auto& value : dataGraph) {
        if (value.to == INVALID_INDEX) {
            std::cout << value.from << " (isolated)\n";
        } else {
            std::cout << value.from << " -> " << value.to << " : " << value.weight << "\n";
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
    for (auto& value : dataGraph) {
        if (value.to == INVALID_INDEX) {
            std::cout << value.from << " (isolated)\n";
        } else {
            std::cout << value.from << " -> " << value.to << " : " << value.weight << "\n";
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

    data.clear();

    for (int i = 0; i < n; i++) {
        data.push_back(MathUtils::getRandomInRange(minValue, maxValue));
    }
}

void DataManager::randomDataGraph(int vertexCount, int edgeCount, int minValue, int maxValue) {
    if (vertexCount <= 0) {
        std::cerr << "Error: DataManager::randomDataGraph received invalid vertexCount="
                  << vertexCount << "." << std::endl;

        return;
    }
    if (edgeCount < 0) {
        std::cerr << "Error: DataManager::randomDataGraph received invalid edgeCount="
                  << edgeCount << "." << std::endl;

        return;
    }

    dataGraph.clear();

    for (int i = 0; i < vertexCount; ++i) {
        dataGraph.push_back({(size_t)i, INVALID_INDEX, 1}); // Single vertice
    }

    int maxEdges = vertexCount * (vertexCount - 1); 
    if (edgeCount > maxEdges) {
        edgeCount = maxEdges;
    }

    std::set<std::pair<int, int>> existingEdges;

    for (int i = 0; i < edgeCount; i++) {
        int u = MathUtils::getRandomInRange(0, vertexCount - 1);
        int v = MathUtils::getRandomInRange(0, vertexCount - 1);

        if (u == v || existingEdges.count({u, v})) {
            i--;
            continue;
        }

        existingEdges.insert({u, v});
        
        int weight = MathUtils::getRandomInRange(minValue, maxValue);
        dataGraph.push_back({(size_t)u, (size_t)v, weight});
    }
}

void DataManager::randomDataPlanarGraph(int n, int minValue, int maxValue, float screenWidth, float screenHeight) {
    if (n <= 0) {
        std::cerr << "Error: DataManager::randomDataPlanarGraph received invalid vertice number n="
                  << n << "." << std::endl;

        return;
    }

    dataGraph.clear();
    nodePositions.clear();
    nodePositions.resize(n);
    float margin = 100.0f;

    // priority_queue cannot compare x and y in sf::Vector2f
    std::priority_queue<std::vector<int>> que;
    for (int i = 0; i < n; ++i) {
        // Range should be (padding, screen - padding)
        int x = MathUtils::getRandomInRange(margin, static_cast<int>(screenWidth)-margin);
        int y = MathUtils::getRandomInRange(margin, static_cast<int>(screenHeight)-margin);
        nodePositions[i] = Position(static_cast<float>(x), static_cast<float>(y));
        que.push({x, y, i});
    }

    std::set<std::pair<int, int>> existingEdges;

    std::vector<int> initialVertice = que.top();
    que.pop();
    while (!que.empty()) {
        std::vector<int> nextVertice = que.top();
        que.pop();

        int u = initialVertice[2];
        int v = nextVertice[2];
        int weight = MathUtils::getRandomInRange(minValue, maxValue);

        dataGraph.push_back({(size_t)u, (size_t)v, weight});
        existingEdges.insert({u, v}); // Đánh dấu cạnh đã tồn tại

        initialVertice = nextVertice;
    }

    // Adding remaining edges

    // Shuffle buffer
    std::vector<std::pair<int, int>> nodeBuffer;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i == j) continue;
            
            if (!existingEdges.count({i, j})) {
                nodeBuffer.push_back({i, j});
            }
        } 
    }

    int maxEdgesToAdd = std::max(0, 2 * n - 5);
    int edge_addition = (maxEdgesToAdd > 0) ? MathUtils::getRandomInRange(0, maxEdgesToAdd) : 0; // (3n-6) - (n-1)
    while (edge_addition > 0 && !nodeBuffer.empty()) {
        int randIdx = MathUtils::getRandomInRange(0, nodeBuffer.size() - 1);
        bool hasIntersect = false;

        int idx1 = nodeBuffer[randIdx].first;
        int idx2 = nodeBuffer[randIdx].second;
        for (const auto& edge : dataGraph) { 
            if (edge.to == INVALID_INDEX)
                continue;

            int u = edge.from;
            int v = edge.to;

            if (idx1 == u || idx1 == v || idx2 == u || idx2 == v) 
                continue;

            if (MathUtils::doIntersect(nodePositions[idx1], nodePositions[idx2], nodePositions[u], nodePositions[v])) {
                hasIntersect = true;
                break;
            }
        }

        if (!hasIntersect) {
            int weight = MathUtils::getRandomInRange(minValue, maxValue);
            
            dataGraph.push_back({(size_t)idx1, (size_t)idx2, weight});
            existingEdges.insert({idx1, idx2});

            edge_addition--; 
        }

        // The graph may not store more edge_addition edges
        nodeBuffer[randIdx] = nodeBuffer.back();
        nodeBuffer.pop_back();
    }
}

void DataManager::randomDataDAG(int vertexCount, int edgeCount, int minWeight, int maxWeight) {
    dataGraph.clear();
    
    if (vertexCount <= 0) return;

    int maxPossibleEdges = vertexCount * (vertexCount - 1) / 2;
    if (edgeCount > maxPossibleEdges) {
        edgeCount = maxPossibleEdges; 
    }

    std::set<std::pair<int, int>> existingEdges;
    
    int attempts = 0;
    int maxAttempts = edgeCount * 10; 

    while (existingEdges.size() < (size_t)edgeCount && attempts < maxAttempts) {
        int u = MathUtils::getRandomInRange(0, vertexCount - 1);
        int v = MathUtils::getRandomInRange(0, vertexCount - 1);

        if (u > v) {
            std::swap(u, v);
        }

        if (u != v && existingEdges.find({u, v}) == existingEdges.end()) {
            existingEdges.insert({u, v});
            int weight = MathUtils::getRandomInRange(minWeight, maxWeight);
            
            dataGraph.push_back(Edge((size_t)u, (size_t)v, weight));
        }
        attempts++;
    }
}

void DataManager::randomDataGridGraph(size_t rows, size_t cols, int wallPercentage) {
    if (rows <= 0 || cols <= 0) return;

    dataGridGraph.assign(rows, std::vector<int>(cols, 0));

    size_t startR = MathUtils::getRandomInRange(0, rows - 1);
    size_t startC = MathUtils::getRandomInRange(0, cols - 1);
    dataGridGraph[startR][startC] = 2;

    size_t targetR, targetC;
    do {
        targetR = MathUtils::getRandomInRange(0, rows - 1);
        targetC = MathUtils::getRandomInRange(0, cols - 1);
    } while (targetR == startR && targetC == startC);
    
    dataGridGraph[targetR][targetC] = 3;

    for (size_t r = 0; r < rows; ++r) {
        for (size_t c = 0; c < cols; ++c) {
            if (dataGridGraph[r][c] != 0) continue;

            // Random wall depends on percent
            if (MathUtils::getRandomInRange(1, 100) <= wallPercentage) {
                dataGridGraph[r][c] = 1;
            }
        }
    }
}

// Data getter
const std::vector<int>& DataManager::getData() const {
    // Neither changing vector data nor changing class value
    return data; 
}

const std::vector<Edge>& DataManager::getDataGraph() const {
    return dataGraph;
}

const std::vector<std::vector<int>>& DataManager::getDataGridGraph() const {
    return dataGridGraph;
}

const std::vector<Position>& DataManager::getNodePositions() const {
    return nodePositions;
}
