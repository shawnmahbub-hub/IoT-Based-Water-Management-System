#define BLYNK_PRINT Serial
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

//give authentication code here
char auth[]="ba4dce62000e4554a9f95d69abb65bb8";

//give the wifi ssid and password
char ssid[]="TOTOLINK_N200RE";
char pass[]="Roya10661220172";

BlynkTimer timer;

#define TurbidityPin A0

//int sensorData;
float TurbidityValue;

void sendAnalog()
{
  //sensorData = analogRead(A0); //reading the sensor on A0
  TurbidityValue=(analogRead(A0)*5.0)/1024.0;//convert the analog readings
  Blynk.virtualWrite(V8  , TurbidityValue); //sending to Blynk
}

void setup() {
  Serial.begin(9600);
  
  Blynk.begin(auth, ssid, pass);
  //sensors.begin();
  timer.setInterval(1000L, sendAnalog);

}


void loop() {
  Blynk.run();
  timer.run(); 
  Serial.println(TurbidityValue);
  delay(1000);

}
