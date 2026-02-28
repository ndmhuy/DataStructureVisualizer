#!/bin/bash

# Ensure the script runs in the directory where it's located
cd "$(dirname "$0")"

echo "========================================="
echo "Building Data Structure Visualizer (macOS)"
echo "========================================="

# Create build directory if it doesn't exist
mkdir -p build
cd build

# 1. Configure the project with CMake
echo -e "\n[1/3] Configuring CMake..."
cmake ..

# 2. Compile the project
echo -e "\n[2/3] Compiling C++ Code..."
cmake --build .

# 3. Find and run the executable
echo -e "\n[3/3] Launching Application..."
echo "========================================="

if [ -f "./DataStructureVisualizer" ]; then
    ./DataStructureVisualizer
else
    echo -e "\nERROR: Could not find the executable. Please check the compile errors above!"
fi
