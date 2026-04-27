#ifndef DATAMANAGER_HPP
#define DATAMANAGER_HPP

#include "Utilities/Position.h"

#include <vector>
#include <string>
#include <fstream>
#include <unordered_map>
#include <Model/GraphStructure/Edge.h>

class DataManager
{
public:
    // Input
    void inputFromFile(const std::string& filePath);

    void inputFromFileGraph(const std::string& filePath);

    void inputFromFileGridGraph(const std::string& filePath);

    void inputFromStream(std::istream& in); // Input directly on terminal

    void inputFromStreamGraph(std::istream& in); // Input directly on terminal

    void inputFromStreamGridGraph(std::istream& in);

    void inputFromConsole(const std::string& text); // Input by changing the given text

    void inputFromConsoleGraph(const std::string& text); // Input by changing the given text

    void inputFromConsoleGridGraph(const std::string& text);

    void inputFromConsoleNonNegative(const std::string& text);

    // Output
    void outputToFile(const std::string& filePath) const;

    void outputToFileGraph(const std::string& filePath) const;

    void outputToConsole() const;

    void outputToConsoleGraph() const;

    // Getter
    const std::vector<int>& getData() const;

    const std::vector<Edge>& getDataGraph() const;

    const std::vector<std::vector<int>>& getDataGridGraph() const;

    const std::vector<Position>& getNodePositions() const;

    // Data randomizer
    void randomData(int n, int minValue, int maxValue);

    void randomDataGraph(int vertexCount, int edgeCount, int minValue, int maxValue);

    void randomDataPlanarGraph(int n, int minValue, int maxValue, float screenWidth, float screenHeight);

    void randomDataDAG(int vertexCount, int edgeCount, int minWeight, int maxWeight);

    void randomDataGridGraph(size_t rows, size_t cols, int wallPercentage);

private:
    std::vector<int> data;
    std::vector<Edge> dataGraph; // Edge vector: {u, v, w}
    std::vector<std::vector<int>> dataGridGraph;
    std::vector<Position> nodePositions;
};

#endif