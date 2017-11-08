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
  {B0010, 600, 617},
  {B0011, 667, 679},
  {B0100, 730, 740},
  {B0101, 765, 773},
  {B0110, 813, 830},
  {B0111 & CLEAR, 831, 837}, // Unstable. B0111 values too similar to B1000.
  {B1000, 838, 842},
  {B1001, 848, 855},
  {B1010, 872, 881},
  {B1011, 882, 891},
  {B1100, 892, 899},
  {B1101, 900, 906},
  {B1110, 907, 918},
  {B1111, 919, 1001}
};
#endif


volatile charliePin leds[20] = {{ 4 , 0 },{ 3 , 0 },{ 2 , 0 },{ 1 , 0 },
{ 0 , 1 },{ 4 , 1 },{ 3 , 1 },{ 2 , 1 },
{ 1 , 2 },{ 0 , 2 },{ 4 , 2 },{ 3 , 2 },
{ 2 , 3 },{ 1 , 3 },{ 0 , 3 },{ 4 , 3 },
{ 2 , 4 },{ 1 , 4 },{ 0 , 4 },{ 3 , 4 }};

int buttons[5] = {0,0,0,0,0};

int down[5][2] = {{0,0},{0,0},{0,0},{0,0},{0,0}};
int oneshot[20];

volatile boolean display[20];

volatile boolean show = false;

volatile int di = 0;

volatile byte pins[5] = {5,4,3,2,1};


MAKERPad::MAKERPad(void) {

  for(int i = 0; i < 5*4; i++) {
    display[i] = false;
  }
  di = 2;

  pinMode( 13 , INPUT);
  pinMode( 12 , INPUT);
  pinMode( 11 , INPUT);
  pinMode( 10 , INPUT);
  pinMode( 9 , INPUT);


  cli();          // disable global interrupts
  TIMSK1 = (1<<TOIE1);
  // set timer0 counter initial value to 0
  TCNT1=35624;


  // start timer0 with /1024 prescaler
  TCCR1B |= (1 << CS10);
  sei();          // enable global interrupts



  sei();
  interrupts();             // enable all interrupts
}

void MAKERPad::Set(int num) {
  display[num] = true;
}

void MAKERPad::Clear(int num) {
  display[num] = false;
}

void MAKERPad::Clear() {
  for(int i = 0; i < 5*4; i++) {
    display[i] = false;
  }
}

void MAKERPad::Toggle(int num) {
  display[num] = !display[num];
}

void MAKERPad::ClearRow(int num) {
  for(int i  = 0; i < 4; i++)
  display[num*4+i] = false;
}
boolean MAKERPad::CheckButtonPressed(int num){
  if(oneshot[num]) {
    oneshot[num] = false;
    return true;

  } else {
    return oneshot[num];
  }
}

boolean MAKERPad::CheckButtonDown(int num){

  return oneshot[num];
}

void MAKERPad::UpdateButtons() {
  buttons[0] =  analogRead(0);
  buttons[1] =  analogRead(1);
  buttons[2] =  analogRead(2);
  buttons[3] =  analogRead(3);
  buttons[4] =  analogRead(4);

  for(int j = 0; j < 5; j++) {
    if(buttons[j] >= down[j][0] && buttons[j] <= down[j][1]) {

    } else {
      down[j][0] = 0;
      down[j][1] = 0;
      for(int k = 0; k < 4;k++) oneshot[j*4+k] = false;
      for(int i = 0; i <= 16; i++)
      {
        if(buttons[j] >= Button[i][1] && buttons[j] <= Button[i][2])
        {


          int label = Button[i][0];

          down[j][0] = Button[i][1];
          down[j][1] = Button[i][2];

          oneshot[j*4+label] = true;

        }
      }
    }
  }
}

ISR(TIMER1_OVF_vect)
{
  TCNT1 = 1000;
  TCCR1B = (1<<CS10);
  DDRB = DDRB & ~B00111110;
  PORTB = PORTB & 0xFF80;

  if(display[di]) {
    DDRB = DDRB | (1 << pins[leds[di].vcc]) | (1 << pins[leds[di].gnd]);
    PORTB = PORTB | (1 << pins[leds[di].vcc]);
  }

  di++;
  if(di >= 20) di = 0;

}
