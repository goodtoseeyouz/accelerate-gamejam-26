#include <iostream>
#include <random>
#include <raylib.h>
#include <vector>

struct Note {
  float duration;
  int key;
};

struct Rhythm {
  Rhythm(float duration) {
    float currentDuration = 0;
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<int> chooseKey(0, 5);
    std::uniform_int_distribution<int> chooseDuration(0, 6);
    // 0-1 eight, 2-3 quarter, 4 whole, 5 triplet

    while (currentDuration < duration - 0.1) {
      switch (chooseDuration(rng)) {
      case 0:
      case 1:
        notes.push_back(Note{0.25, chooseKey(rng)});
        break;
      case 2:
      case 3:
        if (duration - 0.1 - currentDuration < 0.5)
          break;
        notes.push_back(Note{0.50, chooseKey(rng)});
        break;
      case 4:
        if (duration - 0.1 - currentDuration < 1)
          break;
        notes.push_back(Note{1.0, chooseKey(rng)});

      case 5:
        if (duration - 0.1 - currentDuration < 1)
          break;
        notes.push_back(Note{0.33, chooseKey(rng)});
        notes.push_back(Note{0.33, chooseKey(rng)});
        notes.push_back(Note{0.34, chooseKey(rng)});
        break;
      default:
        std::cerr << "Something is wrong in the Rhythm generator" << std::endl;
        break;
      }
    }
  }

  std::vector<Note> notes;
};
