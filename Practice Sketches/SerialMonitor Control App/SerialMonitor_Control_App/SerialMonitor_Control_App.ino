/*
  "HC-SR04 TO 8-BIT SERIAL MONITOR"

  - Reads distance (cm) using HC-SR04 sensor
  - Maps it to 8-bit range (0–255)
  - Prints the value to Serial Monitor as human-readable text

  CONNECTIONS:
  HC-SR04 TRIG → GPIO 13
  HC-SR04 ECHO → GPIO 14
  VCC → 5V
  GND → GND
*/

const int PIN_TRIG = 12;
const int PIN_ECHO = 14;

// distance range in cm for mapping
const int MIN_CM = 5;
const int MAX_CM = 200;

// software timer
unsigned long lastTime = 0;
unsigned long timerInterval = 50;  // update every 50 ms (20 Hz)

void setup() {
  Serial.begin(9600);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  digitalWrite(PIN_TRIG, LOW);
}

void loop() {
  unsigned long currentTime = millis();
  if (currentTime - lastTime >= timerInterval) {
    lastTime = currentTime;

    int distance = readDistanceCm();
    if (distance == -1) distance = MAX_CM;  // timeout fallback

    // map to 0–255 range (invert if you want opposite)
    distance = constrain(distance, MIN_CM, MAX_CM);
    int mappedVal = map(distance, MIN_CM, MAX_CM, 255, 0);  

    // print readable values
    Serial.print("Distance(cm): ");
    Serial.print(distance);
    Serial.print("  |  Mapped(0-255): ");
    Serial.println(mappedVal);
  }
}

// --- Read HC-SR04 distance in cm (returns -1 on timeout) ---
int readDistanceCm() {
  // send 10 µs trigger pulse
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  // measure echo HIGH pulse (timeout 30ms ≈ ~5m)
  unsigned long dur = pulseIn(PIN_ECHO, HIGH, 30000UL);
  if (dur == 0) return -1; // timeout
  return dur / 58; // convert µs to cm
}