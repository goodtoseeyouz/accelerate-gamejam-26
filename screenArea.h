#pragma once

// #include "gameManager.h"

#include <cassert>
#include <iostream>
#include <memory>
#include <raylib.h>
#include <string>
#include <vector>

class GameManager;

struct IntRectangle {
  int x = 0;
  int y = 0;
  int width = 0;
  int height = 0;

  bool contains(int x1, int y1) {
    if (!(x1 > x && x1 < x + width))
      return false;
    if (!(y1 > y && y1 < y + height))
      return false;
    return true;
  }
};

struct InputData {
  bool mouseDown = false;
  bool mousePressed = false;
  int mouseX;
  int mouseY;
};

struct InputEventFromUI {};

// struct ExecuteData {};

class ScreenArea {
public:
  ScreenArea(std::string name, Rectangle relativeArea,
             Color color = {0, 0, 0, 0});
  virtual ~ScreenArea() = default;
  void draw();
  void checkInput(InputData input);

  void calculateTrueArea(const IntRectangle &parentTrueArea);
  ScreenArea &addChild(std::unique_ptr<ScreenArea> child);
  ScreenArea &getChild(std::string childName);
  const std::string &getName();
  void setShouldDraw(bool state);
  virtual bool shouldDraw() { return shouldDraw_; }

  // protected:
  virtual void checkInput_(InputData input);
  virtual void draw_();
  Rectangle relativeArea_{};
  IntRectangle trueArea_{};
  Color color_;
  std::vector<std::unique_ptr<ScreenArea>> children_;
  std::string name_;
  bool shouldDraw_ = true;
};

/// The string provided to this class must NOT move
/// or idk what will happen
class DynamicStringScreenArea : public ScreenArea {
public:
  DynamicStringScreenArea(std::string name, Rectangle relativeArea,
                          std::string *infoText, Color color = {0, 0, 0, 0});

  // protected:
  std::string *infoText_;
  void draw_() override;
};

class TriggerOnClickScreenArea : public ScreenArea {
public:
  TriggerOnClickScreenArea(std::string name, Rectangle relativeArea);

  void checkInput_(InputData input) override;
};

class TriggerOnHoverScreenArea : public ScreenArea {
public:
  TriggerOnHoverScreenArea(std::string name, Rectangle relativeArea);

  void checkInput_(InputData input) override;
};

class ClassFunctionScreenArea : public ScreenArea {
public:
  ClassFunctionScreenArea(std::string name, GameManager *obj,
                          void (GameManager::*callback)());

  void execute();
  // protected:
  GameManager *obj;
  void (GameManager::*callback)();
};

class PixelGridScreenArea : public ScreenArea {
public:
  PixelGridScreenArea(std::string name, Rectangle relativeArea)
      : ScreenArea(name, relativeArea) {}
  /*
    Color &getCell(int x, int y) {
      assert(y < grid_.size() && " PixelGridScreenArearow column count error ");
      assert(x < grid_[0].size() && " PixelGridScreenArearow count error ");
      return grid_[y][x];
    }*/

  void setGrid(std::vector<std::vector<Color>> newGrid);

  // protected:
  std::vector<std::vector<Color>> grid_;

  void draw_() override;
};

class ToggleableScreenArea : public ScreenArea {
public:
  ToggleableScreenArea(std::string name, Rectangle relativeArea,
                       bool &shouldDraww, Color color = RED)
      : ScreenArea(name, relativeArea, color), shouldDraww(shouldDraww) {}
  bool shouldDraw() override { return shouldDraww; }

  bool &shouldDraww;
};
