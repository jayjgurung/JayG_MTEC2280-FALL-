/*
BUTTON COUNTER "EDGE DETECTION" - PUSH BUTTON AS TOGGELE SWITCH
JAY J GURUNG
9-17-2025
*/

const int buttonPin = 9;

bool buttonState = 0;     //current button state
bool lastButtonState =0;  // previous button state

int counter = 0;
int maxCount = 10;
void setup() 
{
  pinMode(buttonPin, INPUT_PULLUP);
  // pinMode(ledPin, OUTPUT);

  Serial.begin(115200);
}

void loop() 
{
  buttonState = !digitalRead(buttonPin);


  //RISING EDGE DETECTION
  if (buttonState && !lastButtonState) //if button went from LOW to HIGH
  {
    counter++;
  }

  if (counter > maxCount) //if button went from LOW to HIGH
  {
    counter = 0;
  }

  lastButtonState = buttonState;

  Serial.printf("ButtonState = %i, Counter = %i \n", buttonState, counter);

}
