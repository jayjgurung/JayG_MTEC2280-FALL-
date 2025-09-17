/* MTEC 2280 - September 17 2025
4 led blinking Sketch
*/

int ms = 300;
int state = 1;

const int LED1_PIN = 9;         // LED1 connected to pin 2
const int LED2_PIN = 10;         // LED2 connected to pin 3
const int LED3_PIN = 11;         // LED3 connected to pin 4
const int LED4_PIN = 12;         // LED4 connected to pin 5

void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(115200); //115200 is BAUD rate i.e rate of communication.
  Serial.println("SETUP COMPLETE");
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);
  pinMode(LED4_PIN, OUTPUT);

}

void loop() 
{
  if (state == 1)
  {
    digitalWrite(LED1_PIN, HIGH);
    delay(ms);
    digitalWrite(LED1_PIN, LOW);

    digitalWrite(LED2_PIN, HIGH);
    delay(ms);
    digitalWrite(LED2_PIN, LOW);

    digitalWrite(LED3_PIN, HIGH);
    delay(ms);
    digitalWrite(LED3_PIN, LOW);

    digitalWrite(LED4_PIN, HIGH);
    delay(ms);
    digitalWrite(LED4_PIN, LOW);
  }
  if (state == 2)
  {
    digitalWrite(LED4_PIN, HIGH);
    delay(ms);
    digitalWrite(LED4_PIN, LOW);

    digitalWrite(LED3_PIN, HIGH);
    delay(ms);
    digitalWrite(LED3_PIN, LOW);
    
    digitalWrite(LED2_PIN, HIGH);
    delay(ms);
    digitalWrite(LED2_PIN, LOW);
    
    digitalWrite(LED1_PIN, HIGH);
    delay(ms);
    digitalWrite(LED1_PIN, LOW);

  }
  if (state == 3)
  {
    digitalWrite(LED1_PIN, HIGH);
    digitalWrite(LED2_PIN, HIGH);
    digitalWrite(LED3_PIN, HIGH);
    digitalWrite(LED4_PIN, HIGH);
    delay(ms);
    digitalWrite(LED1_PIN, LOW);
    digitalWrite(LED2_PIN, LOW);
    digitalWrite(LED3_PIN, LOW);
    digitalWrite(LED4_PIN, LOW);
  }
  if (state == 4)
  {
    digitalWrite(LED1_PIN, HIGH);
    digitalWrite(LED2_PIN, HIGH);
    delay(ms);
    digitalWrite(LED1_PIN, LOW);
    digitalWrite(LED2_PIN, LOW);
    delay(ms);
    digitalWrite(LED3_PIN, HIGH);
    digitalWrite(LED4_PIN, HIGH);
    delay(ms);
    digitalWrite(LED3_PIN, LOW);
    digitalWrite(LED4_PIN, LOW);
  }
  if (state == 4)
  {
    digitalWrite(LED1_PIN, HIGH);
    digitalWrite(LED2_PIN, HIGH);
    delay(ms);
    digitalWrite(LED1_PIN, LOW);
    digitalWrite(LED2_PIN, LOW);
    delay(ms);
    digitalWrite(LED3_PIN, HIGH);
    digitalWrite(LED4_PIN, HIGH);
    delay(ms);
    digitalWrite(LED3_PIN, LOW);
    digitalWrite(LED4_PIN, LOW);
  }
  if (state == 5)
  {
    digitalWrite(LED1_PIN, HIGH);
    digitalWrite(LED3_PIN, HIGH);
    delay(ms);
    digitalWrite(LED1_PIN, LOW);
    digitalWrite(LED3_PIN, LOW);
    delay(ms);
    digitalWrite(LED2_PIN, HIGH);
    digitalWrite(LED4_PIN, HIGH);
    delay(ms);
    digitalWrite(LED2_PIN, LOW);
    digitalWrite(LED4_PIN, LOW);
  }
  if (state == 6)
  {
    ms = 100;
    digitalWrite(LED1_PIN, HIGH);
    delay(ms);
    digitalWrite(LED2_PIN, HIGH);
    digitalWrite(LED1_PIN, LOW);
    delay(ms);
    digitalWrite(LED2_PIN, LOW);
    digitalWrite(LED3_PIN, HIGH);
    delay(ms);
    digitalWrite(LED4_PIN, HIGH);
    digitalWrite(LED3_PIN, LOW);
    delay(ms);
    digitalWrite(LED4_PIN, LOW);
  }

  // FRIST LED TURNS OF 1 SECOND BEFORE THE STATE SWITCHES
  digitalWrite(LED1_PIN, HIGH);
  delay(1000);
  digitalWrite(LED1_PIN, LOW);
  state++;
  if (state>6){
    state = 1;
  }
}
