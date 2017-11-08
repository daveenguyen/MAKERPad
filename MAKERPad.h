#ifndef MAKERPAD_H
#define MAKERPAD_H
#include <Arduino.h>

// Pulled from the charlieplex library
struct CharliePin {
	byte vcc;
	byte gnd;
};

typedef CharliePin charliePin;

class MAKERPad {

public:
  MAKERPad(void);
  void Set(int num);
  void Clear(int num);
  void Clear();
  void Toggle(int num);
  void ClearRow(int num);
  void UpdateButtons();
  boolean CheckButtonPressed(int num);
  boolean CheckButtonDown(int num);
  boolean CheckButtonPosEdge(int num);
  boolean CheckButtonNegEdge(int num);

};

#endif
