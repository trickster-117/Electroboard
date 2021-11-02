#include <Servo.h>
const uint8_t VescOutputPin = 3;
Servo esc;
int sensIn = A0;
int val = 0;

int duty_start = 1000;
int duty_end = 2000;
int duty_center = 1500;
int duty = duty_center;
int duty_step = 100;
int sign = 1;

bool do_stop = false;
bool cyclic_remote = true;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  esc.attach(VescOutputPin);
  esc.writeMicroseconds(1000);
  Serial.begin(9600); delay(10);
}

void loop() {

  // Run motor
  if (not do_stop) {
      esc.writeMicroseconds(duty);
//      Serial.print("duty cycle ");
//      Serial.println(duty);

  // Stop run, break motor
  } else {
      esc.writeMicroseconds(duty_center);
      digitalWrite(LED_BUILTIN, HIGH);
      delay(100);
      digitalWrite(LED_BUILTIN, LOW);
      delay(100);
  }

  // change speed by remote
  if (not cyclic_remote) {
    val = analogRead(sensIn);
    Serial.print("thumbstick ");
    Serial.println(val);
    duty = map(val, 0, 1023, duty_end, duty_start);
    // signal that run is still going
    digitalWrite(LED_BUILTIN, HIGH);
    delay(50);
    digitalWrite(LED_BUILTIN, LOW);
    delay(50);

  // change speed cyclic
  } else {
    // increase duty cycle
    duty = duty + sign * duty_step;
    
    // return when out of boundaries
    if (duty > duty_end or duty < duty_start) {
      //revert step and switch sign
      duty = duty - sign * duty_step;
      sign = -sign;
    }
    // signal that run is still going
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
  }
}
