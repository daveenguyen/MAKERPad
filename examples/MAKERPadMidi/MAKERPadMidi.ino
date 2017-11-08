#include <MAKERPad.h>
#include <MIDI.h>
//#include <Tone.h>

#define STATUS_LED_PIN 19
#define BUZZER_PIN 6
MAKERPad mp = MAKERPad();
boolean buzzer_enable = true;

struct MySettings : public midi::DefaultSettings {
  static const long BaudRate = 115200;
};

MIDI_CREATE_CUSTOM_INSTANCE(HardwareSerial, Serial, MIDI, MySettings);

//Tone voice;
int octave = 3;

#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978


int note_to_freq(byte note){
  if (note == 32) return NOTE_B0;
  if (note == 33) return NOTE_C1;
  if (note == 34) return NOTE_CS1;
  if (note == 35) return NOTE_D1;
  if (note == 36) return NOTE_DS1;
  if (note == 37) return NOTE_E1;
  if (note == 38) return NOTE_F1;
  if (note == 39) return NOTE_FS1;
  if (note == 40) return NOTE_G1;
  if (note == 41) return NOTE_GS1;
  if (note == 42) return NOTE_A1;
  if (note == 43) return NOTE_AS1;
  if (note == 44) return NOTE_B1;
  if (note == 45) return NOTE_C2;
  if (note == 46) return NOTE_CS2;
  if (note == 47) return NOTE_D2;
  if (note == 48) return NOTE_DS2;
  if (note == 49) return NOTE_E2;
  if (note == 50) return NOTE_F2;
  if (note == 51) return NOTE_FS2;
  if (note == 52) return NOTE_G2;
  if (note == 53) return NOTE_GS2;
  if (note == 54) return NOTE_A2;
  if (note == 55) return NOTE_AS2;
  if (note == 56) return NOTE_B2;
  if (note == 57) return NOTE_C3;
  if (note == 58) return NOTE_CS3;
  if (note == 59) return NOTE_D3;
  if (note == 60) return NOTE_DS3;
  if (note == 61) return NOTE_E3;
  if (note == 62) return NOTE_F3;
  if (note == 63) return NOTE_FS3;
  if (note == 64) return NOTE_G3;
  if (note == 65) return NOTE_GS3;
  if (note == 66) return NOTE_A3;
  if (note == 67) return NOTE_AS3;
  if (note == 68) return NOTE_B3;
  if (note == 69) return NOTE_C4;
  if (note == 70) return NOTE_CS4;
  if (note == 71) return NOTE_D4;
  if (note == 72) return NOTE_DS4;
  if (note == 73) return NOTE_E4;
  if (note == 74) return NOTE_F4;
  if (note == 75) return NOTE_FS4;
  if (note == 76) return NOTE_G4;
  if (note == 77) return NOTE_GS4;
  if (note == 78) return NOTE_A4;
  if (note == 79) return NOTE_AS4;
  if (note == 80) return NOTE_B4;
  if (note == 81) return NOTE_C5;
  if (note == 82) return NOTE_CS5;
  if (note == 83) return NOTE_D5;
  if (note == 84) return NOTE_DS5;
  if (note == 85) return NOTE_E5;
  if (note == 86) return NOTE_F5;
  if (note == 87) return NOTE_FS5;
  if (note == 88) return NOTE_G5;
  if (note == 89) return NOTE_GS5;
  if (note == 90) return NOTE_A5;
  if (note == 91) return NOTE_AS5;
  if (note == 92) return NOTE_B5;
  if (note == 93) return NOTE_C6;
  if (note == 94) return NOTE_CS6;
  if (note == 95) return NOTE_D6;
  if (note == 96) return NOTE_DS6;
  if (note == 97) return NOTE_E6;
  if (note == 98) return NOTE_F6;
  if (note == 99) return NOTE_FS6;
  if (note == 100) return NOTE_G6;
  if (note == 101) return NOTE_GS6;
  if (note == 102) return NOTE_A6;
  if (note == 103) return NOTE_AS6;
  if (note == 104) return NOTE_B6;
  if (note == 105) return NOTE_C7;
  if (note == 106) return NOTE_CS7;
  if (note == 107) return NOTE_D7;
  if (note == 108) return NOTE_DS7;
  if (note == 109) return NOTE_E7;
  if (note == 110) return NOTE_F7;
  if (note == 111) return NOTE_FS7;
  if (note == 112) return NOTE_G7;
  if (note == 113) return NOTE_GS7;
  if (note == 114) return NOTE_A7;
  if (note == 115) return NOTE_AS7;
  if (note == 116) return NOTE_B7;
  if (note == 117) return NOTE_C8;
  if (note == 118) return NOTE_CS8;
  if (note == 119) return NOTE_D8;
  if (note == 120) return NOTE_DS8;
  return 0;
}

void handleNoteOn(byte channel, byte note, byte velocity){
  if (!buzzer_enable) return;
  if (channel == 2) {
    unsigned int freq = note_to_freq(note);
    if (freq) tone(BUZZER_PIN, note_to_freq(note));
  }
}
void handleNoteOff(byte channel, byte note, byte velocity){
  if (!buzzer_enable) return;
  if (channel == 2) {
    noTone(BUZZER_PIN);
  }
}

void setup(){
 MIDI.begin(MIDI_CHANNEL_OMNI);  // Listen to all incoming messages
// MIDI.begin(2);
 mp.Set(STATUS_LED_PIN);
 mp.Set(18);
// MIDI.setHandleNoteOn(handleNoteOn);
// MIDI.setHandleNoteOff(handleNoteOff);
}


void loop(){
//  MIDI.read(2);
  // Update the buttons down LED
  mp.UpdateButtons();

  for(int btn=0; btn<19; btn++) {
    if (btn < 16) {
      if (mp.CheckButtonPosEdge(btn)) {
        MIDI.sendNoteOn(36+btn+(12*octave), 127, 1);
        handleNoteOn(2, 36+btn+(12*octave), 127);
      }
      if (mp.CheckButtonNegEdge(btn)) {
        MIDI.sendNoteOff(36+btn+(12*octave), 127, 1);
        handleNoteOff(2, 36+btn+(12*octave), 127);
      }
    }
    if (btn == 16 && mp.CheckButtonPosEdge(btn)) octave--;
    if (btn == 17 && mp.CheckButtonPosEdge(btn)) octave++;
    if (btn == 18 && mp.CheckButtonPosEdge(btn)) {
      buzzer_enable = !buzzer_enable;
      mp.Toggle(btn);
    }
    if (btn != 18) {
      if (mp.CheckButtonDown(btn)) {
        mp.Set(btn);
      } else {
        mp.Clear(btn);
      }
    }
  }
}