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
    bool isTrash = true;

    while (std::getline(fileIn, line)) {
        if (line.empty() || (line.find_first_not_of(" \t\n\v\f\r") == std::string::npos)) {
            continue;
        }

        std::stringstream ss(line);
        int value;
        
        while (!ss.eof()) {
            
            if (ss >> value) {
                isTrash = false;
                data.push_back(value);
            }
            else {
                ss.clear();
                ss.ignore(1);
            }
        }

        if (isTrash && ss.eof()) {
            std::cout << "Invalid data input text file!" << std::endl;
            ss.ignore()
        }
    }

    fileIn.close();

    if (isTrash) {
        std::cout << "There is not any valid interger in this file!" << std::endl;
    }
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
    if (n <= 0) {
        std::cerr << "Invalid data size!";

        return;
    }
    if (minValue > maxValue) {
        std::cerr << "Invalid range!";

        return;
    }

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
