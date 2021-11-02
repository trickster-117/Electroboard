#include "HX711.h"
#include <Servo.h>

// TODO:
// Sensoren sind kacke: wir messen nicht Gewichtsverlagerung, sondern wie gut gerade der Fuß im Schuh auf dem Sensor steht. Wenn man mal ne gute Position gefunden hat passts auch ca.
// --> Sensoren mit Platte obendrauf = bessere Gewichtsverteilung, --> Sensoren zwischen Achsen und Board
// Board Bremst zu schnell, das bringt einen aus dem Takt und lässt den Fahrer quasi "aufschwingen": 
// --> Bremsen und Beschleunigen asymmetrisch anordnen. Erst Bremsen ab gewissem negativen Wert. Beschleunigen wie gehabt.
// Finetuning Zuordnung von Scalewerten (=val) und Motordutycycle (=duty)
// --> evtl. andere Kennlinie. z.B.: Quadratisch, dass man weniger stark das Gewicht verlagern muss für mehr Beschleunigen
// --> y = -(duty_max_delta/val_max^2) * (val-val_max)^2 + duty_max_delta. Für jede Halbebene (pos und negative val). Für negative dann jeweils beginnend bei -500 und gedreht

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 4;
const int LOADCELL_SCK_PIN = 5;
const int SCALE_START = -90;
const int SCALE_END = 90;
const int W_AVG = 1;
int val = 0;
HX711 scale;

// Motor
const uint8_t VescOutputPin = 3;
const int DUTY_START = 1200;
const int DUTY_END = 1800;
const int DUTY_CENTER = 1500;
int duty = DUTY_CENTER;
Servo esc;

void setup() {
  Serial.begin(9600);

  // Motor setup
  esc.attach(VescOutputPin);
  // begin standing still
  esc.writeMicroseconds(DUTY_CENTER);

  // serial setup
  Serial.begin(9600);

  // Sensor setup
  Serial.println("Initializing the scale");
  // Initialize library with data output pin, clock input pin and gain factor.
  // Channel selection is made by passing the appropriate gain:
  // - With a gain factor of 64 or 128, channel A is selected
  // - With a gain factor of 32, channel B is selected
  // By omitting the gain factor parameter, the library
  // default "128" (Channel A) is used here.
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  Serial.println("Before setting up the scale:");
  Serial.print("read: \t\t");
  Serial.println(scale.read());      // print a raw reading from the ADC
  Serial.print("read average: \t\t");
  Serial.println(scale.read_average(20));   // print the average of 20 readings from the ADC
  Serial.print("get value: \t\t");
  Serial.println(scale.get_value(5));   // print the average of 5 readings from the ADC minus the tare weight (not set yet)
  Serial.print("get units: \t\t");
  Serial.println(scale.get_units(5), 1);  // print the average of 5 readings from the ADC minus tare weight (not set) divided
            // by the SCALE parameter (not set yet)
  scale.set_scale(2280.f);                      // this value is obtained by calibrating the scale with known weights; see the README for details
  scale.tare();               // reset the scale to 0

  Serial.println("After setting up the scale:");
  Serial.print("read: \t\t");
  Serial.println(scale.read());                 // print a raw reading from the ADC
  Serial.print("read average: \t\t");
  Serial.println(scale.read_average(20));       // print the average of 20 readings from the ADC
  Serial.print("get value: \t\t");
  Serial.println(scale.get_value(5));   // print the average of 5 readings from the ADC minus the tare weight, set with tare()
  Serial.print("get units: \t\t");
  Serial.println(scale.get_units(5), 1);        // print the average of 5 readings from the ADC minus tare weight, divided
            // by the SCALE parameter set with set_scale
  Serial.println("Readings:");
}

void loop() {

  // get sensor value
  val = scale.get_units(W_AVG);

  // set duty cycle
  duty = map(val, SCALE_START, SCALE_END, DUTY_START, DUTY_END);

  Serial.println(duty);

  // run motor
  esc.writeMicroseconds(duty);
  
//  scale.power_down();             // put the ADC in sleep mode
//  delay(5000);
//  scale.power_up();
}
