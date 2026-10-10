#include "raylib.h"
#include "screenArea.h"
#include <cmath>

#define NO_COLOR {0, 0, 0, 0}

void playNote(Sound &sound, int semitones) {
  SetSoundPitch(sound, powf(2.0f, semitones / 12.0f));
  PlaySound(sound);
}

class GameManager {
public:
  std::string titleText = "MUSIC";
  std::string titleText2 = "MAYHEM";
  std::string titleText3 = "made by bradley yang";

  std::string playButtonText = "PLAY?";
  std::string quitButtonText = "QUIT?";
  std::vector<ScreenArea *> bases;

  bool keepLooping = true;

  void toggleMenuGame() {
    bases[0]->setShouldDraw(!bases[0]->shouldDraw_);
    bases[1]->setShouldDraw(!bases[1]->shouldDraw_);
  }
  void stopKeepLooping() { keepLooping = false; }
};

int main() {
  const int screenWidth = 1600;
  const int screenHeight = 900;

  InitWindow(screenWidth, screenHeight, "music mayhem");
  InitAudioDevice();
  Sound piano = LoadSound("piano_C_major.wav");

  SetTargetFPS(60);
  /*
    ScreenArea(std::string name, Rectangle relativeArea,
               Color color = {0, 0, 0, 0});*/
  GameManager gm;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // MENU TEXT

  ScreenArea menu{"main menu", Rectangle{0, 0, 1, 1}, DARKBLUE};
  gm.bases.push_back(&menu);

  menu.addChild(std::make_unique<DynamicStringScreenArea>(
      "main text", Rectangle{0.40f, 0.2f, 1, 0.2f}, &gm.titleText, BLUE));

  menu.addChild(std::make_unique<DynamicStringScreenArea>(
      "main text lower", Rectangle{0.40f, 0.4f, 1, 0.2f}, &gm.titleText2,
      BLUE));

  menu.addChild(std::make_unique<DynamicStringScreenArea>(
      "main text even lower", Rectangle{0.40f, 0.6f, 1, 0.05f}, &gm.titleText3,
      BLUE));

  menu.addChild(std::make_unique<PixelGridScreenArea>(
      "note image", Rectangle{0.8, 0.2, 0.1, 0.2}));
  dynamic_cast<PixelGridScreenArea &>(menu.getChild("note image"))
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

  // MENU TEXT
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // BUTTONS

  menu.addChild(std::make_unique<PixelGridScreenArea>(
                    "play button", Rectangle{0.1, 0.3, 0.2, 0.1}))
      .addChild(std::make_unique<TriggerOnClickScreenArea>(
          "play button trigger", Rectangle{0, 0, 1, 1}))
      .addChild(std::make_unique<ClassFunctionScreenArea>(
          "play button function", &gm, &GameManager::toggleMenuGame));

  menu.getChild("play button")
      .addChild(std::make_unique<DynamicStringScreenArea>(
          "play button text", Rectangle{0.18, 0.2, 1, 0.7}, &gm.playButtonText,
          BLUE));
  dynamic_cast<PixelGridScreenArea &>(menu.getChild("play button"))
      .setGrid({{BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE},
                {BLUE, NO_COLOR, NO_COLOR, NO_COLOR, NO_COLOR, NO_COLOR,
                 NO_COLOR, NO_COLOR, NO_COLOR, BLUE},
                {BLUE, NO_COLOR, NO_COLOR, NO_COLOR, NO_COLOR, NO_COLOR,
                 NO_COLOR, NO_COLOR, NO_COLOR, BLUE},
                {BLUE, NO_COLOR, NO_COLOR, NO_COLOR, NO_COLOR, NO_COLOR,
                 NO_COLOR, NO_COLOR, NO_COLOR, BLUE},
                {BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE}});

  menu.addChild(std::make_unique<PixelGridScreenArea>(
                    "exit button", Rectangle{0.1, 0.45, 0.2, 0.1}))
      .addChild(std::make_unique<TriggerOnClickScreenArea>(
          "exit button trigger", Rectangle{0, 0, 1, 1}))
      .addChild(std::make_unique<ClassFunctionScreenArea>(
          "exit button function", &gm, &GameManager::stopKeepLooping));

  menu.getChild("exit button")
      .addChild(std::make_unique<DynamicStringScreenArea>(
          "exit button text", Rectangle{0.18, 0.2, 1, 0.7}, &gm.quitButtonText,
          BLUE));
  dynamic_cast<PixelGridScreenArea &>(menu.getChild("exit button"))
      .setGrid({{BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE},
                {BLUE, NO_COLOR, NO_COLOR, NO_COLOR, NO_COLOR, NO_COLOR,
                 NO_COLOR, NO_COLOR, NO_COLOR, BLUE},
                {BLUE, NO_COLOR, NO_COLOR, NO_COLOR, NO_COLOR, NO_COLOR,
                 NO_COLOR, NO_COLOR, NO_COLOR, BLUE},
                {BLUE, NO_COLOR, NO_COLOR, NO_COLOR, NO_COLOR, NO_COLOR,
                 NO_COLOR, NO_COLOR, NO_COLOR, BLUE},
                {BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE}});
  // BUTTONS
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // MENU SCREEN ENDS HERE | MENU SCREEN ENDS HERE | MENU SCREEN ENDS HERE
  // ##############################################################################
  // GAME SCREEN STARTS HERE | GAME SCREEN STARTS HERE | GAME SCREEN STARTS HERE

  ScreenArea game{"game screen", Rectangle{0, 0, 1, 1}, DARKGREEN};
  game.setShouldDraw(false);
  gm.bases.push_back(&game);

  game.addChild(std::make_unique<ScreenArea>("bottom text area",
                                             Rectangle{0, 0.9, 1, 0.1}, GREEN));

  // GAME SCREEN ENDS HERE | GAME SCREEN ENDS HERE | GAME SCREEN ENDS HERE
  // ##############################################################################
  // SETTINGS SCREEN STARTS HERE | SETTINGS SCREEN STARTS HERE

  ScreenArea settings{"settings screen", Rectangle{0, 0, 1, 1},
                      Color{255, 255, 255, 100}};
  settings.setShouldDraw(false);
  gm.bases.push_back(&settings);

  settings
      .addChild(std::make_unique<ScreenArea>(
          "solid background", Rectangle{0.3, 0.2, 0.4, 0.6}, ORANGE))
      .addChild(std::make_unique<ScreenArea>(
          "exit button 2", Rectangle{0.1, 0.1, 0.8, 0.3}, YELLOW))
      .addChild(std::make_unique<TriggerOnClickScreenArea>(
          "exit button 2 trigger", Rectangle{0, 0, 1, 1}))
      .addChild(std::make_unique<ClassFunctionScreenArea>(
          "exit button 2 function", &gm, &GameManager::stopKeepLooping));

  settings.getChild("solid background")
      .getChild("exit button 2")
      .addChild(std::make_unique<DynamicStringScreenArea>(
          "exit button 2 text", Rectangle{0.15, 0.2, 1, 0.7},
          &gm.quitButtonText, BLACK));

  // SETTINGS SCREEN ENDS HERE | SETTINGS SCREEN ENDS HERE
  // ##############################################################################

  /*
  struct InputData {
    bool mouseDown = false;
    bool mousePressed = false;
    int mouseX;
    int mouseY;
  };
  */
  menu.calculateTrueArea({0, 0, screenWidth, screenHeight});
  game.calculateTrueArea({0, 0, screenWidth, screenHeight});
  settings.calculateTrueArea({0, 0, screenWidth, screenHeight});

  while (gm.keepLooping) {
    BeginDrawing();
    ClearBackground(BLACK);
    menu.draw();
    game.draw();
    settings.draw();
    EndDrawing();
    InputData innn{IsMouseButtonPressed(MOUSE_BUTTON_LEFT),
                   IsMouseButtonPressed(MOUSE_BUTTON_LEFT), GetMouseX(),
                   GetMouseY()};
    menu.checkInput(innn);
    settings.checkInput(innn);
    if (IsKeyPressed(KEY_ESCAPE)) {
      gm.bases[2]->setShouldDraw(!gm.bases[2]->shouldDraw_);
    }
    if (gm.bases[1]->shouldDraw_) {
      if (IsKeyPressed(KEY_S))
        playNote(piano, -6);
      if (IsKeyPressed(KEY_D))
        playNote(piano, -4);
      if (IsKeyPressed(KEY_F))
        playNote(piano, -2);
      if (IsKeyPressed(KEY_J))
        playNote(piano, 1);
      if (IsKeyPressed(KEY_K))
        playNote(piano, 3);
      if (IsKeyPressed(KEY_L))
        playNote(piano, 6);
    }
  }
}
