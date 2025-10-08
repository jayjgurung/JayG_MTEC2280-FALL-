//  Jay J Gurung
//  10-05-2025
//  6 states, 4 led, 2 pushButtons and 1 potentiometer
//  states + blink sketch


//  Initiallizing the components to their respective pins
#define LED1_PIN   46
#define LED2_PIN    4
#define LED3_PIN   37
#define LED4_PIN   40

#define BTN_BLINK   6   // press = blink the current state once
#define BTN_NEXT    7   // press = go to next state (0..5)
#define POT_PIN    10   // pot sets on/off time (same value)

//  state 0 to 5:
//  0: only LED1
//  1: only LED2
//  2: only LED3
//  3: only LED4
//  4: LED1 + LED2
//  5: LED3 + LED4

int stateIndex = 0;          // start at 0
int potRaw = 0;              // just holding last pot raw reading
unsigned long beatMs = 200;  // default blink time until pot is read

// simple button edge tracking 
bool prevNextRead  = HIGH;

void setup() {
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);
  pinMode(LED4_PIN, OUTPUT);

  pinMode(BTN_BLINK, INPUT_PULLUP); // button to GND
  pinMode(BTN_NEXT,  INPUT_PULLUP); // button to GND
  // pot is analog, no pinMode needed

  // start with whatever the state says the LEDs should be
  applyState(stateIndex);
}

// turns everything off
void allOff() {
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  digitalWrite(LED3_PIN, LOW);
  digitalWrite(LED4_PIN, LOW);
}

// sets LEDs according to the state
void applyState(int s) {
  // kill all first
  allOff();

  // turn on by state
  if (s == 0) digitalWrite(LED1_PIN, HIGH);
  if (s == 1) digitalWrite(LED2_PIN, HIGH);
  if (s == 2) digitalWrite(LED3_PIN, HIGH);
  if (s == 3) digitalWrite(LED4_PIN, HIGH);
  if (s == 4) { digitalWrite(LED1_PIN, HIGH); digitalWrite(LED2_PIN, HIGH); }
  if (s == 5) { digitalWrite(LED3_PIN, HIGH); digitalWrite(LED4_PIN, HIGH); }
}

// blinks the current state's LEDs once (on time == off time)
// idea: they are already ON steady → go OFF for beat, then back ON for beat
void blinkCurrentStateOnce(unsigned long ms) {
  // s# = conditions when # led is on, s1 = led1 is on when stateIndex is either 0 or 4. and so on
  bool s1 = (stateIndex == 0) || (stateIndex == 4);
  bool s2 = (stateIndex == 1) || (stateIndex == 4);
  bool s3 = (stateIndex == 2) || (stateIndex == 5);
  bool s4 = (stateIndex == 3) || (stateIndex == 5);

  // turn them OFF for ms
  if (s1) digitalWrite(LED1_PIN, LOW);
  if (s2) digitalWrite(LED2_PIN, LOW);
  if (s3) digitalWrite(LED3_PIN, LOW);
  if (s4) digitalWrite(LED4_PIN, LOW);
  delay(ms);

  // turn them back ON for ms
  if (s1) digitalWrite(LED1_PIN, HIGH);
  if (s2) digitalWrite(LED2_PIN, HIGH);
  if (s3) digitalWrite(LED3_PIN, HIGH);
  if (s4) digitalWrite(LED4_PIN, HIGH);
  delay(ms);
}

void loop() {
  // read the pot to set blink speed
  potRaw = analogRead(POT_PIN);
  beatMs = map(potRaw, 0, 4095, 50, 1000);

  //  read buttons
  int blinkRead = digitalRead(BTN_BLINK);
  int nextRead  = digitalRead(BTN_NEXT);

  //  next button still only changes state once per press
  if (prevNextRead == HIGH && nextRead == LOW) {
    stateIndex++;
    if (stateIndex > 5) stateIndex = 0;
    applyState(stateIndex);
    delay(20);
  }

  //  blink button now blinks while held down
  if (blinkRead == LOW) {
    blinkCurrentStateOnce(beatMs);
  } else {
    applyState(stateIndex);
  }

  //  remember last button reads
  prevNextRead  = nextRead;
}