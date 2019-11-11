#define BLYNK_PRINT Serial
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#include <SimpleTimer.h>

//authentication code
char auth[]="KNo03uafmSGgRf3-nx4wzM27BorkAPI8";

//wifi ssid and password
char ssid[]="TOTOLINK_N200RE";
char pass[]="Roya10661220172";

//flow sensor input pino
byte sensorInterrupt = 0;
byte flowPin = D2;   //NodeMCU D2
int flowRate, count=1;

SimpleTimer timer;

void setup() 
{
  pinMode(flowPin,INPUT);
  digitalWrite(flowPin, HIGH);
  //attachInterrupt(4, Flow, RISING);
  Serial.begin(115200);
  delay(10);
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, pass);
  int wifi_ctr=0;
  while(WiFi.status() !=WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");

  Blynk.begin(auth,ssid,pass);
  pinMode(D2, INPUT);
  attachInterrupt(D2, Flow, RISING);
}

void loop() {
    Blynk.run();//initiates blynk
    count=0;
    detachInterrupt(sensorInterrupt);

    //flowMath
    flowRate=(count * 2.25);
    flowRate=flowRate * 60;
    flowRate= flowRate /1000;

    Serial.println(flowRate);
    Serial.print("L/min");
    Blynk.virtualWrite(V1, flowRate);
    attachInterrupt(sensorInterrupt, Flow, FALLING);  
}

void Flow()
{
  count++;
}
