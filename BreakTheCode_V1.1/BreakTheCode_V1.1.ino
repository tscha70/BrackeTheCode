// >>>>>>> Break The Code Version 1.1 for ARDUBOY  GPLv3 <<<<<<
//         Programmer: (c) Roger Buehler 2024
//         Contact EMAIL: tscha70@gmail.com
//         Official repository:  https://github.com/tscha70/
//  'Break The Code' is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//  Thanks for sharing!
//  The victory song "Minuet in G – Christian Petzold" is adapted from Robson Couto, Arduino Tutorials (22.2.2019)

#include <Arduboy2.h>
#include <ArduboyTones.h>
#include <EEPROM.h>
#include "Sprites.h"
#include "Sounds.h"
#include "GameCore.h"

// define keypad
#define NONE 0
#define UP 1
#define RIGHT 2
#define DOWN 4
#define LEFT 8
#define A 16
#define B 32

#define EEPROM_MAGIC_VALUE 0xCA
#define EEPROM_MAGIC_ADDRESS (EEPROM_STORAGE_SPACE_START + 348)
#define EEPROM_ADDRESS_MAX (EEPROM_STORAGE_SPACE_START + 349)
#define EEPROM_ADDRESS_CURRENT (EEPROM_STORAGE_SPACE_START + 350)

#define MAX_TIMEOUT 4000
#define TIMEOUT_WARNING 2000
#define LEFT_OFFSET 70

Arduboy2 arduboy;
ArduboyTones sound(arduboy.audio.enabled);
Player player;
Game game;

void setup() {
  bool isTestMode = false;

  arduboy.begin();
  arduboy.setFrameRate(30);

  // pause render until it's time for the next frame
  if (!(arduboy.nextFrame()))
    return;

  arduboy.clear();


  // show intro and check for test-mode entry
  Sprites::drawOverwrite(0, 0, INTRO, 0);
  arduboy.display();
  for (int i = 0; i < 100; i++) {
    if (arduboy.pressed(B_BUTTON)) { break; }
    if (arduboy.pressed(A_BUTTON) && arduboy.pressed(LEFT_BUTTON)) {
      sound.tone(NOTE_G7, 20);
      for (int n = 0; n < 100; n++) {
        if (arduboy.pressed(B_BUTTON) && arduboy.pressed(RIGHT_BUTTON)) {
          // reset player
          sound.tone(NOTE_A7, 20);
          saveMaxPlayerLevel(0);
          saveCurrentPlayerLevel(0);
          for (int k = 0; k <= CAREERMAX; k++) {
            game.Levels[k] = 1;
          }
          player.CurrentLevel = 0;
          player.MaxLevel = 5;
          isTestMode = true;
          break;
        }
        delay(10);
      }
    }
    delay(22);
  }

  // make sure A is not presssed
  while (arduboy.pressed(A_BUTTON)) {
  }
  arduboy.clear();
  arduboy.display();

  // initialize player
  if (!isTestMode) {
    player.MaxLevel = loadMaxPlayerLevel();
    player.CurrentLevel = loadCurrtenPlayerLevel();
  }
}

void loop() {
  int duration = 250;
  LevelSelection();
  arduboy.display();
  if (arduboy.pressed(A_BUTTON)) {
    sound.tone(NOTE_F4, duration);
    while (arduboy.pressed(A_BUTTON)) {
    }
    printStartGame();
    PlayGame();
  }
}

void LevelSelection() {
  const int offset = 15;

  arduboy.clear();
  arduboy.setTextSize(1);
  arduboy.setCursor(0, 0);
  arduboy.print("Career path");

  arduboy.setCursor(0, 55);
  arduboy.print("Press (A) to start!");

  Sprites::drawOverwrite(0, offset, CAREERLEVELS, 0);
  for (int i = 0; i <= player.MaxLevel; i++) {
    Sprites::drawSelfMasked((i * 18) + 5, offset + 3, CAREERSTATUS, 0);
  }

  Sprites::drawOverwrite((player.CurrentLevel * 18) + 3, offset, CAREERSELECTION, 0);

  arduboy.setCursor(0, 35);
  arduboy.print("Code length: " + String(game.Levels[player.CurrentLevel]));
  arduboy.display();

  if (arduboy.pressed(RIGHT_BUTTON)) {
    if (player.CurrentLevel < player.MaxLevel) {
      sound.tone(NOTE_A6, 10);
      player.CurrentLevel++;
    } else {
      sound.tone(NOTE_A2, 200);
    }
    while (arduboy.pressed(RIGHT_BUTTON)) {}
  }

  if (arduboy.pressed(LEFT_BUTTON)) {
    if (player.CurrentLevel > 0) {
      sound.tone(NOTE_D6, 10);
      player.CurrentLevel--;
    } else {
      sound.tone(NOTE_A2, 200);
    }
    while (arduboy.pressed(LEFT_BUTTON)) {}
  }
}


void PlayGame() {
  game.CodeLength = game.Levels[player.CurrentLevel];
  game.IsOver = false;
  int counter = 0;
  int toneLength = 500;
  randomSeed(millis());
  for (int i = 0; i < MAX; i++) {
    game.Sequence[i] = 0x08;
  }

  // game loop -------------------------------
  while (!game.IsOver) {
    counter++;
    game.Sequence[counter - 1] = random(0, 4);
    // play the tone and wait for the user to anser
    playSequence(game.Sequence, counter, toneLength);

    // show empty
    drawDPad(0);
    arduboy.display();

    if (!isPlayBackCorrect(game.Sequence, toneLength)) {
      game.IsOver = true;
      game.Score = counter - 1;
      arduboy.clear();
      Sprites::drawOverwrite(0, 0, VAULT, 0);
      sound.tones(FAILED);
      // you loose!
      Sprites::drawOverwrite(90, 24, BURGLER, 2);
      arduboy.setCursor(LEFT_OFFSET, 5);
      arduboy.print("ouch!");
      arduboy.display();
      delay(1500);
    } else if (counter >= game.CodeLength) {
      game.Score = counter;
      sound.tones(FANFARE);
      arduboy.setCursor(LEFT_OFFSET, 5);
      arduboy.print("congrats!");
      openVault(player.CurrentLevel);

      // victory!!! - when max level is reached
      if (player.CurrentLevel == CAREERMAX)
      {
        victoryAnimation();
      }

      // increase the player's level and update the code-length
      if (player.CurrentLevel < CAREERMAX) {
        player.CurrentLevel++;
      }
      if (player.CurrentLevel > player.MaxLevel) {
        player.MaxLevel = player.CurrentLevel;
        saveMaxPlayerLevel(player.MaxLevel);
      }
      saveCurrentPlayerLevel(player.CurrentLevel);
      game.CodeLength = game.Levels[player.CurrentLevel];
      game.IsOver = true;
      delay(150);
    }
  }
}

bool isPlayBackCorrect(char seq[], int duration) {
  bool isCorrect = true;
  int index = 0;
  unsigned long starttime = millis();
  uint8_t keys = 0;

  // show it is your turn
  arduboy.clear();
  Sprites::drawOverwrite(0, 0, VAULT, 0);
  Sprites::drawOverwrite(90, 24, BURGLER, 0);
  arduboy.setCursor(LEFT_OFFSET, 5);
  arduboy.print("repeat!  ");
  arduboy.display();

  while (isCorrect && seq[index] != 0x08) {
    keys = readKeyPad();
    drawDPad(keys);
    arduboy.display();
    makeSound(keys, duration);
    uint8_t s = 1 << seq[index];

    if (keys > 0) {
      if (keys == s) {

        // reset the timeout
        starttime = millis();
        arduboy.setCursor(LEFT_OFFSET, 15);
        Sprites::drawOverwrite(90, 24, BURGLER, 0);
        arduboy.print("      ");
        arduboy.display();

        index++;
        // debounce keypad
        while (readKeyPad() > 0) {
        }

        // show the key for a given time
        delay(duration / 2);

        if (seq[index] == 0x08) {
          drawDPad(0);
          arduboy.display();
          return true;
        }
      } else {
        return false;
      }
    }

    unsigned long elapsed = millis() - starttime;

    if (elapsed > MAX_TIMEOUT) {
      return false;
    } else if (elapsed > TIMEOUT_WARNING) {
      arduboy.setCursor(LEFT_OFFSET, 15);
      Sprites::drawOverwrite(90, 24, BURGLER, 1);
      arduboy.print("hurry!");
      arduboy.display();
    }
  }

  return true;
}

void printStartGame() {
  const int intraDelay = 150;
  arduboy.clear();
  saveCurrentPlayerLevel(player.CurrentLevel);
  Sprites::drawOverwrite(90, 24, BURGLER, 0);
  sound.tones(STARTNEW);
  drawDPad(0b00001111);
  arduboy.display();
  delay(intraDelay);
  drawDPad(0);
  arduboy.display();
  delay(intraDelay);
  drawDPad(0b00001111);
  arduboy.display();
  delay(intraDelay);
  drawDPad(0);
  arduboy.display();

  delay(1000);
}



// play the current sequence
void playSequence(char seq[], int counter, int duration) {
  arduboy.clear();
  Sprites::drawOverwrite(0, 0, VAULT, 0);
  Sprites::drawOverwrite(90, 24, BURGLER, 0);
  arduboy.setCursor(LEFT_OFFSET, 5);
  arduboy.print("watch! ");
  arduboy.display();
  delay(500);

  for (int i = 0; i < counter; i++) {
    if (seq[i] == 0x08) {
      break;
    }

    playKey(seq[i], duration);
  }
}

void playKey(uint8_t index, int duration) {
  uint8_t keys = 0;
  keys |= 1 << index;
  drawDPad(keys);
  arduboy.display();
  makeSound(keys, duration);
  delay(duration);
  drawDPad(0);
  arduboy.display();
  delay(100);
}

uint8_t readKeyPad() {
  uint8_t keys = 0;
  if (arduboy.pressed(UP_BUTTON))
    keys |= 1 << 0;
  if (arduboy.pressed(RIGHT_BUTTON))
    keys |= 1 << 1;
  if (arduboy.pressed(DOWN_BUTTON))
    keys |= 1 << 2;
  if (arduboy.pressed(LEFT_BUTTON))
    keys |= 1 << 3;
  if (arduboy.pressed(A_BUTTON))
    keys |= 1 << 4;
  if (arduboy.pressed(B_BUTTON))
    keys |= 1 << 5;

  return keys;
}

void makeSound(uint8_t keys, int duration) {
  if (keys & UP)
    sound.tone(NOTE_G5, duration);
  if (keys & RIGHT)
    sound.tone(NOTE_C5, duration);
  if (keys & DOWN)
    sound.tone(NOTE_C4, duration);
  if (keys & LEFT)
    sound.tone(NOTE_G4, duration);
  if (keys & A)
    sound.tone(NOTE_F4, duration);
  if (keys & B)
    sound.tone(NOTE_B4, duration);
}

void drawDPad(byte pattern) {
  Sprites::drawOverwrite(0, 0, VAULT, 0);
  if (pattern & UP)  // up
    Sprites::drawSelfMasked(20, 13, TILE_VERTICAL, 0);
  if (pattern & RIGHT)  // right
    Sprites::drawSelfMasked(36, 20, TILE_HORIZONTAL, 1);
  if (pattern & DOWN)  // down
    Sprites::drawSelfMasked(20, 36, TILE_VERTICAL, 1);
  if (pattern & LEFT)  // left
    Sprites::drawSelfMasked(13, 20, TILE_HORIZONTAL, 0);
}

void openVault(uint8_t win) {
  int myDelay = 200;
  Sprites::drawOverwrite(90, 24, BURGLER, 4);
  for (int i = 1; i < 5; i++) {
    Sprites::drawOverwrite(0, 0, VAULT, i);
    Sprites::drawOverwrite(9, 4, WIN, (win * 3) + 1);
    drawMask(i);
    Sprites::drawSelfMasked(0, 0, VAULT, i);
    arduboy.display();
    delay(myDelay);
  }

  for (int i = 0; i < 15; i++) {
    for (int k = 1; k <= 3; k++) {
      Sprites::drawOverwrite(9, 4, WIN, (win * 3) + k);
      arduboy.display();
      delay(100);
    }
  }
  delay(500);
  Sprites::drawOverwrite(90, 24, BURGLER, 4);
  for (int i = 4; i > 0; i--) {
    Sprites::drawOverwrite(0, 0, VAULT, i);
    Sprites::drawOverwrite(9, 4, WIN, (win * 3) + 1);
    drawMask(i);
    Sprites::drawSelfMasked(0, 0, VAULT, i);
    arduboy.display();
    delay(myDelay);
  }
  Sprites::drawOverwrite(0, 0, VAULT, 0);
  arduboy.display();
  delay(500);
}

void drawMask(int m) {
  const int doormask[] = { 50, 42, 25, 9 };
  if (m < 0) return;
  if (m > 4) return;
  arduboy.fillRect(0, 0, doormask[m - 1], 64, BLACK);
}


uint8_t loadMaxPlayerLevel() {
  uint8_t value = 0;
  if (EEPROM.read(EEPROM_MAGIC_ADDRESS) == EEPROM_MAGIC_VALUE) {
    EEPROM.get(EEPROM_ADDRESS_MAX, value);
  }
  return value;
}

uint8_t loadCurrtenPlayerLevel() {
  uint8_t value = 0;
  if (EEPROM.read(EEPROM_MAGIC_ADDRESS) == EEPROM_MAGIC_VALUE) {
    EEPROM.get(EEPROM_ADDRESS_CURRENT, value);
  }
  return value;
}

void saveMaxPlayerLevel(uint8_t value) {
  EEPROM.update(EEPROM_MAGIC_ADDRESS, EEPROM_MAGIC_VALUE);
  EEPROM.put(EEPROM_ADDRESS_MAX, value);
}

void saveCurrentPlayerLevel(uint8_t value) {
  EEPROM.update(EEPROM_MAGIC_ADDRESS, EEPROM_MAGIC_VALUE);
  EEPROM.put(EEPROM_ADDRESS_CURRENT, value);
}

void victoryAnimation() {
  const int numStars = 11;
  Star star[numStars];
  Sprites::drawOverwrite(0, 0, VICTOIRE, 0);
  arduboy.display();
  sound.tones(VICTORY);

  for (int n = 0; n < 5; n++) {
    // gerate stars
    for (int m = 0; m < numStars; m++) {
      if (random(0, 8) > 4) {
        star[m] = { random(0, 128), random(64, 120), random(0, 3), (uint8_t*)STAR24 };
      } else {
        star[m] = { random(0, 128), random(64, 120), random(0, 3), (uint8_t*)STAR16 };
      }
    }
    for (int i = 0; i < 50; i++) {
      Sprites::drawOverwrite(0, 0, VICTOIRE, 0);

      for (int k = 0; k < numStars; k++) {
        Sprites::drawSelfMasked(star[k].PosX, star[k].PosY - (i * 3), star[k].Bitmap, (i + star[k].StartFrame) % 4);
      }
      arduboy.display();
      delay(100);
    }
  }
}
