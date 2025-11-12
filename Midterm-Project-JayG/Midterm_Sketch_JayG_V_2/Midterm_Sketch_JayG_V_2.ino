/*  Mid-Term Project MTEC 2800
    Jay Jung Gurung
    Distance Theremin (HC-SR04) with A natural minor scale.
      - Button gates everything: while held -> play piezo + print freq/note
      - LED brightness follows distance (closer = brighter)
      - Quantized pitch (A3..A5) 
*/

//  ---- PIN SETUP  
const int PIN_BTN    = 13;   // button
const int PIN_LED    = 2;    // LED
const int PIN_TRIG   = 12;   // HC-SR04 TRIG 
const int PIN_ECHO   = 14;   // HC-SR04 ECHO 
const int PIN_PIEZO  = 21;   //  piezo 

//   ---- SCALE in frequencies and notes (A natural minor: A3..A5) 
const int A_MIN_LEN = 15;
const float A_MIN_FREQS[A_MIN_LEN] = {
  220.00, 246.94, 261.63, 293.66, 329.63,
  349.23, 392.00, 440.00, 493.88, 523.25,
  587.33, 659.25, 698.46, 783.99, 880.00
};
const char* A_MIN_NOTES[A_MIN_LEN] = {
  "A3","B3","C4","D4","E4",
  "F4","G4","A4","B4","C5",
  "D5","E5","F5","G5","A5"
};

//  --- TIMING ----------
unsigned long lastPrintMs = 0;
const unsigned long PRINT_EVERY_MS = 100; // so that it does not spam, makes data more readable

// ---------- MAPPING (tune these to your space/hand range) ----------
const int MIN_CM = 5;    // nearest hand distance you want to use
const int MAX_CM = 60;   // farthest hand distance you want to use

// helper: constrain
int iconstrain(int v, int lo, int hi) { return (v < lo) ? lo : (v > hi) ? hi : v; }

// reads HC-SR04 distance in cm (returns -1 on timeout)
int readDistanceCm() {
  // sends 10 µs trigger
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  // measures echo HIGH pulse (has timeout of 30ms ≈ ~5m)
  unsigned long dur = pulseIn(PIN_ECHO, HIGH, 30000UL);
  if (dur == 0) return -1; // timeout
  // speed of sound: cm = microseconds / 58.0
  return (int)(dur / 58.0);
}

void setup() {
  Serial.begin(115200);

  // I/O
  pinMode(PIN_BTN, INPUT_PULLUP);    // not pressed = HIGH, pressed = LOW
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);          // MUST be level-shifted to 3.3V on ESP32!

  // idle states
  digitalWrite(PIN_TRIG, LOW);
  digitalWrite(PIN_LED, LOW);

  Serial.println("Distance-Theremin (HC-SR04, A minor) ready!");
  Serial.println("Hold the button to hear tone and see readout.");
}

void loop() {
  // gate
  bool pressed = (digitalRead(PIN_BTN) == LOW);

  // measure distance
  int cm = readDistanceCm();                 // -1 means no reading
  if (cm == -1) {
    // no echo: keep LED off unless pressed? We'll just treat as MAX distance
    cm = MAX_CM;
  }

  // map distance to scale index (closer -> lower index by default)
  // If you want closer = higher pitch, swap MIN/MAX in map().
  cm = iconstrain(cm, MIN_CM, MAX_CM);
  int idx = map(cm, MIN_CM, MAX_CM, 0, A_MIN_LEN - 1);
  idx = iconstrain(idx, 0, A_MIN_LEN - 1);

  float freq = A_MIN_FREQS[idx];
  const char* noteName = A_MIN_NOTES[idx];

  // LED follows distance: closer = brighter
  // map: MIN_CM -> 255, MAX_CM -> 0
  int ledPWM = map(cm, MIN_CM, MAX_CM, 255, 0);
  ledPWM = iconstrain(ledPWM, 0, 255);
  analogWrite(PIN_LED, ledPWM); // ESP32 core maps to LEDC; fine for simple use

  if (pressed) {
    // plays a tone and prints occasionally
    tone(PIN_PIEZO, freq);

    unsigned long now = millis();
    if (now - lastPrintMs >= PRINT_EVERY_MS) {
      lastPrintMs = now;
      Serial.print("dist=");
      Serial.print(cm);
      Serial.print(" cm  ->  ");
      Serial.print(noteName);
      Serial.print("  ");
      Serial.print(freq, 2);
      Serial.println(" Hz");
    }
  } else {
    noTone(PIN_PIEZO);
    // no Serial spam when not pressed
  }
}