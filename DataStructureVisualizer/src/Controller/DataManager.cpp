#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <ctime>
#include <cstdlib>

#include "DataManager.h"

// File input
void DataManager::inputFromFile(const std::string& filePath) {
    std::ifstream fileIn(filePath);

    if (!fileIn.is_open()) {
        std::cerr << "Error: Cannot open the input file!" << std::endl;

        return;
    }

    std::string line = "";
    data.clear();

    while (std::getline(fileIn, line)) {
        std::stringstream ss(line);
        int value;
        
        while (ss >> value) {
            data.push_back(value);
        }
    }

    fileIn.close();
}

// Manual input
void DataManager::inputFromConsole(const std::string& text) {
    data.clear();
    std::stringstream ss(text);
    int value;

    while (ss >> value) {
        data.push_back(value);
    }
}

// File output
void DataManager::outputToFile(const std::string& filePath) const { // not changing class value
    std::ofstream fileOut(filePath);

    if (!fileOut.is_open()) {
        std::cerr << "Error: Cannot open the output file!" << endl;

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
    data.clear();

    srand(time(0)); // This should be put in MAIN for calling just one seed
    for (int i = 0; i < n; i++) {
        int value = minValue + rand() % (maxValue - minValue + 1);
        data.push_back(value);
    }
}

// Data getter
const std::vector<int>& DataManager::getData() const {
    // Neither changing vector data nor changing class value
    return data; 
}
