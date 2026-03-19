#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <random>

#include "Controller/DataManager.h"

// File input
void DataManager::inputFromFile(const std::string& filePath) {
    std::ifstream fileIn(filePath);

    if (!fileIn.is_open()) {
        std::cerr << "Error: Cannot open the input file!" << std::endl;
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

        while (ss >> value) {
            data.push_back(value);
            hasAnyValid = true;
            lineHasValid = true;
        }

        // When input file only contains non "specific" integer value
        if (!lineHasValid && (ss.fail() || !ss.eof())) {
            std::cout << "Invalid data input text file! No valid integers found on current line; line ignored." << std::endl;
        }
        // Warning when there is some integer and some text
        else if (lineHasValid && ss.fail() && !ss.eof()) {
            std::cout << "Warning: Non-integer content after valid integers ignored on current line." << std::endl;
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

    while (in >> value) {
        data.push_back(value);
    }
}

// Manual input
void DataManager::inputFromConsole(const std::string& text) {
    std::stringstream ss(text);
    inputFromStream(ss);
}

// File output
void DataManager::outputToFile(const std::string& filePath) const { // not changing class value
    std::ofstream fileOut(filePath);

    if (!fileOut.is_open()) {
        std::cerr << "Error: Cannot open the output file!" << std::endl;

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
        std::cerr << "Invalid data size!" << std::endl;

        return;
    }
    if (minValue > maxValue) {
        std::cerr << "Invalid range!" << std::endl;

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
