#ifndef DATAMANAGER_HPP
#define DATAMANAGER_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

class DataManager
{
private:
    std::vector<int> data;
public:
    // Input
    void inputFromFile(const string& filePath);

    void inputFromConsole(const std::string& text);
    
    // Output
    void outputToFile(const std::string& filePath) const;

    void outputToConsole() const;
};

#endif