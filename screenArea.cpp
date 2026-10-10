#include "screenArea.h"

#include <raylib.h>

ScreenArea::ScreenArea(std::string name, Rectangle relativeArea, Color color) {
  relativeArea_ = std::move(relativeArea);
  color_ = color;
  name_ = name;
}

void ScreenArea::draw() {
  if (!shouldDraw_)
    return;
  BeginScissorMode(trueArea_.x, trueArea_.y, trueArea_.width, trueArea_.height);
  draw_();
  EndScissorMode();
  for (const auto &child : children_) {
    child->draw();
  }
}

void ScreenArea::checkInput(InputData input) {
  if (!shouldDraw_)
    return;
  for (auto &child : children_) {
    child->checkInput(input);
  }
  checkInput_(input);
}

void ScreenArea::calculateTrueArea(const IntRectangle &parentTrueArea) {
  trueArea_.x = parentTrueArea.x + parentTrueArea.width * relativeArea_.x;
  trueArea_.y = parentTrueArea.y + parentTrueArea.height * relativeArea_.y;
  trueArea_.width = parentTrueArea.width * relativeArea_.width;
  trueArea_.height = parentTrueArea.height * relativeArea_.height;
  for (auto &child : children_) {
    child->calculateTrueArea(trueArea_);
  }
}

ScreenArea &ScreenArea::addChild(std::unique_ptr<ScreenArea> child) {
  children_.emplace_back(std::move(child));
  return *children_.back();
}

ScreenArea &ScreenArea::getChild(std::string childName) {
  for (auto &child : children_) {
    if (child->getName() == childName)
      return *child.get();
  }

  std::cerr << "Failed to find child " << childName << std::endl;
  std::abort();
}

const std::string &ScreenArea::getName() { return name_; }

void ScreenArea::setShouldDraw(bool state) { shouldDraw_ = state; }

void ScreenArea::checkInput_(InputData input) { return; }

void ScreenArea::draw_() {
  DrawRectangle(trueArea_.x, trueArea_.y, trueArea_.width, trueArea_.height,
                color_);
}

DynamicStringScreenArea::DynamicStringScreenArea(std::string name,
                                                 Rectangle relativeArea,
                                                 std::string *infoText,
                                                 Color color)
    : ScreenArea(name, relativeArea, color) {
  infoText_ = infoText;
}

void DynamicStringScreenArea::draw_() {
  DrawText(infoText_->c_str(), trueArea_.x, trueArea_.y, trueArea_.height,
           color_);
}

TriggerOnClickScreenArea::TriggerOnClickScreenArea(std::string name,
                                                   Rectangle relativeArea)
    : ScreenArea(name, relativeArea, {0, 0, 0, 0}) {}

void TriggerOnClickScreenArea::checkInput_(InputData input) {
  if (input.mousePressed && trueArea_.contains(input.mouseX, input.mouseY)) {
    for (auto &child : children_) {
      if (dynamic_cast<ClassFunctionScreenArea *>(child.get())) {
        auto goodChild = dynamic_cast<ClassFunctionScreenArea *>(child.get());
        goodChild->execute();
      }
    }
  }
}

TriggerOnHoverScreenArea::TriggerOnHoverScreenArea(std::string name,
                                                   Rectangle relativeArea)
    : ScreenArea(name, relativeArea, {0, 0, 0, 0}) {}

void TriggerOnHoverScreenArea::checkInput_(InputData input) {
  if (trueArea_.contains(input.mouseX, input.mouseY)) {
    for (auto &child : children_) {
      if (dynamic_cast<ClassFunctionScreenArea *>(child.get())) {
        auto goodChild = dynamic_cast<ClassFunctionScreenArea *>(child.get());
        goodChild->execute();
      }
    }
  }
}

ClassFunctionScreenArea::ClassFunctionScreenArea(
    std::string name, GameManager *obj, void (GameManager::*callback)())
    : ScreenArea(name, {0, 0, 0, 0}, {0, 0, 0, 0}), obj(obj),
      callback(callback) {}

void ClassFunctionScreenArea::execute() { (obj->*callback)(); }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void PixelGridScreenArea::setGrid(std::vector<std::vector<Color>> newGrid) {
  grid_ = std::move(newGrid);
}

void PixelGridScreenArea::draw_() {
  for (int row = 0; row < grid_.size(); row++) {
    for (int col = 0; col < grid_[row].size(); col++) {
      DrawRectangleRec(
          {static_cast<float>(trueArea_.x + static_cast<float>(col) *
                                                trueArea_.width /
                                                grid_[row].size()),
           static_cast<float>(trueArea_.y + static_cast<float>(row) *
                                                trueArea_.height /
                                                grid_.size()),
           static_cast<float>(static_cast<float>(trueArea_.width) /
                              grid_[row].size()),
           static_cast<float>(static_cast<float>(trueArea_.height) /
                              grid_.size())},
          grid_[row][col]);
    }
  }
}
