#include "raylib.h"
#include "rhythm.cpp"
#include "screenArea.h"
#include <cmath>
#include <deque>
#include <memory>

#define NO_COLOR {0, 0, 0, 0}

void playNote(Sound &sound, int semitones) {
  SetSoundPitch(sound, powf(2.0f, semitones / 12.0f));
  PlaySound(sound);
}

// used in game scene rendering
float getMidPosition(int lane) {
  return ((0.01 + lane * 0.165) + (0.01 + (lane + 1) * 0.165)) / 2;
}

class GameManager {
public:
  std::string titleText = "MUSIC";
  std::string titleText2 = "MAYHEM";
  std::string titleText3 = "made by bradley yang";

  std::string playButtonText = "PLAY?";
  std::string quitButtonText = "QUIT?";
  std::string resetButtonText = "RESET";
  std::vector<ScreenArea *> bases;

  std::string textS = "S";
  std::string textD = "D";
  std::string textF = "F";
  std::string textJ = "J";
  std::string textK = "K";
  std::string textL = "L";

  std::string streak = "0";
  std::string multiplierString = "x1.00";
  float multiplier = 0.8;

  bool reset = false;

  bool keepLooping = true;
  std::deque<bool> notes_pressed;

  void toggleMenuGame() {
    bases[0]->setShouldDraw(!bases[0]->shouldDraw_);
    bases[1]->setShouldDraw(!bases[1]->shouldDraw_);
  }
  void stopKeepLooping() { keepLooping = false; }
  void resetGameState() {
    streak = "0";
    multiplierString = "x1.00";
    multiplier = 1.0;
    reset = true;
  }
};

int main() {
  const int screenWidth = 1600;
  const int screenHeight = 900;

  InitWindow(screenWidth, screenHeight, "music mayhem");
  InitAudioDevice();
  Sound piano = LoadSound("piano_C_major.wav");
  // Sound piano = LoadSound("lo-fi-nu-snare-1_D_major.wav");

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

  ScreenArea game{"game screen", Rectangle{0.2, 0, 0.8, 1}, DARKGREEN};
  game.setShouldDraw(false);
  gm.bases.push_back(&game);

  game.addChild(std::make_unique<ScreenArea>("bottom text area",
                                             Rectangle{0, 0.9, 1, 0.1}, GREEN));
  ScreenArea &bottomArea = game.getChild("bottom text area");

  // DIVIDERS
  for (int i = 0; i < 7; i++) {
    game.addChild(std::make_unique<ScreenArea>(
        "divider " + std::to_string(i), Rectangle{0.165f * i, 0, 0.01, 0.9},
        GREEN));
  }

  // LETTERS
  float keyWidth = 0.05;
  bottomArea.addChild(std::make_unique<DynamicStringScreenArea>(
      "s key", Rectangle{getMidPosition(0) - keyWidth / 2, 0, keyWidth, 1},
      &gm.textS, BLACK));
  bottomArea.addChild(std::make_unique<DynamicStringScreenArea>(
      "d key", Rectangle{getMidPosition(1) - keyWidth / 2, 0, keyWidth, 1},
      &gm.textD, BLACK));
  bottomArea.addChild(std::make_unique<DynamicStringScreenArea>(
      "f key", Rectangle{getMidPosition(2) - keyWidth / 2, 0, keyWidth, 1},
      &gm.textF, BLACK));
  bottomArea.addChild(std::make_unique<DynamicStringScreenArea>(
      "j key", Rectangle{getMidPosition(3) - keyWidth / 2, 0, keyWidth, 1},
      &gm.textJ, BLACK));
  bottomArea.addChild(std::make_unique<DynamicStringScreenArea>(
      "k key", Rectangle{getMidPosition(4) - keyWidth / 2, 0, keyWidth, 1},
      &gm.textK, BLACK));
  bottomArea.addChild(std::make_unique<DynamicStringScreenArea>(
      "l key", Rectangle{getMidPosition(5) - keyWidth / 2, 0, keyWidth, 1},
      &gm.textL, BLACK));

  // ON PRESS COLORS
  std::string temp[] = {"S", "D", "F", "J", "K", "L"};
  for (int i = 0; i < 6; i++) {
    game.addChild(std::make_unique<ScreenArea>(
        temp[i], Rectangle{0.02f + 0.165f * i, 0, 0.135, 1},
        Color{0, 0, 0, 50}));
  }

  game.addChild(std::make_unique<ScreenArea>("top area",
                                             Rectangle{0, 0, 1, 0.9}, Color()));

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

  settings.getChild("solid background")
      .addChild(std::make_unique<ScreenArea>(
          "reset button", Rectangle{0.1, 0.45, 0.8, 0.3}, YELLOW))
      .addChild(std::make_unique<TriggerOnClickScreenArea>(
          "reset button trigger", Rectangle{0, 0, 1, 1}))
      .addChild(std::make_unique<ClassFunctionScreenArea>(
          "reset button function", &gm, &GameManager::resetGameState));

  settings.getChild("solid background")
      .getChild("reset button")
      .addChild(std::make_unique<DynamicStringScreenArea>(
          "reset button text", Rectangle{0.15, 0.2, 1, 0.7},
          &gm.resetButtonText, BLACK));

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

  game.addChild(std::make_unique<ScreenArea>(
                    "info text area", Rectangle{-0.25, 0, 0.25, 1}, DARKGRAY))
      .addChild(std::make_unique<DynamicStringScreenArea>(
          "streak", Rectangle{0.1, 0.1, 1, 0.1}, &gm.streak, Color(BLACK)));
  game.getChild("info text area")
      .addChild(std::make_unique<DynamicStringScreenArea>(
          "multiplier", Rectangle{0.1, 0.3, 1, 0.1}, &gm.multiplierString,
          Color(BLACK)));

  while (gm.keepLooping) {

    float currentTime = 1000000000;
    gm.multiplier = 0.8;
    Rhythm rhythm(100);
    std::cout << "A";
    game.getChild("top area")
        .addChild(std::make_unique<ScreenArea>("sliding", Rectangle{0, 0, 0, 0},
                                               Color(BLUE)));
    std::cout << "B";

    const int secondsPerRhythm = 10;
    const int wholeNotesPerSecond = 10 / secondsPerRhythm;

    ScreenArea failScreen("failscreen", Rectangle{0, 0, 1, 1}, RED);
    failScreen.calculateTrueArea({0, 0, screenWidth, screenHeight});

    while (gm.keepLooping && !gm.reset) {
      if (game.shouldDraw_ && !settings.shouldDraw_) {
        if (currentTime >= secondsPerRhythm) {
          gm.notes_pressed.erase(gm.notes_pressed.begin(),
                                 gm.notes_pressed.end());
          game.getChild("top area").children_.pop_back();
          currentTime = -2;
          gm.multiplier += 0.2;
          gm.multiplierString =
              "x" + std::to_string(gm.multiplier).substr(0, 4);
          rhythm = Rhythm(10);
          std::cout << "C";
          ScreenArea &slider =
              game.getChild("top area")
                  .addChild(std::make_unique<ScreenArea>(
                      "sliding", Rectangle{0, -10, 1, 10}, Color(NO_COLOR)));

          std::cout << "D";
          float place = 0;
          for (int i = 0; i < rhythm.notes.size(); i++) {
            gm.notes_pressed.push_back(false);
            slider
                .addChild(std::make_unique<ScreenArea>(
                    "note " + std::to_string(i),
                    Rectangle{0.02f + 0.165f * rhythm.notes[i].key,
                              1.0f - (place + rhythm.notes[i].duration) /
                                         rhythm.duration,
                              0.135f,
                              rhythm.notes[i].duration / rhythm.duration -
                                  0.001f},
                    BLUE))
                .addChild(std::make_unique<ToggleableScreenArea>(
                    "clicked " + std::to_string(i),
                    Rectangle{
                        0,
                        0,
                        1,
                        1,
                    },
                    gm.notes_pressed[i], Color(DARKBLUE)));
            std::cout << 20 - place / 10 << " ";
            place += rhythm.notes[i].duration;
          }
          std::cout << "E" << std::endl;
        }

        currentTime += GetFrameTime() * gm.multiplier;
        game.getChild("top area").getChild("sliding").relativeArea_.y +=
            GetFrameTime() * gm.multiplier;
        game.calculateTrueArea({0, 0, screenWidth, screenHeight});
      }
      // std::cout << game.getChild("top
      // area").getChild("sliding").relativeArea_.y;
      BeginDrawing();
      ClearBackground(BLACK);
      menu.draw();
      game.draw();
      failScreen.draw();
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

      const int keys[] = {KEY_S, KEY_D, KEY_F, KEY_J, KEY_K, KEY_L};
      failScreen.setShouldDraw(false);

      if (game.shouldDraw_ && !settings.shouldDraw_) {
        ScreenArea &topArea = game.getChild("top area");
        ScreenArea &slider = topArea.getChild("sliding");

        float currentHitHeight = topArea.trueArea_.y + topArea.trueArea_.height;

        for (int lane = 0; lane < 6; lane++) {
          if (!IsKeyPressed(keys[lane]))
            continue;

          bool validPress = false;
          for (int i = 0; i < rhythm.notes.size(); i++) {
            if (rhythm.notes[i].key != lane || gm.notes_pressed[i])
              continue;

            IntRectangle currentNote =
                slider.getChild("note " + std::to_string(i)).trueArea_;

            if (currentNote.y <= currentHitHeight &&
                currentNote.y + currentNote.height >= currentHitHeight) {
              validPress = true;
              gm.notes_pressed[i] = true;
              gm.streak = std::to_string(std::stoi(gm.streak) + 1);
              break;
            }
          }
          if (!validPress) {
            gm.streak = "0";
            failScreen.setShouldDraw(true);
          }
        }
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

      for (int key : {KEY_S, KEY_D, KEY_F, KEY_J, KEY_K, KEY_L}) {
        gm.bases[1]
            ->getChild(std::string(1, static_cast<char>(key)))
            .setShouldDraw(IsKeyDown(key));
      }
    }
    if (gm.reset) {
      gm.keepLooping = true;
      game.getChild("top area").children_.pop_back();
    }
    gm.reset = false;
  }
}
