
const int potPin = 1;
// const pinRGB = 38;

void setup() 
{
  analogReadResolution(12);   //default ADC resolution for ESP32 is 12-bit [range of 0-4095]

  Serial.begin(115200); //start serial comms
}

void loop() 
{
  int adcVal = analogRead(potPin);  //read current analog voltage value @ potPin and store in variable

  Serial.println(adcVal); //print ADC value to serial monitor
}