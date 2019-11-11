#include <DHT.h>  // Including library for dht
#include <ESP8266WiFi.h>
#include <WiFiClient.h>;
#include <ThingSpeak.h>;
#define DHTTYPE DHT11
 
WiFiClient client;
unsigned long myChannelNumber = 839378; //Your Channel Number (Without Brackets)
const char * myWriteAPIKey = "T0QF2I7SES1SLB7U";

const char *ssid =  "Zarif";     // replace with your wifi ssid and wpa2 key
const char *pass =  "3652929b524c";

//pin where the dht11 is connected
const int DHTPIN = 5;//D1 in nodemcu

//turbidity pin 
float val;
int TurbidityPin = A0;

//ultrasonic pin
const int trigger = 2;
const int echo = 0;
long T;
float distanceCM;

DHT dht(DHTPIN, DHT11);
 
void setup() 
{
       Serial.begin(115200);
       delay(10);
       pinMode(trigger, OUTPUT);
       pinMode(echo, INPUT);
       WiFi.mode(WIFI_STA);
       dht.begin();
 
       Serial.println("Connecting to ");
       Serial.println(ssid);
 
      // Connect to WiFi network
       WiFi.begin(ssid, pass);
 
      while (WiFi.status() != WL_CONNECTED) 
     {
            delay(500);
            Serial.print(".");
     }
      Serial.println("");
      Serial.println("WiFi connected");
      ThingSpeak.begin(client);
 
}
 
void loop() 
{
  
  float h = dht.readHumidity();
  float t = dht.readTemperature();
      
  if (isnan(h) || isnan(t)) 
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
 
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" degrees Celcius, Humidity: ");
  Serial.print(h);
  ThingSpeak.writeField(myChannelNumber, 1,t, myWriteAPIKey);
  ThingSpeak.writeField(myChannelNumber, 2,h, myWriteAPIKey);

  digitalWrite(trigger, LOW);
  delay(1);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);
  T = pulseIn(echo, HIGH);
  distanceCM = T * 0.034;
  Serial.print("Distance in cm: ");
  Serial.println(distanceCM);
  ThingSpeak.writeField(myChannelNumber, 3, distanceCM, myWriteAPIKey);
                      
  val = analogRead(TurbidityPin)/1024.0*5.0; //Read Analog values and Store in val variable
  Serial.println(val); //Print on Serial Monitor
  delay(1000);
  
            
  ThingSpeak.writeField(myChannelNumber, 4,val, myWriteAPIKey); //Update in ThingSpeak
  delay(100);
  
  // thingspeak needs minimum 15 sec delay between updates
  delay(1000);
}
