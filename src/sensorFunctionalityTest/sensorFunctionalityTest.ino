#include <Wire.h>
#include <Adafruit_Sensor.h> 
#include <Adafruit_ADXL345_U.h>

#define STRETCH_PIN A0
#define FSR_PIN A5

Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified();

void setup(void) 
{
   Serial.begin(9600);  
   /*if(!accel.begin())
   {
    
      Serial.println("No ADXL345 sensor detected.");
      while(1);
   }*/
   Serial.write(1);

   
}
void loop(void) 
{
   //pressureTest();
   //stretchTest();
   //adxl345Test();
   Serial.write(1);
} 

void stretchTest(){
  int value;
  value = analogRead(STRETCH_PIN);
  Serial.print("Analog reading ");
  Serial.println(value);
  delay(50);
}

void adxl345Test(){
   sensors_event_t event; 
   accel.getEvent(&event);

   Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print("  ");
   Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print("  ");
   Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.print("  ");
   Serial.println("m/s^2 ");
   delay(500);
}

void pressureTest(){
  int fsrread;
  int fsrreading;
  // Read the FSR pin and store the output as fsrreading:
  fsrread = analogRead(FSR_PIN);
  // Print the fsrreading in the serial monitor:
  // Print the string "Analog reading = ".
  Serial.print("Analog reading = ");
  // Print the fsrreading:
  //I get readings between 90 and 700 so we will map to 0 - 1023
  fsrreading = map(fsrread, 90, 700, 0, 1023); 
  Serial.print(fsrreading);
  // We can set some threshholds to display how much pressure is roughly applied:
  if (fsrreading < 10) {
    Serial.println(" - No pressure");
  } else if (fsrreading < 200) {
    Serial.println(" - Light touch");
  } else if (fsrreading < 500) {
    Serial.println(" - Light squeeze");
  } else if (fsrreading < 800) {
    Serial.println(" - Medium squeeze");
  } else {
    Serial.println(" - Big squeeze");
  }
  delay(50); //Delay 500 ms.

}
