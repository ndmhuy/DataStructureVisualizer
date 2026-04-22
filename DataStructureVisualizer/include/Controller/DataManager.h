#ifndef DATAMANAGER_HPP
#define DATAMANAGER_HPP

#include <vector>
#include <string>
#include <fstream>
#include <unordered_map>
#include <SFML/System/Vector2.hpp>

class DataManager
{
public:
    using Edge = std::pair<int, int>; 
    using GraphData = std::unordered_map<int, std::vector<Edge>>;

    // Input
    void inputFromFile(const std::string& filePath);

    void inputFromFileGraph(const std::string& filePath);

    void inputFromStream(std::istream& in); // Input directly on terminal

    void inputFromStreamGraph(std::istream& in); // Input directly on terminal

    void inputFromConsole(const std::string& text); // Input by changing the given text

    void inputFromConsoleGraph(const std::string& text); // Input by changing the given text

    // Output
    void outputToFile(const std::string& filePath) const;

    void outputToFileGraph(const std::string& filePath) const;

    void outputToConsole() const;

    void outputToConsoleGraph() const;

    // Getter
    const std::vector<int>& getData() const;

    const GraphData& getDataGraph() const;

    const std::unordered_map<int, sf::Vector2f>& getNodePositions() const;

    // Data randomizer
    void randomData(int n, int minValue, int maxValue);

    void randomDataGraph(int n, int minValue, int maxValue, float screenWidth, float screenHeight);

private:
    std::vector<int> data;
    GraphData dataGraph;
    std::unordered_map<int, sf::Vector2f> nodePositions;
};

#endif