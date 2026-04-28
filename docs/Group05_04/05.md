# Weekly Report: [W04]

## General Information
* **Group ID:** 05
* **Group Name:**
* **Class:** 25A01
* **Project Name:** Data Structure Visualizer
* **Date range:** 2026-03-02 – 2026-03-07
* **Github Repository:** https://github.com/ndmhuy/DataStructureVisualizer.git

## Tasks Completed This Week

During this week, our group decided to divide the project timeline into 2 phases. Phase 1 focuses on building the MVC framework for representing and visualizing data structures, while Phase 2 will be dedicated to implementing the logic for specific data structures. We divided Phase 1 into 4 main domains, assigning each to a specific team member to avoid Git conflicts.

**25125083 - Nguyễn Đình Minh Huy**
* Task 1: Designed the project development timeline and MVC folder architecture.
* Task 2: Established group collaboration rules, including Git branch naming conventions and task division for Phase 1.
* Task 3: Set up the cross-platform CMake build environment, run scripts, and LaTeX report structure.
* Task 4: Write weekly report templates and the initial README.md and PHASE1_TASKS.md documentation.
* **Evidence:** GitHub repository initialization (CMakeLists.txt, run.bat, run.command, README.md, PHASE1_TASKS.md).

**25125074 - Nguyễn Anh Kiệt**
* Task 1: Initialized the Window API and successfully configured the SFML 3.0.2 Environment via CMake FetchContent (Issue #2).
* Task 2: Implemented the main graphical interface module (`Window.h` and `Window.cpp`) with a 60 FPS limit and custom dark theme background.
* **Evidence:** Commits related to `Window` implementation and SFML integration.

**25125045 - Trần Như Khải**
* Task 1: Researched Dear ImGui integration with SFML for the upcoming UI control panel design.
* Task 2: Built and tested a prototype SFML window program to verify local rendering capabilities.
* **Evidence:** [Testing SFML programme](https://drive.google.com/drive/folders/1rKOs7w0GftqWsP_KnI9TTmaRtL2xOtwo?usp=drive_link)

**25125014 - Nguyễn Đức Huy**
* Task 1: Write a generic file parser in the `Controller` that reads an external text/CSV file and returns a standard array/vector of strings or integers.
* Task 2: Implement logic to generate randomized data arrays.
**Evidence:** Commits implementing File I/O parsing logic in the Controller.

## AI Usage Declaration
* Used AI to brainstorm the project MVC architecture, generate boilerplate code for folder structure generation, and write documentation templates.
* Used AI to troubleshoot new syntax changes in SFML 3.x (specifically using `std::optional` for `pollEvent`), understand C++ class encapsulation, and resolve Git `pathspec` errors via the VS Code GUI.
* Used AI to suggest an optimized outline for the data management system, leveraged AI to deepen understanding of memory safety through constant references and class structures.
* Help with implement the project architechture, create the folder structure, write docs
* Provided necessary knowledge.
[Chat history](https://chatgpt.com/share/69ac3542-940c-8001-9cf1-68575f51aae7)
[Chat history](https://gemini.google.com/share/85d43d6040e9)

## Tasks Planned for Next Week
* **Task 1:** Complete Phase 1 Framework integration

## Issues
* **Issue 1:** We encountered a "pathspec did not match" error in Git when attempting to restore `main.cpp` via the terminal.
  * **Resolution:** Switched to VS Code's built-in Source Control GUI to safely unstage/discard `main.cpp` changes, effectively avoiding merge conflicts with the Core Architect's base code.
* **Issue 2:** Faced compilation errors in the Window system because online tutorials use SFML 2.x syntax, but our project is downloading the newly released SFML 3.0.2 environment.
  * **Resolution:** Researched the new 3.0 API documentation, included the `<optional>` header, and updated the window event polling loop to match the modern standards.
