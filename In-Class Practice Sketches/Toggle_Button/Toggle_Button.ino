/*
TOGGLE BUTTON "EDGE DETECTION" - PUSH BUTTON AS TOGGELE SWITCH
JAY J GURUNG
9-17-2025
*/

const int buttonPin = 9;
const int ledPin = 4;

bool buttonState = 0;     //current button state
bool lastButtonState =0;  // previous button state
bool toggle =0;           //toggle bool for rising edge
bool fallToggle = 0;      //falling edge bool

void setup() 
{
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);

  Serial.begin(115200);
}

void loop() 
{
  buttonState = !digitalRead(buttonPin);


  //RISING EDGE DETECTION
  if (buttonState && !lastButtonState) //if button went from LOW to HIGH
  {
    toggle = !toggle; //flip toggle state
  }

  //FALLING EDGE DETECTION
  if (!buttonState && lastButtonState) //if button went from LOW to HIGH
  {
    fallToggle = !fallToggle; //flip toggle state
  }

  lastButtonState = buttonState;
   digitalWrite(ledPin, toggle);

  Serial.printf("Button = %i _ Toggle %i _ Falling  %i _ \n", buttonState, toggle, fallToggle);

}
