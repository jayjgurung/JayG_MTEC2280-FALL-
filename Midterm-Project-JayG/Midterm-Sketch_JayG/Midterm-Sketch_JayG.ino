/*  Mid-Term Project MTEC 2800
    Jay Jung Gurung
    Simple Theremin (A natural minor)
*/

const int PIN_BTN   = 1;    // button to GND (uses INPUT_PULLUP)
const int PIN_LED   = 2;    // LED -> 220 ohm -> GND
const int PIN_LDR   = 4;    // LDR divider junction
const int PIN_PIEZO = 15;   // passive piezo (+)

// we will average the light sensor a few times to make it less noisy
const int NUM_SAMPLES = 16;
int samples[NUM_SAMPLES];   // array to store the readings for averaging

// print to Serial Monitor every 100 ms (no delay(), we use millis)
unsigned long lastPrintMs = 0;
const unsigned long PRINT_EVERY_MS = 100;

// A natural minor notes from A3 (220 Hz) up to A5 (880 Hz)
const int A_MIN_LEN = 15;
const float A_MIN_FREQS[A_MIN_LEN] = {
  220.00, 246.94, 261.63, 293.66, 329.63,
  349.23, 392.00, 440.00, 493.88, 523.25,
  587.33, 659.25, 698.46, 783.99, 880.00
};

void setup() {
  Serial.begin(115200);
  pinMode(PIN_BTN, INPUT_PULLUP); // button not pressed = HIGH, pressed = LOW
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_LDR, INPUT);

  // make sure LED is off at start and PWM is set up
  analogWrite(PIN_LED, 0);

  Serial.println("Theremin (A minor) ready!");
}

void loop() {
  // 1) read the button
  bool pressed = (digitalRead(PIN_BTN) == LOW); // LOW means pressed (because INPUT_PULLUP)

  // 2) read the light sensor a bunch and average it
  long sum = 0;
  for (int i = 0; i < NUM_SAMPLES; i++) {
    samples[i] = analogRead(PIN_LDR); // 0..4095 on ESP32 ADC
    sum += samples[i];
  }
  int adcAvg = sum / NUM_SAMPLES;

  // 3) turn the light value into a note from our A-minor array
  //    map minimum reading..max reading -> 0..(A_MIN_LEN-1)
  int noteIndex = map(adcAvg, 1100, 3200, 0, A_MIN_LEN - 1);
  if (noteIndex < 0) noteIndex = 0;
  if (noteIndex > A_MIN_LEN - 1) noteIndex = A_MIN_LEN - 1;

  float freq = A_MIN_FREQS[noteIndex];

  // 4) if button is held, play sound and light LED (LED brightness follows light)
  if (pressed) {
    tone(PIN_PIEZO, freq);   // start a tone on the piezo
    int ledPWM = map(adcAvg, 1000, 4095, 40, 255); // dim in dark, bright in light
    if (ledPWM < 0) ledPWM = 0;
    if (ledPWM > 255) ledPWM = 255;
    analogWrite(PIN_LED, ledPWM);
  } else {
    // button not pressed: no sound, LED off
    noTone(PIN_PIEZO);
    analogWrite(PIN_LED, 0);
  }

  // 5) print info to Serial Monitor every 100 ms
  unsigned long now = millis();
  if (now - lastPrintMs >= PRINT_EVERY_MS) {
    lastPrintMs = now;
    Serial.print("BTN=");
    Serial.print(pressed ? "PRESSED" : "released");
    Serial.print("  LDR=");
    Serial.print(adcAvg);
    Serial.print("  noteIndex=");
    Serial.print(noteIndex);
    Serial.print("  freq=");
    Serial.print((int)freq);
    Serial.println(" Hz");
  }
}