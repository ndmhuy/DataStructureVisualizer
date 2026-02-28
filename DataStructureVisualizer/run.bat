@echo off
echo =========================================
echo Building Data Structure Visualizer (Windows)
echo =========================================

:: Create a build directory if it doesn't exist to keep the root folder clean
if not exist build mkdir build
cd build

:: 1. Configure the project with CMake
echo.
echo [1/3] Configuring CMake...
cmake ..

:: 2. Compile the project
echo.
echo [2/3] Compiling C++ Code...
cmake --build .

:: 3. Find and run the executable
echo.
echo [3/3] Launching Application...
echo =========================================

:: MSVC usually puts the .exe inside a Debug folder, MinGW puts it in the root build folder
if exist Debug\DataStructureVisualizer.exe (
    Debug\DataStructureVisualizer.exe
) else if exist Release\DataStructureVisualizer.exe (
    Release\DataStructureVisualizer.exe
) else if exist DataStructureVisualizer.exe (
    DataStructureVisualizer.exe
) else (
    echo.
    echo ERROR: Could not find the executable. Please check the compile errors above!
)

echo.
pause
