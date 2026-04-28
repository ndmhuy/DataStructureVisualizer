# CS163 Data Structure Visualizer

## Overview
Welcome to the CS163 Data Structure Visualizer! This project is an interactive tool designed to help users understand various data structures (such as Linked Lists, Heaps, AVL Trees, and Graphs) and their algorithms through step-by-step animations and synchronized pseudocode. 

**Links:**
* **Demo Video:** [Watch on YouTube](https://youtu.be/yeWcSl\_vmm8?si=3cRV\_46PZOY9nGyl)
* **GitHub Repository:** [DataStructureVisualizer](https://github.com/ndmhuy/DataStructureVisualizer.git)
* **Marking Link 1:** [Google Sheets](https://docs.google.com/spreadsheets/d/1LUOf2ghZPS7XZFOv3BseZurSNlruZG4B/edit?gid=623355065#gid=623355065)
* **Marking Link 2:** [Google Sheets](https://docs.google.com/spreadsheets/d/1RSPTgHUg1bgCLG5kOZPGbZshFIRhyMVpxjx0V2vyMlo/edit?gid=747030814#gid=747030814)

---

## Requirements
To compile and run this project, you will need the following dependencies installed on your system:
* **C++ Compiler:** Supporting C++17 or higher (e.g., GCC, Clang, or MSVC).
* **CMake:** Version 3.10 or higher.
* **SFML (Simple and Fast Multimedia Library):** Used for rendering graphical components.
* **Dear ImGui:** Used for the interactive User Interface (bundled in the project / handled via CMake).

---

## Project Layout
The repository is strictly structured using the **Model-View-Controller (MVC)** architectural pattern:

```text
DataStructureVisualizer/
├── include/                # Header files (.h)
│   ├── Model/              # Data states, timelines, and structures (Timeline.h, Frame.h)
│   ├── View/               # UI and Rendering logic (UIManager.h, Renderer.h)
│   └── Controller/         # Application coordination and playback (AppEngine.h)
├── src/                    # Implementation files (.cpp)
│   ├── Model/              
│   ├── View/               
│   ├── Controller/         
│   └── main.cpp            # Application entry point
├── assets/                 # Fonts, icons, and default data files
├── CMakeLists.txt          # CMake configuration
└── run.bat / run.command   # Automated build scripts for Windows/Mac
```

---

## How to Build and Run

We use CMake to ensure cross-platform compatibility across Windows and macOS.

### 1. Clone the Repository
```bash
git clone https://github.com/ndmhuy/DataStructureVisualizer.git
cd DataStructureVisualizer
```

### 2. Automated Scripts (Recommended)
You can easily build and launch the project using the provided scripts:

* **Windows:** 
  Double-click `run.bat` in the root folder.
* **macOS:** 
  Open Terminal, make the script executable, and run it:
  ```bash
  chmod +x run.command
  ./run.command
  ```

### 3. Manual Build (via CMake)
If you prefer to compile manually using a terminal or an IDE (like VS Code or CLion):
```bash
mkdir -p build
cd build
cmake ..
cmake --build .
```
After a successful build, the executable will be generated inside the `build/` directory (or `build/Debug/` on some setups).

---

## Input Format (File I/O)
The visualizer allows importing initial states for data structures via text files. 

The application uses the DataManager class to handle data ingestion. It supports three main modalities: *File I/O*, *Direct Console Input*, and *Random Generation*.

### 1. Linear Structures & Trees
Used for: Static Arrays, Linked Lists, Heaps, BST, and AVL Trees.

* *Format:* A single line of integers separated by spaces or commas.
* *Methods:* inputFromFile, inputFromConsole, inputFromStream.
* *Example:*
    
text
    15, 10, 25, 5, 12, 20, 30
    

### 2. General Graphs
Used for: Directed/Undirected and Weighted Graphs.

* *Format:* Adjacency Matrix representation.
    * *Line 1:* An integer $V$ (number of vertices).
    * *Next $V$ lines:* $V$ integers each, representing edge weights (use 0 for no edge).
* *Methods:* inputFromFileGraph, inputFromConsoleGraph.
* *Example (3-vertex graph):*
    
text
    3
    0 5 0
    0 0 10
    5 0 0
    

### 3. Grid Graphs (Pathfinding)
Used for: A and Dijkstra algorithms on 2D Grids.*

* *Format:*
    * *Line 1:* Two integers $R$ and $C$ (Rows and Columns).
    * *Following $R$ lines:* $C$ binary integers (0 for walkable path, 1 for wall/obstacle).
* *Methods:* inputFromFileGridGraph, inputFromConsoleGridGraph.
* *Example:*
    
text
    4 4
    0 0 1 0
    1 0 1 0
    0 0 0 0
    0 1 0 0
    

### 4. Synthetic Data (Randomizers)
The DataManager provides built-in engines to generate test cases directly from the UI:

* *Standard Random:* randomData(n, min, max) for basic arrays/trees.
* *Planar Graph:* randomDataPlanarGraph() generates graphs with pre-calculated coordinates to ensure visual clarity.
* *DAG:* randomDataDAG() specifically generates Directed Acyclic Graphs for topological sort visualizations.
* *Maze/Grid:* randomDataGridGraph() generates 2D grids with a configurable wallPercentage.

---

## Technical Notes
* *Validation:* The system uses inputFromConsoleNonNegative to filter out invalid inputs before processing.
* *Efficiency:* Before any new ingestion, data.clear() is invoked to ensure memory safety and prevent data leakage between sessions.

*(Note: Specific formatting requirements depend on the data structure being loaded. See the /resources/ or /TestFiles/ folder for example text file templates.)*
