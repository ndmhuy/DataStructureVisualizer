#ifndef RENDERER_H
#define RENDERER_H

#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <map>
#include <string>

#include "Model/Frame.h"
#include "Model/Payloads/IPayloadVisitor.h"
#include "Model/Timeline.h"
#include "Utilities/StructureType.h"
#include "View/Core/Theme.h"
#include "View/Core/Window.h"

enum class TextPosition {
    TopLeft,
    TopRight,
    BottomLeft,
    BottomRight,
    Top,
    Bottom,
};
enum class TextPositionMode {
    Absolute,   // Direct position with alignment
    Up,         // Above object (requires objHeight, padding)
    Down,       // Below object (requires objHeight, padding)
    OnLine,     // Along a line (requires lineEnd, padding)
    TopLeft,    // Corner position (requires objSize, padding)
    TopRight,   // Corner position (requires objSize, padding)
    BottomLeft, // Corner position (requires objSize, padding)
    BottomRight // Corner position (requires objSize, padding)
};

struct NodeAnimState {
    sf::Vector2f pos;
    float scale = 0.0f;
    float highlightAlpha = 0.0f;
    sf::Vector2f velPos = {0.0f, 0.0f};
    float velScale = 0.0f;
    float colorR = -1.0f; // -1 signifies uninitialized
    float colorG = 0.0f;
    float colorB = 0.0f;
};
enum class ShapeType { Circle, Rectangle };

class Renderer : public IPayloadVisitor {
private:
    Window& window;
    const Theme& theme;

    sf::Font mainFont;
    sf::Texture bgTexture;
    sf::Texture nodeTexture;
    sf::Texture arrayTexture;
    std::map<size_t, sf::Vector2f> customNodePositions;
    std::map<size_t, sf::Vector2f> defaultNodePositions; 
    std::map<size_t, std::vector<size_t>> currentChildren;
    int draggedNodeIndex = -1;
    bool hasMovedDuringDrag = false;
    std::map<size_t, sf::Vector2f> preDragPositions;
    std::vector<std::map<size_t, sf::Vector2f>> positionHistory;
    sf::Vector2f dragOffset;
    sf::Sprite bgSprite;

    std::map<size_t, NodeAnimState> nodeAnimStates;
    float currentDeltaTime = 0.0f;
    float appTime = 0.0f;

public:
    // Creates a renderer bound to the given window and theme.
    Renderer(Window& m_window, const Theme& m_theme);

    // Updates animation state using the elapsed delta time.
    void updateAnimations(float dt);
    // Resets all animation state to defaults.
    void resetAnimations();

    // Asset lifecycle and scene background.
    // Loads textures, fonts, and other assets required by the renderer.
    bool loadAssets();
    // Reloads the background sprite.
    void reloadBackground();
    // Draws the current background.
    void drawBackground();

    // Node and array element rendering.
    // Draws a node image with optional text and pulse effect.
    void drawImageNode(sf::Vector2f pos, const std::string& text, float scaleMultiplier, sf::Color nodeColor, float pulseAlpha = 0.0f);
    // Draws an array cell with optional text and pulse effect.
    void drawArrayCell(sf::Vector2f pos, const std::string& text, float scaleMultiplier, sf::Color cellColor, float pulseAlpha = 0.0f);

    // Edge/connector rendering helpers.
    // Returns the boundary point of a shape for connector rendering.
    sf::Vector2f getBoundaryPoint(sf::Vector2f center, sf::Vector2f size,
                                float angle, ShapeType type);
    // Draws a directed connector between two shapes.
    void drawLineWithArrow(sf::Vector2f p1, sf::Vector2f size1, ShapeType type1,
                         sf::Vector2f p2, sf::Vector2f size2, ShapeType type2,
                         float thickness, float arrowSize = 12.0f,
                         bool isHighlighted = false, float offset = 0.0f);
    // Draws an undirected connector between two shapes.
    void drawLine(sf::Vector2f p1, sf::Vector2f size1, ShapeType type1,
                sf::Vector2f p2, sf::Vector2f size2, ShapeType type2,
                float thickness, bool isHighlighted = false, float offset = 0.0f);

    // Renders the current frame payload.
    void renderActiveState(const Frame* currentFrame);

    // IPayloadVisitor overrides.
    // Visits a linked-list payload.
    void visit(const LinkedListPayload& payload) override;
    // Visits a tree payload.
    void visit(const TreePayload& payload) override;
    // Visits a heap payload.
    void visit(const HeapPayload& payload) override;
    // Visits a graph payload.
    void visit(const GraphPayload& payload) override;
    // Visits a single-source payload.
    void visit(const SingleSourcePayload& payload) override;
    // Visits an A* payload.
    void visit(const AStarPayload& payload) override;
    // Visits an all-pairs payload.
    void visit(const AllPairsPayload& payload) override;
    // Visits a grid payload.
    void visit(const GridPayload& payload) override;
    // Visits a menu animation payload.
    void visit(const MenuAnimPayload& payload) override;
    // Visits a decoration payload.
    void visit(const DecorationPayload& payload) override;
    // Visits a top-bar payload.
    void visit(const TopBarPayload& payload) override;

    // Texture-based size queries.
    // Returns the on-screen node size.
    sf::Vector2f getNodeSize() const;  // in main.cpp have to use to access nodeW
    // Returns the on-screen array cell size.
    sf::Vector2f getArraySize() const; // in main.cpp have to use to access nodeH

    // Drag and Drop handlers
    // Handles a mouse press for node dragging.
    bool handleMousePress(sf::Vector2f mousePos);
    // Handles mouse movement while dragging.
    void handleMouseMove(sf::Vector2f mousePos);
    // Handles release of the dragged node.
    bool handleMouseRelease();
    // Resets all custom node positions.
    void resetCustomPositions();
    // Restores the last drag state.
    void undoLastDrag();

    // Generic and positional text helpers.
    // Draws text at a positioned anchor.
    void drawText(sf::Vector2f pos, const std::string& text, unsigned int size,
                sf::Color color, TextPosition align = TextPosition::TopLeft,
                sf::Angle angle = sf::degrees(0.f));

    // Unified text positioning function.
    // Draws text using one of the supported positioning modes.
    void drawTextPositioned(sf::Vector2f basePosition, const std::string& text,
                          unsigned int size, sf::Color color,
                          TextPositionMode mode = TextPositionMode::Absolute,
                          float objHeight = 0.0f, float objWidth = 0.0f,
                          float padding = 0.0f,
                          sf::Vector2f lineEnd = {0.0f, 0.0f},
                          TextPosition align = TextPosition::TopLeft,
                          sf::Angle angle = sf::degrees(0.f));

    // Convenience wrappers for common positioning modes (use drawTextPositioned
    // for more control).
    // Draws text above a target position.
    void drawTextUp(sf::Vector2f center, float objHeight, float padding,
                  const std::string& text, unsigned int size, sf::Color color);
    // Draws text below a target position.
    void drawTextDown(sf::Vector2f center, float objHeight, float padding,
                    const std::string& text, unsigned int size,
                    sf::Color color);
    // Draws text along a line segment.
    void drawTextOnLine(sf::Vector2f p1, sf::Vector2f p2, float padding,
                      const std::string& text, unsigned int size,
                      sf::Color color);
    // Draws text near the top-left corner of an object.
    void drawTextTopLeft(sf::Vector2f center, sf::Vector2f objSize, float padding,
                       const std::string& text, unsigned int size,
                       sf::Color color);
    // Draws text near the top-right corner of an object.
    void drawTextTopRight(sf::Vector2f center, sf::Vector2f objSize,
                        float padding, const std::string& text,
                        unsigned int size, sf::Color color);
    // Draws text near the bottom-left corner of an object.
    void drawTextBottomLeft(sf::Vector2f center, sf::Vector2f objSize,
                          float padding, const std::string& text,
                          unsigned int size, sf::Color color);
    // Draws text near the bottom-right corner of an object.
    void drawTextBottomRight(sf::Vector2f center, sf::Vector2f objSize,
                           float padding, const std::string& text,
                           unsigned int size, sf::Color color);
};

#endif // RENDERER_H