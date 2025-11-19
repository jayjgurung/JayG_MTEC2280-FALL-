/*
  Jay J Gurung
  11/18/2025
  Serial Handshaking Grid Drawer
  - Receives control bytes 'A' and 'B' from p5.js
  - On 'A': turns DRAW LED on, PAUSE LED off, reads sensors, sends CSV:
        button1,button2,pot1,pot2\n
  - On 'B': turns DRAW LED off, PAUSE LED on, sends a simple status line
*/

#include <Arduino.h>

// === PIN SETUP (change if needed) ===
const int POT_X_PIN      = 6;   // potentiometer 1 (horizontal)
const int POT_Y_PIN      = 7;   // potentiometer 2 (vertical)
const int BUTTON_FILL_PIN  = 1; // button 1 (fill)
const int BUTTON_RESET_PIN = 2; // button 2 (reset)
const int LED_DRAW_PIN     = 40; // LED 1: drawing mode
const int LED_PAUSE_PIN    = 39; // LED 2: paused mode

// === GLOBALS ===
int potX = 0;
int potY = 0;
int buttonFill  = 0;
int buttonReset = 0;
char inByte = 0;

void setup() {
  Serial.begin(9600);

  pinMode(POT_X_PIN, INPUT);
  pinMode(POT_Y_PIN, INPUT);

  pinMode(BUTTON_FILL_PIN, INPUT_PULLUP);
  pinMode(BUTTON_RESET_PIN, INPUT_PULLUP);

  pinMode(LED_DRAW_PIN, OUTPUT);
  pinMode(LED_PAUSE_PIN, OUTPUT);

  // start in paused state
  digitalWrite(LED_DRAW_PIN, LOW);
  digitalWrite(LED_PAUSE_PIN, HIGH);
}

void loop() {
  if (Serial.available() > 0) {
    inByte = Serial.read();

    if (inByte == 'A') {
      // ----- ACTIVE / DRAWING MODE -----
      digitalWrite(LED_DRAW_PIN, HIGH);
      digitalWrite(LED_PAUSE_PIN, LOW);

      // read sensors
      potX = analogRead(POT_X_PIN);
      potY = analogRead(POT_Y_PIN);

      // buttons are wired with INPUT_PULLUP (pressed = 0, released = 1)
      // invert so pressed = 1 for p5:
      buttonFill  = !digitalRead(BUTTON_FILL_PIN);
      buttonReset = !digitalRead(BUTTON_RESET_PIN);

      // send CSV: buttonFill,buttonReset,potX,potY
      Serial.print(buttonFill);
      Serial.print(',');
      Serial.print(buttonReset);
      Serial.print(',');
      Serial.print(potX);
      Serial.print(',');
      Serial.print(potY);
      Serial.print('\n');
    }
    else if (inByte == 'B') {
      // ----- PAUSED MODE -----
      digitalWrite(LED_DRAW_PIN, LOW);
      digitalWrite(LED_PAUSE_PIN, HIGH);

      // still reply so call/response is preserved
      Serial.println("PAUSED");
    }
    else {
      // unknown control byte (optional status)
      digitalWrite(LED_DRAW_PIN, LOW);
      digitalWrite(LED_PAUSE_PIN, LOW);
      Serial.println("UNKNOWN_CONTROL_BYTE");
    }
  }
}