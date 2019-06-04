/*
  PS3 Controller Rapid Fire+ Mod
  By: Jake Millington
  License: GPL
   
  Description: Programmed on an Arduino Pro Mini, this project allows for user defined mods for a PS3 controller.
*/

const int ANALOG_HIGH = 255;

//define pins
const int COM1 = A0,
          COM2 = A2,
          ACTIVITY = 5,
          BUTTON = 9;

//define R2 dip starting point for checks
const int SEQUENCE_START = 419; //number used to check for R2 voltage dip

//define button resting voltage
const int BUTTON_LOW = 480;
const int TRIGGER_LOW = 394;

//define COM1/COM2 button timings (µs)
const int UP = 0,       R2 = 0,
          RIGHT = 340,  R1 = 340,
          DOWN = 840,   TRIANGLE = 840,
          LEFT = 1340,  CIRCLE = 1340,
          L2 = 1840,    CROSS = 1840,
          L1 = 2340,    SQUARE = 2340;

//define amount of time to mask button press (µs)
const int BUTTON_MASK = 300;

//define rate of fire
const int FIRE_RATE = 10;

boolean selectedMod;

void setup() {
  //Serial.begin(9600);
  pinMode(COM1, INPUT);
  pinMode(COM2, INPUT);
  pinMode(ACTIVITY, OUTPUT);

  pinMode(BUTTON, INPUT);
  selectedMod = false;
}

void loop() {
  rapidFire();
}

void rapidFire() {
  if (analogRead(COM2) < SEQUENCE_START) { 
    delayMicroseconds(R1); //wait for R1 button dip

    if (analogRead(COM2) < BUTTON_LOW) { //lower voltage than normal means R1 button is pressed
      //NOTE: although not for L2 or R2 since voltage dips even lower and this will trigger even if it isn't pressed; must fix

      analogWrite(COM2, ANALOG_HIGH); //push voltage high to COM2
      digitalWrite(ACTIVITY, HIGH); //switch high to show something is happening

      delayMicroseconds(BUTTON_MASK);

      pinMode(COM2, INPUT); //switch pin back to high-impedance mode
      digitalWrite(ACTIVITY, LOW); //switch off pin

      delay(3 + 1000/FIRE_RATE); //this delay of (x + 1000/y) skips the rest of the current sequence (x) to make sure the next check will hit the next R2 dip
      //and also skips an amount of subsequent sequences, setting the rapid fire speed (1000/y == n presses/s)
    } 
  }
}