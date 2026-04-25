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
enum class ShapeType { Circle, Rectangle };

class Renderer : public IPayloadVisitor {
private:
  Window &window;
  const Theme &theme;

  sf::Font mainFont;
  sf::Texture bgTexture;
  sf::Texture nodeTexture;
  sf::Texture arrayTexture;
  std::map<size_t, sf::Vector2f> customNodePositions;
  std::map<size_t, sf::Vector2f> defaultNodePositions;
  int draggedNodeIndex = -1;
  sf::Vector2f dragOffset;
  sf::Sprite bgSprite;

public:
  Renderer(Window &m_window, const Theme &m_theme);

  // Asset lifecycle and scene background.
  bool loadAssets();
  void drawBackground();

  // Node and array element rendering.
  void drawImageNode(sf::Vector2f pos, const std::string &text,
                     bool isHighlighted = false);
  void drawArrayCell(sf::Vector2f pos, const std::string &text,
                     bool isHighlighted = false);

  // Edge/connector rendering helpers.
  sf::Vector2f getBoundaryPoint(sf::Vector2f center, sf::Vector2f size,
                                float angle, ShapeType type);
  void drawLineWithArrow(sf::Vector2f p1, sf::Vector2f size1, ShapeType type1,
                         sf::Vector2f p2, sf::Vector2f size2, ShapeType type2,
                         float thickness, float arrowSize = 12.0f,
                         bool isHighlighted = false);
  void drawLine(sf::Vector2f p1, sf::Vector2f size1, ShapeType type1,
                sf::Vector2f p2, sf::Vector2f size2, ShapeType type2,
                float thickness, bool isHighlighted = false);

  void renderActiveState(const Frame *currentFrame);

  // IPayloadVisitor overrides.
  void visit(const LinkedListPayload &payload) override;
  void visit(const TreePayload &payload) override;
  void visit(const HeapPayload &payload) override;
  void visit(const GraphPayload &payload) override;
  void visit(const SingleSourcePayload &payload) override;
  void visit(const AStarPayload &payload) override;
  void visit(const AllPairsPayload &payload) override;
  void visit(const GridPayload &payload) override;

  // Texture-based size queries.
  sf::Vector2f getNodeSize() const;  // in main.cpp have to use to access nodeW
  sf::Vector2f getArraySize() const; // in main.cpp have to use to access nodeH

  // Drag and Drop handlers
  void handleMousePress(sf::Vector2f mousePos);
  void handleMouseMove(sf::Vector2f mousePos);
  void handleMouseRelease();
  void resetCustomPositions();

  // Generic and positional text helpers.
  void drawText(sf::Vector2f pos, const std::string &text, unsigned int size,
                sf::Color color, TextPosition align = TextPosition::TopLeft,
                sf::Angle angle = sf::degrees(0.f));

  // Unified text positioning function.
  void drawTextPositioned(sf::Vector2f basePosition, const std::string &text,
                          unsigned int size, sf::Color color,
                          TextPositionMode mode = TextPositionMode::Absolute,
                          float objHeight = 0.0f, float objWidth = 0.0f,
                          float padding = 0.0f,
                          sf::Vector2f lineEnd = {0.0f, 0.0f},
                          TextPosition align = TextPosition::TopLeft,
                          sf::Angle angle = sf::degrees(0.f));

  // Convenience wrappers for common positioning modes (use drawTextPositioned
  // for more control).
  void drawTextUp(sf::Vector2f center, float objHeight, float padding,
                  const std::string &text, unsigned int size, sf::Color color);
  void drawTextDown(sf::Vector2f center, float objHeight, float padding,
                    const std::string &text, unsigned int size,
                    sf::Color color);
  void drawTextOnLine(sf::Vector2f p1, sf::Vector2f p2, float padding,
                      const std::string &text, unsigned int size,
                      sf::Color color);
  void drawTextTopLeft(sf::Vector2f center, sf::Vector2f objSize, float padding,
                       const std::string &text, unsigned int size,
                       sf::Color color);
  void drawTextTopRight(sf::Vector2f center, sf::Vector2f objSize,
                        float padding, const std::string &text,
                        unsigned int size, sf::Color color);
  void drawTextBottomLeft(sf::Vector2f center, sf::Vector2f objSize,
                          float padding, const std::string &text,
                          unsigned int size, sf::Color color);
  void drawTextBottomRight(sf::Vector2f center, sf::Vector2f objSize,
                           float padding, const std::string &text,
                           unsigned int size, sf::Color color);
};

#endif // RENDERER_H