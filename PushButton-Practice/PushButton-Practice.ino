/*
PUSHBUTTON - SIMPLE PRACTICE
JAY J GURUNG
9-17-2025
*/



const int buttonPin = 9;

void setup() {
  // put your setup code here, to run once:
  //pinMode(buttonPin, INPUT); // Normal way do take input.
  pinMode(buttonPin, INPUT_PULLUP); //GND to Button to GPIO 9
  // pinMode(buttonPin, INPUT_PULLDOWN); // 3V3 to Button to GPIO 9


  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  bool buttonState = !digitalRead(buttonPin); // to switch output from 1 to 0 when not pressed
  // bool buttonState = digitalRead(buttonPin); // 0 when pressed


  Serial.println(buttonState);
}
