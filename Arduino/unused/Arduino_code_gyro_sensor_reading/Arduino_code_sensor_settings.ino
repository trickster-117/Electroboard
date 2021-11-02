#include<Wire.h>
const int MPU=0x68; 
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;


float pressure_front = 0;
float presure_back = 0;
float pressure_front_filtert = 0;
float presure_back_filtert = 0;
float arrays [20];
int counter = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B); 
  Wire.write(0);    
  Wire.endTransmission(true);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  pressure_front = analogRead(A0); //-900;//max(analogRead(A0)-900,0);
  
  presure_back = 1024 - pressure_front;
  //presure_back = analogRead(A1)-900;//max(analogRead(A1)-900,0);

  // filter
  arrays[counter] = pressure_front;
  counter = counter + 1;
  
  pressure_front_filtert = 0;
  for (int i=0; i <= 19; i++){
    pressure_front_filtert = pressure_front_filtert + (arrays[i]);
  }
  pressure_front_filtert = pressure_front_filtert / 20;
  presure_back_filtert = 1024 - pressure_front_filtert;

  if (counter == 19){
    counter = 0;
  }



  Wire.beginTransmission(MPU);
  Wire.write(0x3B);  
  Wire.endTransmission(false);
  Wire.requestFrom(MPU,12,true);  
  AcX=Wire.read()<<8|Wire.read();    
  AcY=Wire.read()<<8|Wire.read();  
  AcZ=Wire.read()<<8|Wire.read();  
  GyX=Wire.read()<<8|Wire.read();  
  GyY=Wire.read()<<8|Wire.read();  
  GyZ=Wire.read()<<8|Wire.read();  

  //Serial.print("front = "); Serial.print(pressure_front);
  //Serial.print("back = "); Serial.print(presure_back_filtert);
  //Serial.print(" | front filtert= "); Serial.print(pressure_front_filtert);
  Serial.print("difference= "); Serial.println(pressure_front_filtert-presure_back_filtert + 200);  
  //Serial.print(pressure_front-presure_back);

//  Serial.print("Accelerometer: ");
//  Serial.print("X = "); Serial.print(AcX);
//  Serial.print(" | Y = "); Serial.print(AcY);
//  Serial.print(" | Z = "); Serial.println(AcZ); 
  
//  Serial.print("Gyroscope: ");
//  Serial.print("X = "); Serial.print(GyX);
//  Serial.print(" | Y = "); Serial.print(GyY);
//  Serial.print(" | Z = "); Serial.println(GyZ);
//  Serial.println(" ");
  

  
  //delay(100);


}
