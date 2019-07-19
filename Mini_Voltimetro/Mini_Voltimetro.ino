



#include <SD.h>
#include <math.h>


#define pin_hot_side A5   //temp hot
#define pin_cold_side A6 //temp cold
#define p4 A4 //+ voltimetro
#define p1 A0 //Rele
#define p2 A1
#define p3 A2 //vin

const float R1 = 10000; // [ohm]
const float RL = 1.2; // [ohm]
const float ref_voltage = 3.3;// [V]
const float c1 = 0.9290328511e-03, c2 = 2.665840807e-04, c3 = -0.3884025758e-07;

float logR2, T,temp1,temp2;
File file_1,file_2,file_3,file_4,file_5;

void temp_procces(){
  
  int sensorValue = analogRead(pin_hot_side);//HOT SIDE
  int sensorValue1 = analogRead(pin_cold_side);//COLD SIDE
  
  float voltage = sensorValue * (ref_voltage / 1023.0);
  float voltage1 = sensorValue1 * (ref_voltage / 1023.0);
  
  float resistor= (R1 * voltage)/(ref_voltage- voltage);
  float resistor1= (R1 * voltage1)/(ref_voltage- voltage1);
  
  logR2 = log(resistor);
  temp1 = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));// Steinhart-Hart equation  __https://www.thinksrs.com/downloads/programs/therm%20calc/ntccalibrator/ntccalculator.html
  temp1 = temp1 - 273.15;
  
  logR2 = log(resistor1);
  temp2 = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));// Steinhart-Hart equation  __https://www.thinksrs.com/downloads/programs/therm%20calc/ntccalibrator/ntccalculator.html
  temp2 = temp2 - 273.15;
}  

void setup() {
  Serial.begin(115200);
  pinMode(p1, OUTPUT);
  digitalWrite(p1, HIGH);/*CLOSE CIRCUIT*/
  Serial.println("SDCARD_start");  
  analogReference (EXTERNAL);
  if(!SD.begin(7)){
      Serial.println("start failed");
      return;
    }
}

void loop() {
 
    temp_procces();
    float gradiente=temp1-temp2;

    int peltier = analogRead(p4);
    float tempo= millis();
    float Vout = peltier * (ref_voltage / 1023.0);
    
    int Vin=analogRead(p3);
    float VIN = Vin * (ref_voltage / 1023.0);
    file_1= SD.open("temp1.txt", FILE_WRITE);
    file_2= SD.open("temp2.txt", FILE_WRITE);
      file_3= SD.open("Vin.txt", FILE_WRITE);
      file_4= SD.open("Vout.txt", FILE_WRITE);  
      file_5= SD.open("time.txt", FILE_WRITE);
      
     file_1.println( String(temp1,6) );
     file_2.println( String(temp2,6) );
     file_3.println( String(VIN,6) );
     file_4.println( String(Vout,6) ); 
     file_5.println( String(tempo,6) );
    
     file_1.close();
     file_2.close();
     file_3.close();
     file_4.close();
     file_5.close();

    Serial.print("Vout-->"+String(Vout,6));
    Serial.print(" Vin-->"+String(VIN,6));
    Serial.print(" time-->"+String(tempo,6));
    Serial.println();

}
