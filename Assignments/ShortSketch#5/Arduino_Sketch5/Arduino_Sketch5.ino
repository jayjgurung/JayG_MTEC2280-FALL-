/*  ESP32-S3 Serial Musical Keyboard (p5.js -> MCU)
    Jay Jung Gurung — Short Sketch #5

    Hardware (your pins):
      BTN1: GPIO1  (NOTES mode)
      BTN2: GPIO2  (ARPEGGIO mode)
      LED1: GPIO39
      LED2: GPIO40
      POT : GPIO6   (ADC1_CH5) -> BPM 60..120
      PIEZO: GPIO10 (via LEDC PWM tone)

    Serial protocol (from p5.js):
      - Down/Up: "D,<0..7>\n" / "U,<0..7>\n"
      - Scale  : "S,0\n" = major   | "S,1\n" = natural minor

    Telemetry to p5.js (newline CSV):
      - "MODE,0|1"
      - "BPM,###"
      - "PLAY,keyIndex,Hz"
*/

#include <Arduino.h>
#if !defined(ARDUINO_ARCH_ESP32)
#error "Select an ESP32 board (ESP32S3 Dev Module) under Tools > Board."
#endif
#include "driver/ledc.h"   // ESP-IDF LEDC (works on all ESP32 cores)

// ---------------- Pins ----------------
const int PIN_BTN1  = 1;
const int PIN_BTN2  = 2;
const int PIN_LED1  = 39;
const int PIN_LED2  = 40;
const int PIN_POT   = 6;   // ADC1_CH5 (OK on S3)
const int PIN_PIEZO = 10;  // If silent on your board, try 7 or 11 and update this define

// --------------- LEDC -----------------
static const ledc_mode_t      PIEZO_MODE    = LEDC_LOW_SPEED_MODE; // S3 cores expose LOW speed
static const ledc_timer_t     PIEZO_TIMER   = LEDC_TIMER_0;
static const ledc_channel_t   PIEZO_CH      = LEDC_CHANNEL_0;
static const ledc_timer_bit_t PIEZO_RES     = LEDC_TIMER_10_BIT;   // 0..1023
static const uint32_t         PIEZO_DUTY    = 900;                 // louder than 50%

void piezoInit() {
  ledc_timer_config_t tcfg = {};
  tcfg.speed_mode      = PIEZO_MODE;
  tcfg.duty_resolution = PIEZO_RES;
  tcfg.timer_num       = PIEZO_TIMER;
  tcfg.freq_hz         = 1000;          // initial
  tcfg.clk_cfg         = LEDC_AUTO_CLK;
  ledc_timer_config(&tcfg);

  ledc_channel_config_t ccfg = {};
  ccfg.gpio_num   = PIN_PIEZO;
  ccfg.speed_mode = PIEZO_MODE;
  ccfg.channel    = PIEZO_CH;
  ccfg.intr_type  = LEDC_INTR_DISABLE;
  ccfg.timer_sel  = PIEZO_TIMER;
  ccfg.duty       = 0;   // silent
  ccfg.hpoint     = 0;
  ledc_channel_config(&ccfg);
}
inline void piezoPlay(double hz) {
  if (hz <= 0) { ledc_stop(PIEZO_MODE, PIEZO_CH, 0); return; }
  ledc_set_freq(PIEZO_MODE, PIEZO_TIMER, (uint32_t)hz);
  ledc_set_duty(PIEZO_MODE, PIEZO_CH, PIEZO_DUTY);
  ledc_update_duty(PIEZO_MODE, PIEZO_CH);
}
inline void piezoOff() { ledc_stop(PIEZO_MODE, PIEZO_CH, 0); }

// --------------- Musical --------------
enum Mode // ensum Mode is a custom Class type can hold only the values listed in the definition. Class Mode has two possible values: MODE_NOTES and MODE_ARP.
{ 
  MODE_NOTES = 0, MODE_ARP = 1 
};

Mode mode = MODE_NOTES;
uint8_t scaleType = 0;               // 0=major, 1=natural minor
const int MIDI_ROOT = 60;            // C4 is MIDI 60

const int8_t MAJOR_OFFS[8] = {0,2,4,5,7,9,11,12}; // C D E F G A B C, OFFS represent semitones from root, OFF is short for OFFSET
const int8_t MINOR_OFFS[8] = {0,2,3,5,7,8,10,12};

double midiToHz(int midi) 
{ 
  return 440.0 * pow(2.0, (midi - 69) / 12.0); //440Hz is the frequency of the note A4, which is the 69th key on a standard 88-key piano keyboard. The formula calculates the frequency of any MIDI note number by determining how many semitones it is away from A4 and adjusting the frequency accordingly.
}

void buildChordHz(uint8_t degree, double outHz[], uint8_t &len) { //the parameter deg represents the scale deg (0-7) for which the chord is to be built. The outHz array is used to store the frequencies of the notes in the chord, and len is a reference parameter that will hold the number of notes in the constructed chord.
  const int8_t *offs = (scaleType==0) ? MAJOR_OFFS : MINOR_OFFS; // Select scale offsets based on scale type
  int rootMidi = MIDI_ROOT + offs[degree]; 

   // Quality should wrap within the 7-degree scale
  uint8_t deg = degree % 7;
  // Diatonic qualities (simple triads)
  bool isDim=false, isMin=false;                              // diminished, minor
  if (scaleType==0) {                                        // Major: I ii iii IV V vi vii°
    if (deg==1 || deg==2 || deg==5) isMin = true;  // ii, iii, vi
    if (deg==6) isDim = true;                            // vii°    
  } else {                                                 // Natural minor: i ii° III iv v VI VII
    if (deg==1) isDim = true;
    if (deg==0 || deg==3 || deg==4) isMin = true;
  }
  int third = (isDim||isMin) ? 3 : 4;       // m3 or M3
  int fifth = isDim ? 6 : 7;                // dim5 or P5

  outHz[0] = midiToHz(rootMidi);
  outHz[1] = midiToHz(rootMidi + third);
  outHz[2] = midiToHz(rootMidi + fifth);
  len = 3;
}

// --------------- State ----------------
bool lastBtn1 = true, lastBtn2 = true;      // pullups => idle HIGH
int  heldKey  = -1;                         // -1 = none
bool keyHeld  = false;

int bpm = 90; // default BPM
unsigned long lastTelem = 0;

// Arp engine
double chord[4]; // max 4 notes in chord
uint8_t chordLen = 0, arpIdx = 0;
unsigned long nextArpMs = 0;
bool ledToggle = false;

unsigned long bpmToIntervalMs(int _bpm) {   // 16th-note feel
  return 60000UL / (4UL * (unsigned long)_bpm);
}

inline unsigned long nowMs(){ return millis(); }

// --------------- Actions --------------
void stopAllSound(){  //default function to stop all sound and turn off LEDs
  piezoOff();
  digitalWrite(PIN_LED1, LOW);
  digitalWrite(PIN_LED2, LOW);
}

void startNoteByIndex(int idx){ // this function starts playing a single note based on the held key index.
  const int8_t *offs = (scaleType==0)?MAJOR_OFFS:MINOR_OFFS; // when scale
  int midi = MIDI_ROOT + offs[idx];
  double hz = midiToHz(midi);
  piezoPlay(hz);
  digitalWrite(PIN_LED1, HIGH);
  digitalWrite(PIN_LED2, HIGH);
  Serial.print("PLAY,"); Serial.print(idx); Serial.print(","); Serial.println((int)hz);
}

void startArpByIndex(int idx){ // this function initializes the arpeggio playback based on the held key index.
  buildChordHz(idx, chord, chordLen); // build chord based on held key
  arpIdx = 0; // start from first note
  nextArpMs = nowMs(); //nowMs() is 
}

// --------------- Loop pieces ----------
void updateArpeggio() { // this function updates the arpeggio playback based on the current mode, key state, and timing.
  if (mode != MODE_ARP || !keyHeld || heldKey < 0 || chordLen == 0) return; // this line checks if the current mode is not arpeggio mode, if no key is held, if the held key index is invalid, or if the chord length is zero. If any of these conditions are true, the function exits early without performing any further actions.
  unsigned long ms = nowMs(); // this line retrieves the current time in milliseconds since the program started running.
  if (ms >= nextArpMs) { // this condition checks if the current time has reached or exceeded the scheduled time for the next arpeggio note to be played.
    double hz = chord[arpIdx];
    piezoPlay(hz);

    // LED ping-pong
    digitalWrite(PIN_LED1, ledToggle ? HIGH : LOW);
    digitalWrite(PIN_LED2, ledToggle ? LOW  : HIGH);
    ledToggle = !ledToggle;

    Serial.print("PLAY,"); Serial.print(heldKey); Serial.print(",");
    Serial.println((int)hz);

    arpIdx = (arpIdx + 1) % chordLen;
    nextArpMs = ms + bpmToIntervalMs(bpm);
  }
}

void updateTelemetry() { // this function updates the telemetry data, including BPM and mode, and sends it over the serial connection at regular intervals. telemetry means the process of recording and transmitting the readings of an instrument or device.
  int raw = analogRead(PIN_POT);            // 0..4095
  bpm = map(raw, 0, 4095, 60, 120);

  unsigned long ms = nowMs();
  if (ms - lastTelem >= 100) {
    lastTelem = ms;
    Serial.print("BPM,");  Serial.println(bpm);
    Serial.print("MODE,"); Serial.println((int)mode);
  }
}

void readButtons() { // this function reads the state of two buttons and updates the mode of operation based on button presses.
  // Active-low with INPUT_PULLUP
  bool b1 = digitalRead(PIN_BTN1);
  bool b2 = digitalRead(PIN_BTN2);

  if (b1 != lastBtn1 && b1 == LOW) {         // BTN1 pressed
    mode = MODE_NOTES;
    if (!keyHeld) stopAllSound();
  }
  if (b2 != lastBtn2 && b2 == LOW) {         // BTN2 pressed
    mode = MODE_ARP;                          // arp starts on key down
  }
  lastBtn1 = b1; lastBtn2 = b2;
}

// ---------- Serial protocol (line-based) ----------
//   S,<0|1>   -> set scale (major/minor)
//   D,<0..7>  -> key down (start note/arp; hold)
//   U,<0..7>  -> key up (stop if same key)
void handleSerial() { //this function handles incoming serial commands to control the musical keyboard's behavior, including setting the scale type and managing key presses/releases.
  while (Serial.available()) {
    String line = Serial.readStringUntil('\n');
    line.trim();
    if (!line.length()) continue;

    if (line[0]=='S') {
      int comma = line.indexOf(',');
      scaleType = (comma>=0 ? line.substring(comma+1).toInt() : line.toInt()) ? 1 : 0;
      continue;
    }

    if (line[0]=='D' || line[0]=='U') {
      int comma = line.indexOf(',');
      if (comma < 0) continue;
      int idx = line.substring(comma+1).toInt();
      if (idx < 0 || idx > 7) continue;

      if (line[0]=='D') {
        heldKey = idx; keyHeld = true;
        if (mode == MODE_NOTES) startNoteByIndex(idx);
        else                    startArpByIndex(idx);
      } else { // 'U'
        if (heldKey == idx) {
          keyHeld = false; heldKey = -1;
          stopAllSound();
        }
      }
      continue;
    }

    // (Optional fallback: single-letter keys behave as Down)
    if (line.length()==1) {
      const char KEYS[8] = {'a','s','d','f','g','h','j','k'};
      int idx=-1; for(int i=0;i<8;i++) if(line[0]==KEYS[i]||line[0]==toupper(KEYS[i])){idx=i;break;}
      if (idx>=0) { heldKey=idx; keyHeld=true; if(mode==MODE_NOTES) startNoteByIndex(idx); else startArpByIndex(idx); }
    }
  }
}

// --------------- Arduino --------------
void setup() {
  Serial.begin(9600);

  pinMode(PIN_BTN1, INPUT_PULLUP);
  pinMode(PIN_BTN2, INPUT_PULLUP);
  pinMode(PIN_LED1, OUTPUT);
  pinMode(PIN_LED2, OUTPUT);
  digitalWrite(PIN_LED1, LOW);
  digitalWrite(PIN_LED2, LOW);

  piezoInit();

  // Startup chirp so you know the buzzer + pin work
  ledc_set_freq(PIEZO_MODE, PIEZO_TIMER, 880);
  ledc_set_duty(PIEZO_MODE, PIEZO_CH, PIEZO_DUTY);
  ledc_update_duty(PIEZO_MODE, PIEZO_CH);
  delay(150);
  ledc_stop(PIEZO_MODE, PIEZO_CH, 0);
}

void loop() {
  readButtons();
  handleSerial();
  updateArpeggio();
  updateTelemetry();
}