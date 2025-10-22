/* MTEC 2280 - September 10 2025
Practice Sketch
*/

int loopCount = 0;
int ms = 500;

const int LED1_PIN = 2;         // LED1 connected to pin 2
const int LED2_PIN = 3;         // LED2 connected to pin 3
const int LED3_PIN = 4;         // LED3 connected to pin 4
const int LED4_PIN = 5;         // LED4 connected to pin 5

void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(115200); //115200 is BAUD rate i.e rate of communication.
  Serial.println("SETUP COMPLETE");
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);
  pinMode(LED4_PIN, OUTPUT);
  pinMode(LED5_PIN, OUTPUT);

}

void loop() 
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
