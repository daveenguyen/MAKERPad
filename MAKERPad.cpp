#include "MAKERPad.h"

// Define which mapping to use
#define FREDADAM


// MAPPING BETWEEN ANALOG READING AND BUTTONS
/*
  A list of mapping between buttons and analog reading values.

  If the analog reading is within the range of minimum and maximum values,
  the item is active which maps to specific button currently pressed down.


  Structure of each mapping item:
    binary    buttons pressed down
    int       minimum value
    int       maximum value


  Example:
    Given:
      item:    {B1101, 900, 906}
      reading: 903

    Since reading of 903 is within the range of 900-906, item is active.
    The active item's binary number tell us button 0, 1, and 3 are pressed down.

    B1101
     |||└- Button 0: pressed down
     ||└-- Button 1: not pressed down
     |└--- Button 2: pressed down
     └---- Button 3: pressed down
*/
#define CLEAR B0000
#define TOTAL_MAP_ENTRY 16
#define BUTTON_DOWN 0
#define ACTIVE_MIN  1
#define ACTIVE_MAX  2

#ifdef IAN
int voltage_to_buttons[][4] = {
  {B0000, 000, 300},
  {B0001, 835, 842},
  {B0010, 730, 740},
  {B0011, 892, 1000},
  {B0100, 600, 610},
  {B0101, 872, 880},
  {B0110, 813, 823},
  {B0111 & CLEAR, 831, 837},
  {B1000, 310, 324},
  {B1001, 848, 855},
  {B1010, 765, 773},
  {B1011 & CLEAR, 882, 891},
  {B1100, 667, 675},
  {B1101 & CLEAR, 900, 906},
  {B1110 & CLEAR, 907, 918},
  {B1111 & CLEAR, 919, 1001}
};
#endif

#ifdef FREDADAM
int voltage_to_buttons[][4] = {
  {B0000, 000, 300},
  {B0001, 310, 324},
  {B0010, 600, 620},
  {B0011, 667, 679},
  {B0100, 730, 750},
  {B0101, 765, 773},
  {B0110, 813, 830},
  {B0111 & CLEAR, 831, 837}, // Unstable. B0111 values too similar to B1000.
  {B1000, 838, 844},
  {B1001, 848, 860},
  {B1010, 872, 881},
  {B1011, 882, 891},
  {B1100, 892, 899},
  {B1101, 900, 906},
  {B1110, 907, 918},
  {B1111, 919, 1001}
};
#endif

#define TOTAL 20
#define ROWS 5
#define COLS 4

volatile charliePin leds[TOTAL] = {{ 4 , 0 },{ 3 , 0 },{ 2 , 0 },{ 1 , 0 },
{ 0 , 1 },{ 4 , 1 },{ 3 , 1 },{ 2 , 1 },
{ 1 , 2 },{ 0 , 2 },{ 4 , 2 },{ 3 , 2 },
{ 2 , 3 },{ 1 , 3 },{ 0 , 3 },{ 4 , 3 },
{ 2 , 4 },{ 1 , 4 },{ 0 , 4 },{ 3 , 4 }};

// BUTTON EVENTS
int posedge[TOTAL];
int negedge[TOTAL];

volatile boolean led_display[TOTAL];

volatile int di = 0;

volatile byte pins[ROWS] = {5,4,3,2,1};

// DEBOUNCE
unsigned long debounce_delay = 50;    // the debounce time; increase if the output flickers
unsigned long last_debounce[TOTAL];   // the last time the output pin was toggled
int previous_state[TOTAL];
int current_state[TOTAL];


MAKERPad::MAKERPad(void) {

  for(int i=0; i<TOTAL; i++) {
    led_display[i] = false;
  }
  di = 2;


  pinMode(13, INPUT);
  pinMode(12, INPUT);
  pinMode(11, INPUT);
  pinMode(10, INPUT);
  pinMode(9, INPUT);


  cli();          // disable global interrupts

  TIMSK1 = (1<<TOIE1);
  // set timer0 counter initial value to 0
  TCNT1=35624;
  // start timer0 with /1024 prescaler
  TCCR1B |= (1 << CS10);

  sei();          // enable global interrupts
  interrupts();   // enable all interrupts
}

void MAKERPad::Set(int num) {
  led_display[num] = true;
}

void MAKERPad::Clear(int num) {
  led_display[num] = false;
}

void MAKERPad::Clear() {
  for(int i=0; i<TOTAL; i++) {
    led_display[i] = false;
  }
}

void MAKERPad::Toggle(int num) {
  led_display[num] = !led_display[num];
}

void MAKERPad::ClearRow(int row_num) {
  for(int col=0; col<COLS; col++){
    led_display[row_num*4+col] = false;
  }
}

boolean MAKERPad::CheckButtonPosEdge(int num){
  if(posedge[num]) {
    posedge[num] = false;
    return true;
  } else {
    return posedge[num];
  }
}
boolean MAKERPad::CheckButtonNegEdge(int num){
  if(negedge[num]) {
    negedge[num] = false;
    return true;
  } else {
    return posedge[num];
  }
}

boolean MAKERPad::CheckButtonPressed(int num){
  if(posedge[num]) {
    posedge[num] = false;
    return true;
  } else {
    return posedge[num];
  }
}

boolean MAKERPad::CheckButtonDown(int num){
  return current_state[num];
}

// Helper functions to update buttons state
int get_active_mapping(int read_value) {
  int min_value;
  int max_value;

  for (int i=0; i<TOTAL_MAP_ENTRY; i++) {
    min_value = voltage_to_buttons[i][ACTIVE_MIN];
    max_value = voltage_to_buttons[i][ACTIVE_MAX];

    if (read_value >= min_value && read_value <= max_value) {
      return i;
    }
  }

  return 0; // default
}

boolean is_button_down(int read_value, int button_col) {
  int active = get_active_mapping(read_value);

  if (voltage_to_buttons[active][BUTTON_DOWN] & (B1 << button_col)) {
    return true;
  } else {
    return false;
  }
}

void MAKERPad::UpdateButtons() {

  for(int row=0; row<ROWS; row++) {
    unsigned long current_millis = millis();
    int current_reading = analogRead(row);

    for(int col=0; col<COLS; col++) {

      int current_index = row*4+col;
      boolean current_button_reading = is_button_down(current_reading, col);

      // restart time if reading is unstable
      if (current_button_reading != previous_state[current_index]) {
        last_debounce[current_index] = current_millis;
      }

      // if signal was stable for debounce delay
      // and the signal value changed
      // update button states
      if ((current_millis - last_debounce[current_index]) > debounce_delay) {
        if (current_button_reading != current_state[current_index]) {
          current_state[current_index] = current_button_reading;
          if (current_button_reading) {
            posedge[current_index] = true;
          } else {
            negedge[current_index] = true;
          }
        }
      }

      previous_state[current_index] = current_button_reading;
    }
  }
}

ISR(TIMER1_OVF_vect)
{
  TCNT1 = 1000;
  TCCR1B = (1<<CS10);
  DDRB = DDRB & ~B00111110;
  PORTB = PORTB & 0xFF80;

  if(led_display[di]) {
    DDRB = DDRB | (1 << pins[leds[di].vcc]) | (1 << pins[leds[di].gnd]);
    PORTB = PORTB | (1 << pins[leds[di].vcc]);
  }

  di++;
  if(di >= 20) di = 0;

}
