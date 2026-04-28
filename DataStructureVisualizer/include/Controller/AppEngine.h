#ifndef APPENGINE_H
#define APPENGINE_H

#include "Controller/Playback.h"
#include "Controller/DataManager.h"

#include "Model/IVisualizable.h"

#include "Utilities/StructureType.h"

#include "View/Core/Theme.h"
#include "View/Core/Window.h"
#include "View/Render/Renderer.h"
#include "View/UI/UIManager.h"

class AppEngine {
private:
    Window window;
    bool shouldClose = false;
    bool isDarkMode = false;
    Theme theme;
    Renderer renderer;
    UIManager uiManager;
    DataManager dataManager;
    Playback playbackController;
    float appTime = 0.0f;
    
    int currentDS = -1; // -1: none, 0: SLL, 1: MinHeap, 2: MaxHeap, 3: AVL, 4: SPAGrid, 5: SPAMatrix, 6: SPAList

  IVisualizable *activeStructure = nullptr;
  StructureType activeStructureType = StructureType::None;

  // Maps the selected menu item to a concrete structure type.
  static StructureType mapMenuSelectionToStructureType(int selectedDS);
  // Resolves the active structure instance for the requested type.
  IVisualizable *resolveStructure(StructureType structureType, bool directed);
  // Handles requests that switch the active structure.
  void handleStructureSwitchRequest();
  // Switches the active visualizable structure.
  void switchActiveStructure(StructureType structureType, bool directed);
  // Handles graph/data algorithm actions from the UI.
  void handleDataActionRequest();

public:
  // Creates the application engine and its subsystems.
  AppEngine();
  // Releases engine-owned resources.
  ~AppEngine();

  // Runs the main application loop.
  void run();

  // Processes an incoming SFML event.
  void processInput(const sf::Event &event);
  // Advances simulation state by the given delta time.
  void update(sf::Time deltaTime);
  // Renders the current frame.
  void render();
};

#endif // APPENGINE_H