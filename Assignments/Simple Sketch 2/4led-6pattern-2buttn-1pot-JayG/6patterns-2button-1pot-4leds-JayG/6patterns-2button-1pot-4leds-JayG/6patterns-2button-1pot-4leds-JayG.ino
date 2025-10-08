//Pot analog value testing.

const int pinRGB =38;   // built-in RGB led
const int potPin = 1;

const int numReads = 16;
int reading[numReads];
int count = 0;


void setup() {
  // put your setup code here, to run once:
  analogReadResolution(11);

  Serial.begin(115200);

}

void loop() {
  // put your main code here, to run repeatedly:
  reading[count] = analogRead(potPin);
  count++;
  
  if (count >= numReads){
    count = 0;
  }

  int sum = 0;

  for (int i = 0; i < numReads; i++){
    sum += reading[i];
  }
  

  int analogValue = sum / numReads;

  int mapVal = map(analogValue, 0, 1628, 0, 255);
  
  rgbLedWrite(pinRGB, mapVal, 0, 255);

  Serial.printf("ADC raw = %i \t ADC avg = %i \t ADC mapped = %i \n", analogRead(potPin), analogValue, mapVal);

}
