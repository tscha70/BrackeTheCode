// the absolute max of code length
const uint8_t MAX = 17;
const uint8_t CAREERMAX = 6;

struct Player {
  uint8_t MaxLevel;
  uint8_t CurrentLevel;
};

struct Game {
  bool IsOver = false;
  uint8_t Score;
  uint8_t CodeLength;
  char Sequence[MAX];
  uint8_t Levels[CAREERMAX + 1] = { 5, 7, 9, 11, 13, 15, 17 };
};

struct Star {
  uint8_t PosX;
  uint8_t PosY;
  uint8_t StartFrame;
  uint8_t *Bitmap;
};
