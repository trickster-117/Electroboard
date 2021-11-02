#include <HX711_ADC.h>
#include <Servo.h>

//pins:
const int HX711_dout = 4; //mcu > HX711 dout pin
const int HX711_sck = 5; //mcu > HX711 sck pin
const uint8_t VescOutputPin = 3;

//HX711 constructor:
HX711_ADC LoadCell(HX711_dout, HX711_sck);

// initialize motor
Servo esc;

const int calVal_eepromAdress = 0;
unsigned long t = 0;
float var_speed = 1500;

void setup() {
  Serial.begin(9600); delay(10);

  //esc.attach(VescOutputPin);

  LoadCell.begin();
  float calibrationValue; // calibration value (see example file "Calibration.ino")
  calibrationValue = 696.0; // uncomment this if you want to set the calibration value in the sketch

  unsigned long stabilizingtime = 2000; // preciscion right after power-up can be improved by adding a few seconds of stabilizing time
  boolean _tare = true; //set this to false if you don't want tare to be performed in the next step
  LoadCell.start(stabilizingtime, _tare);
  if (LoadCell.getTareTimeoutFlag()) {
 //   Serial.println("Timeout, check MCU>HX711 wiring and pin designations");
    while (1);
  }
  else {
    LoadCell.setCalFactor(calibrationValue); // set calibration value (float)
//    Serial.println("Startup is complete");
  }
}

void loop() {
  static boolean newDataReady = 0;
  const int serialPrintInterval = 0; //increase value to slow down serial print activity

  // check for new data/start next conversion:
  if (LoadCell.update()) 
    newDataReady = true;

  // get smoothed value from the dataset:
  if (newDataReady) {
    if (millis() > t + serialPrintInterval) {
      float i = LoadCell.getData();

      // motor control
      // cut off to high numbers
//      if (i > 500){
//        i = 500;
//      }
//      else if (i < -500){
//        i = -500;
//      }

      // map working area 
//      if (i < -200){
//        var_speed = (i + 200) * (-1);
//        var_speed = var_speed/;
//      }
//      else if (i > 200){
//        var_speed = map(i , 200 , 500, 1500, 2500);
//      }
//      else{
//        var_speed = var_speed;
//      }
      //esc.writeMicroseconds(var_speed);

      
//      Serial.print(var_speed);
      Serial.print("\t");
      Serial.println(i);
      

      
      newDataReady = 0;
      t = millis();



    }
  }



}
