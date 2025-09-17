/*ANALOG READ 
ANALOG TO DIGITAL CONVERSION

JAY J GURUNG
9-17-2025
*/


const int potPin = 1;

void setup() 
{
  analogReadResolution(12); //default ADC resolution for ESP32 is 12-bit [range of 0-4095]
  Serial.begin(115200);
}

void loop() 
{
  int adcVal = analogRead(potPin);
  Serial.println(adcVal);
}
