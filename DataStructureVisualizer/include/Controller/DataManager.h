#ifndef DATAMANAGER_HPP
#define DATAMANAGER_HPP

#include <vector>
#include <string>
#include <fstream>

class DataManager
{
private:
    std::vector<int> data;
public:
    // Input
    void inputFromFile(const std::string& filePath);

    void inputFromStream(std::istream& in); // Input directly on terminal

    void inputFromConsole(const std::string& text); // Input by changing the given text

    // Output
    void outputToFile(const std::string& filePath) const;

    void outputToConsole() const;

    // Getter
    const std::vector<int>& getData() const;

    // Data randomizer
    void randomData(int n, int minValue, int maxValue);
};

#endif