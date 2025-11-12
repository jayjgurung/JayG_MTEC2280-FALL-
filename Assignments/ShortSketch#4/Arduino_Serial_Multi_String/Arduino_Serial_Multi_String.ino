/*
<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
||||||||||||||||||||||||||||||||||||||||||||
||      "Ultrasonic + Buttons + Pot"      ||
||         "String CSV Version"           ||
||||||||||||||||||||||||||||||||||||||||||||     
- Sends comma-separated values via Serial UART:
  distance_cm, button1, button2, potValue
- Uses '/n' newline at the end for p5.js line parsing.
<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
*/

// ---- Pin Definitions ----
const int PIN_TRIG     = 5;   // HC-SR04 TRIG
const int PIN_ECHO     = 4;   // HC-SR04 ECHO (use 5V→3.3V divider)
const int BUTTON_PIN_1 = 1;   // Push button 1
const int BUTTON_PIN_2 = 2;   // Push button 2
const int POT_PIN      = 6;   // 10k Potentiometer (ADC input)

// ---- Variables ----
bool buttonState_1 = 0;
bool buttonState_2 = 0;
int potValue = 0;
int distanceCM = 0;

// ---- Timer ----
unsigned long lastTime = 0;
const unsigned long INTERVAL_MS = 20;  // ~50 FPS

void setup() {
  // Serial Setup
  Serial.begin(9600);  // match p5.js baud rate

  // Pin Modes
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  pinMode(BUTTON_PIN_1, INPUT_PULLUP);
  pinMode(BUTTON_PIN_2, INPUT_PULLUP);

  analogReadResolution(10); // 10-bit ADC (0–1023)
  analogSetPinAttenuation(POT_PIN, ADC_11db); // allow ~0–3.3V range
}

// ---- Ultrasonic Distance Function ----
int readDistanceCM() {
  // send 10µs trigger pulse
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  // measure echo duration (timeout ~30ms ≈ 5m)
  unsigned long duration = pulseIn(PIN_ECHO, HIGH, 30000UL);
  if (duration == 0) return -1; // timeout or no echo
  int cm = duration / 58;       // convert µs → cm
  return cm;
}

void loop() {
  unsigned long now = millis();
  if (now - lastTime < INTERVAL_MS) return;
  lastTime = now;

  // Read sensors
  distanceCM = readDistanceCM();
  potValue   = analogRead(POT_PIN);
  buttonState_1 = !digitalRead(BUTTON_PIN_1); // inverted due to pull-up
  buttonState_2 = !digitalRead(BUTTON_PIN_2);

  // ---- Send comma-separated string ----
  Serial.print(distanceCM);
  Serial.print(',');
  Serial.print(buttonState_2);
  Serial.print(',');
  Serial.print(buttonState_1);
  Serial.print(',');
  Serial.print(potValue);
  Serial.print('\n');  // newline for p5.js line reading
}