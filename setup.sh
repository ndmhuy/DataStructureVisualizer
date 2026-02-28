#!/bin/bash

echo "Creating CS163-Data-Visualizer folder structure..."

# Create the root and subdirectories
mkdir -p CS163-Data-Visualizer/{resources,include/{Model,View,Controller},src/{Model,View,Controller}}

# Enter the root directory
cd CS163-Data-Visualizer

# Create root files
touch .gitignore CMakeLists.txt README.md

# Create Model files
touch include/Model/Theme.h include/Model/Frame.h include/Model/Timeline.h
touch src/Model/Theme.cpp src/Model/Frame.cpp src/Model/Timeline.cpp

# Create View files
touch include/View/Window.h include/View/UIManager.h include/View/Renderer.h
touch src/View/Window.cpp src/View/UIManager.cpp src/View/Renderer.cpp

# Create Controller files
touch include/Controller/AppEngine.h include/Controller/Playback.h
touch src/Controller/AppEngine.cpp src/Controller/Playback.cpp

# Create the required main.cpp file
touch src/main.cpp

# Add some basic content to main.cpp so it compiles right away
cat <<EOT >> src/main.cpp
#include <iostream>

int main() {
    std::cout << "Data Visualizer Framework Initialized!" << std::endl;
    return 0;
}
EOT

echo "Done! The project is ready for CMake."
