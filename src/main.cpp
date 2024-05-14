#include <Arduino.h>
#include <Bounce2.h>
Bounce2::Button button = Bounce2::Button();
const int DEBOUNCE_TIME = 5;

const unsigned long SEC_TO_MIL = 1000UL;
const unsigned long PUMP_TIME = 75 * SEC_TO_MIL;
const unsigned long VENT_TIME = 13.5 * SEC_TO_MIL; //Calibrated for 25psi inlet pressure
const unsigned long IDLE_TIME = 0.5 * SEC_TO_MIL;
const int NUM_CYCLES = 1;
const unsigned long EXTRA_VENT_TIME = 5 * SEC_TO_MIL;


const int PUMP = 8;
const int VENT = 9;
const int START = 10;

// put function declarations here:
void do_cycle(unsigned long pump_time, unsigned long vent_time);

void setup() {
  Serial.begin(9600);
  pinMode(PUMP, OUTPUT);
  pinMode(VENT, OUTPUT);
  button.attach(START, INPUT_PULLUP);
  button.interval(DEBOUNCE_TIME);
  button.setPressedState(LOW); 
}

void loop() {
  button.update();
  if (button.pressed()) {
    Serial.print("Button Pressed\n");
    int i =0;
    while (i < NUM_CYCLES) {
      Serial.print("Running cycle");
      Serial.print(i);
      Serial.print("\n");
      do_cycle(PUMP_TIME, VENT_TIME);
      i++;
    }
    Serial.print("Final Vent On\n");
    digitalWrite(VENT, HIGH);
    delay(EXTRA_VENT_TIME);
    Serial.print("Final Vent Off\n");
    digitalWrite(VENT, LOW);
  }
}

void do_cycle(unsigned long pump_time, unsigned long vent_time) {
  Serial.print("Pump On\n");
  digitalWrite(PUMP, HIGH);
  delay(PUMP_TIME);
  Serial.print("Pump Off\n");
  digitalWrite(PUMP, LOW);
  delay(IDLE_TIME);
  Serial.print("Vent On\n");
  digitalWrite(VENT, HIGH);
  delay(VENT_TIME);
  Serial.print("Vent Off\n");
  digitalWrite(VENT, LOW);
  delay(IDLE_TIME);
}