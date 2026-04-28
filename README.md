# CS163 Data Structure Visualizer

## Overview
Welcome to the CS163 Data Structure Visualizer! This project is an interactive tool designed to help users understand various data structures (such as Linked Lists, Heaps, AVL Trees, and Graphs) and their algorithms through step-by-step animations and synchronized pseudocode. 

**Links:**
* **Demo Video:** [Watch on YouTube](https://youtu.be/yeWcSl\_vmm8?si=3cRV\_46PZOY9nGyl)
* **GitHub Repository:** [DataStructureVisualizer](https://github.com/ndmhuy/DataStructureVisualizer.git)

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

*General Format Guidelines:*
* **Graphs:** Adjacency matrix representation. The first line specifies the number of vertices $V$. The following $V$ lines contain $V$ space-separated integers representing the edge weights (with `0` typically representing no edge).
* **Arrays / Lists / Heaps / Trees:** Comma-separated or space-separated integer values on a single line (e.g., `10, 25, 5, 14, 30`).

*(Note: Specific formatting requirements depend on the data structure being loaded. See the `/resources/` or `/TestFiles/` folder for example text file templates.)*
