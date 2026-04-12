#include <iostream>
#include <sstream>
#include <random>
#include <algorithm>

#include "Controller/DataManager.h"

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

void DataManager::inputFromFile(const std::string& filePath) {
    std::ifstream fileIn(filePath);

    if (!fileIn.is_open()) {
        std::cerr << "Error: DataManager::inputFromFile cannot open input file '"
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

// Manual output
void DataManager::outputToConsole() const { // not changing class value 
    for (int value : data) {
        std::cout << value << " ";
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

    static thread_local std::mt19937 engine(std::random_device{}());
    std::uniform_int_distribution<int> dist(minValue, maxValue);

    for (int i = 0; i < n; i++) {
        data.push_back(dist(engine));
    }
}

// Data getter
const std::vector<int>& DataManager::getData() const {
    // Neither changing vector data nor changing class value
    return data; 
}

const std::unordered_map<int, std::vector<std::pair<int, int>>>& DataManager::getDataGraph() const {
    return dataGraph;
}