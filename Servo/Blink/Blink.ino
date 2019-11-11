#define BLYNK_PRINT Serial
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SimpleTimer.h>
#include <DHT.h>
#include <Servo.h>
 

char auth[] = "JJ3I3JwbUeaCuaYNqqrw5aT34k3cyf9I";
 
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Zarif";
char pass[] = "3652929b524c";
 
#define DHTPIN 5          // D1
 
#define DHTTYPE DHT11     // DHT 11

 
DHT dht(DHTPIN, DHTTYPE);
SimpleTimer timer;

Servo servo;
 
// This function sends Arduino's up time every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit
 
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V5, t);
  Blynk.virtualWrite(V6, h);
}
 
void setup()
{
  // Debug console
  Serial.begin(9600);
 
  Blynk.begin(auth, ssid, pass);
  dht.begin();
  timer.setInterval(1000L, sendSensor);

  servo.attach(2); //D4
  servo.write(0);
  delay(2000);
}
 
void loop()
{
  Blynk.run();
  timer.run();
  float t = dht.readTemperature();
  
  if(t>25)
  {
    servo.write(90);
    delay(1000);
  }
  else
  {
    servo.write(0);
    delay(1000);
    }
  
  //
}
