
const uint16_t FANFARE[] PROGMEM = {
  // High-pitched excitement
  NOTE_E5, 100,  // Short, high E
  NOTE_G5, 150,  // Slightly longer, higher G with a slide up
  NOTE_C5, 200,  // Land on a mid-range C

  // Celebration with a wobble
  NOTE_E5, 100,  // Short, high E again
  NOTE_D5, 150,  // Slide down to a lower D
  NOTE_E5, 200,  // Bounce back up to E

  // Triumphant finish
  NOTE_G5, 200,  // Hold a high G
  NOTE_C6, 400,  // End with a soaring high C

  TONES_END
};

#define D2 500
#define D4 250
#define D8 125
#define D_2 750

const uint16_t VICTORY[] PROGMEM = {

  // Minuet in G - Petzold
  // Score available at https://musescore.com/user/3402766/scores/1456391
  NOTE_D5, D4, NOTE_G4, D8, NOTE_A4, D8, NOTE_B4, D8, NOTE_C5, D8,  //1
  NOTE_D5, D4, NOTE_G4, D4, NOTE_G4, D4,
  NOTE_E5, D4, NOTE_C5, D8, NOTE_D5, D8, NOTE_E5, D8, NOTE_FS5, D8,
  NOTE_G5, D4, NOTE_G4, D4, NOTE_G4, D4,
  NOTE_C5, D4, NOTE_D5, D8, NOTE_C5, D8, NOTE_B4, D8, NOTE_A4, D8,

  NOTE_B4, D4, NOTE_C5, D8, NOTE_B4, D8, NOTE_A4, D8, NOTE_G4, D8,  //6
  NOTE_FS4, D4, NOTE_G4, D8, NOTE_A4, D8, NOTE_B4, D8, NOTE_G4, D8,
  NOTE_A4, D_2,
  NOTE_D5, D4, NOTE_G4, D8, NOTE_A4, D8, NOTE_B4, D8, NOTE_C5, D8,
  NOTE_D5, D4, NOTE_G4, D4, NOTE_G4, D4,
  NOTE_E5, D4, NOTE_C5, D8, NOTE_D5, D8, NOTE_E5, D8, NOTE_FS5, D8,

  NOTE_G5, D4, NOTE_G4, D4, NOTE_G4, D4,
  NOTE_C5, D4, NOTE_D5, D8, NOTE_C5, D8, NOTE_B4, D8, NOTE_A4, D8,  //12
  NOTE_B4, D4, NOTE_C5, D8, NOTE_B4, D8, NOTE_A4, D8, NOTE_G4, D8,
  NOTE_A4, D4, NOTE_B4, D8, NOTE_A4, D8, NOTE_G4, D8, NOTE_FS4, D8,
  NOTE_G4, D_2,

  //continues from 17
  NOTE_B5, D4, NOTE_G5, D8, NOTE_A5, D8, NOTE_B5, D8, NOTE_G5, D8,  //17
  NOTE_A5, D4, NOTE_D5, D8, NOTE_E5, D8, NOTE_FS5, D8, NOTE_D5, D8,
  NOTE_G5, D4, NOTE_E5, D8, NOTE_FS5, D8, NOTE_G5, D8, NOTE_D5, D8,
  NOTE_CS5, D4, NOTE_B4, D8, NOTE_CS5, D8, NOTE_A4, D4,
  NOTE_A4, D8, NOTE_B4, D8, NOTE_CS5, D8, NOTE_D5, D8, NOTE_E5, D8, NOTE_FS5, D8,

  NOTE_G5, D4, NOTE_FS5, D4, NOTE_E5, D4,  //22
  NOTE_FS5, D4, NOTE_A4, D4, NOTE_CS5, D4,
  NOTE_D5, D_2,
  NOTE_D5, D4, NOTE_G4, D8, NOTE_FS5, D8, NOTE_G4, D4,
  NOTE_E5, D4, NOTE_G4, D8, NOTE_FS4, D8, NOTE_G4, D4,
  NOTE_D5, D4, NOTE_C5, D4, NOTE_B4, D4,

  NOTE_A4, D8, NOTE_G4, D8, NOTE_FS4, D8, NOTE_G4, D8, NOTE_A4, D4,  //28
  NOTE_D4, D8, NOTE_E4, D8, NOTE_FS4, D8, NOTE_G4, D8, NOTE_A4, D8, NOTE_B4, D8,
  NOTE_C5, D4, NOTE_B4, D4, NOTE_A4, D4,
  NOTE_B4, D8, NOTE_D5, D8, NOTE_G4, D4, NOTE_FS4, D4,
  NOTE_G4, D_2,

  TONES_END
};


const uint16_t FAILED[] PROGMEM = {
  NOTE_G3, 150,
  NOTE_D3, 100,
  NOTE_F3, 100,
  NOTE_F2, 150,
  NOTE_B2, 100,
  NOTE_B0, 550,

  TONES_END
};

const uint16_t STARTNEW[] PROGMEM = {
  NOTE_B5, 100,
  NOTE_F4, 100,
  NOTE_F6, 100,

  TONES_END
};
