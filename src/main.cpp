#include <Arduino.h>
#include <Bounce2.h>
Bounce2::Button start_button = Bounce2::Button();
Bounce2::Button pump_button = Bounce2::Button();
Bounce2::Button vent_button = Bounce2::Button();
const int DEBOUNCE_TIME = 5;

const unsigned long SEC_TO_MIL = 1000UL;
const unsigned long PUMP_TIME = 75 * SEC_TO_MIL;
const unsigned long VENT_TIME = 13.5 * SEC_TO_MIL; //Calibrated for 25psi inlet pressure
const unsigned long IDLE_TIME = 2 * SEC_TO_MIL;
const unsigned long MAN_OP_TIME = 5 * SEC_TO_MIL;
const int NUM_CYCLES = 10;
const unsigned long EXTRA_VENT_TIME = 5 * SEC_TO_MIL;

const int STATUS = LED_BUILTIN;
const int PUMP = 8;
const int VENT = 9;
const int START = 10;
const int MAN_PUMP = 11;
const int MAN_VENT = 12;

// put function declarations here:
void do_cycle(unsigned long pump_time, unsigned long vent_time);

void setup() {
  Serial.begin(9600);
  pinMode(STATUS, OUTPUT);
  pinMode(PUMP, OUTPUT);
  pinMode(VENT, OUTPUT);
  start_button.attach(START, INPUT_PULLUP);
  start_button.interval(DEBOUNCE_TIME);
  start_button.setPressedState(LOW);
  vent_button.attach(MAN_VENT, INPUT_PULLUP);
  vent_button.interval(DEBOUNCE_TIME);
  vent_button.setPressedState(LOW); 
  pump_button.attach(MAN_PUMP, INPUT_PULLUP);
  pump_button.interval(DEBOUNCE_TIME);
  pump_button.setPressedState(LOW); 
}

void loop() {
  digitalWrite(STATUS, LOW);
  start_button.update(); //make sure status LED off
  if (start_button.pressed()) {
    Serial.print("Start Button Pressed\n");
    digitalWrite(STATUS, HIGH); //turn on status LED
    int i =0;
    while (i < NUM_CYCLES) {
      Serial.print("Running cycle: ");
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
    delay(IDLE_TIME);
    digitalWrite(STATUS, LOW); //turn off status LED
  }
  pump_button.update();
  if (pump_button.pressed()) {
    Serial.print("Pump Button Pressed\n");
    digitalWrite(PUMP, HIGH);
    delay(MAN_OP_TIME);
    digitalWrite(PUMP,LOW);
    delay(IDLE_TIME);
  }
  pump_button.update();
  if (vent_button.pressed()) {
    Serial.print("Vent Button Pressed\n");
    digitalWrite(VENT, HIGH);
    delay(MAN_OP_TIME);
    digitalWrite(VENT,LOW);
    delay(IDLE_TIME);
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