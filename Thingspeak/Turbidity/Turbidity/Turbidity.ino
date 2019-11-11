#include <ESP8266WiFi.h>;
#include <WiFiClient.h>;
#include <ThingSpeak.h>;

const char* ssid = "TOTOLINK_N200RE"; //Your Network SSID
const char* password = "Roya10661220172"; //Your Network Password

int val;
int TurbidityPin = A0; //LDR Pin Connected at A0 Pin



WiFiClient client;
unsigned long myChannelNumber = 839378; //Your Channel Number (Without Brackets)
const char * myWriteAPIKey = "T0QF2I7SES1SLB7U"; //Your Write API Key

void setup()

{

  Serial.begin(115200);
  delay(10);
  
  // Connect to WiFi network
  
  WiFi.begin(ssid, password);
  ThingSpeak.begin(client);

}



void loop()

{

  val = analogRead(TurbidityPin)/1024.0*5.0; //Read Analog values and Store in val variable
  
  Serial.println(val); //Print on Serial Monitor
  delay(1000);
  
  ThingSpeak.writeField(myChannelNumber, 4,val, myWriteAPIKey); //Update in ThingSpeak
  delay(100);

}
