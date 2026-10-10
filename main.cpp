#include "raylib.h"
#include "screenArea.h"

#define NO_COLOR {0, 0, 0, 0}

class GameManager {
public:
  std::string titleText = "MUSIC";
  std::string titleText2 = "MAYHEM";
  std::string titleText3 = "made by bradley yang";

  std::string playButtonText = "PLAY?";

  void addMeowToTitleText3() { titleText3 = titleText3 + "meow"; }
};

int main() {
  const int screenWidth = 1600;
  const int screenHeight = 900;

  InitWindow(screenWidth, screenHeight, "music mayhem");

  SetTargetFPS(60);
  /*
    ScreenArea(std::string name, Rectangle relativeArea,
               Color color = {0, 0, 0, 0});*/
  GameManager gm;
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // MENU TEXT
  ScreenArea base1{"main menu", Rectangle{0, 0, 1, 1}, DARKBLUE};
  base1.addChild(std::make_unique<DynamicStringScreenArea>(
      "main text", Rectangle{0.40f, 0.2f, 1, 0.2f}, &gm.titleText, BLUE));

  base1.addChild(std::make_unique<DynamicStringScreenArea>(
      "main text lower", Rectangle{0.40f, 0.4f, 1, 0.2f}, &gm.titleText2,
      BLUE));

  base1.addChild(std::make_unique<DynamicStringScreenArea>(
      "main text even lower", Rectangle{0.40f, 0.6f, 1, 0.05f}, &gm.titleText3,
      BLUE));

  base1.addChild(std::make_unique<PixelGridScreenArea>(
      "note image", Rectangle{0.8, 0.2, 0.1, 0.2}));
  dynamic_cast<PixelGridScreenArea &>(base1.getChild("note image"))
      .setGrid(
          {{NO_COLOR, NO_COLOR, NO_COLOR, NO_COLOR, NO_COLOR, NO_COLOR,
            NO_COLOR, NO_COLOR},
           {NO_COLOR, NO_COLOR, NO_COLOR, NO_COLOR, BLUE, BLUE, NO_COLOR,
            NO_COLOR},
           {NO_COLOR, NO_COLOR, NO_COLOR, NO_COLOR, BLUE, NO_COLOR, BLUE,
            NO_COLOR},
           {NO_COLOR, NO_COLOR, NO_COLOR, NO_COLOR, BLUE, NO_COLOR, NO_COLOR,
            NO_COLOR},
           {NO_COLOR, NO_COLOR, BLUE, BLUE, BLUE, NO_COLOR, NO_COLOR, NO_COLOR},
           {NO_COLOR, BLUE, BLUE, BLUE, BLUE, NO_COLOR, NO_COLOR, NO_COLOR},
           {NO_COLOR, NO_COLOR, BLUE, BLUE, NO_COLOR, NO_COLOR, NO_COLOR,
            NO_COLOR},
           {NO_COLOR, NO_COLOR, NO_COLOR, NO_COLOR, NO_COLOR, NO_COLOR,
            NO_COLOR, NO_COLOR}});

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // BUTTONS
  base1
      .addChild(std::make_unique<PixelGridScreenArea>(
          "play button", Rectangle{0.1, 0.2, 0.2, 0.1}))
      .addChild(std::make_unique<TriggerOnClickScreenArea>(
          "play button trigger", Rectangle{0, 0, 1, 1}))
      .addChild(std::make_unique<ClassFunctionScreenArea>(
          "play button function", &gm, &GameManager::addMeowToTitleText3));

  base1.getChild("play button")
      .addChild(std::make_unique<DynamicStringScreenArea>(
          "play button text", Rectangle{0.18, 0.2, 1, 0.7}, &gm.playButtonText,
          BLUE));
  dynamic_cast<PixelGridScreenArea &>(base1.getChild("play button"))
      .setGrid({{BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE},
                {BLUE, NO_COLOR, NO_COLOR, NO_COLOR, NO_COLOR, NO_COLOR,
                 NO_COLOR, NO_COLOR, NO_COLOR, BLUE},
                {BLUE, NO_COLOR, NO_COLOR, NO_COLOR, NO_COLOR, NO_COLOR,
                 NO_COLOR, NO_COLOR, NO_COLOR, BLUE},
                {BLUE, NO_COLOR, NO_COLOR, NO_COLOR, NO_COLOR, NO_COLOR,
                 NO_COLOR, NO_COLOR, NO_COLOR, BLUE},
                {BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE}});
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  base1.calculateTrueArea({0, 0, screenWidth, screenHeight});

  /*
  struct InputData {
    bool mouseDown = false;
    bool mousePressed = false;
    int mouseX;
    int mouseY;
  };
  */

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLACK);
    base1.draw();
    EndDrawing();
    base1.checkInput({IsMouseButtonPressed(MOUSE_BUTTON_LEFT),
                      IsMouseButtonPressed(MOUSE_BUTTON_LEFT), GetMouseX(),
                      GetMouseY()});
    std::cout << IsMouseButtonPressed(MOUSE_BUTTON_LEFT) << std::endl;
  }
}
