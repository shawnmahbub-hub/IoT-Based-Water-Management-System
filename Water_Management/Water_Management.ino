#define BLYNK_PRINT Serial
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SimpleTimer.h>
#include <DHT.h>       // including the library of DHT11 temperature and humidity sensor
#include <Servo.h>
#define DHTTYPE DHT11   // DHT 11

//give authentication code here
char auth[]="ba4dce62000e4554a9f95d69abb65bb8";

//give the wifi ssid and password
char ssid[]="TOTOLINK_N200RE";
char pass[]="Roya10661220172";

//BlynkTimer timer;

//defining gpio pin for ultrasonic sensor
#define TRIGGER 2 //D4 in nodeMCU
#define ECHO    0 //D3 in nodeMCU

//turbidity pin
float TurbidityValue;
#define TurbidityPin A0

//servo
Servo servo;

// DHT Sensor
const int DHTPin = 5;//D1 in nodemcu
// Initialize DHT sensor.
DHT dht(DHTPin, DHTTYPE);

SimpleTimer timer;

//part for sending sensor value to the blynk application
void sendSensor()
{
  float h=dht.readHumidity();
  float t=dht.readTemperature();

  //checking if the sensor is faled to send the value
  if(isnan(h)|| isnan(t))
  {
    Serial.println("Failed to read from DHT sensor");
    return;  
  }
  if(t>=30)
  {
    Blynk.virtualWrite(V4, "Risk of Resin");   
  }
  
  //sending the value of the sensor from dht11
  Blynk.virtualWrite(V6,h);
  Blynk.virtualWrite(V7,t);
}

void sendAnalog()
{
  //sensorData = analogRead(A0); //reading the sensor on A0
  //Voltage=0.004888*analogRead(TurbidityPin);
  TurbidityValue=analogRead(TurbidityPin)/1024.0*5.0;//convert the analog readings

  if (TurbidityValue <= 4.4 && TurbidityValue >= 3.51){
        Blynk.email("md.mahbub.rahman.shwon@g.bracu.ac.bd","Water needs to be filtered");    
    }
    else if(TurbidityValue <= 3.50 && TurbidityValue >= 0){
        Blynk.email("md.mahbub.rahman.shwon@g.bracu.ac.bd","Water tanks needs to be cleaned right now.");    
    }
    else {
         Blynk.email("md.mahbub.rahman.shwon@g.bracu.ac.bd","Water is safe to drink");
    }
    
  Blynk.virtualWrite(V8 , TurbidityValue); //sending to Blynk
}

void setup(void)
{ 
  
  Serial.begin(115200);
  Blynk.begin(auth,ssid,pass); //calling blynk method with parameter
  
  dht.begin(); //initiates to take the value from environment

  //setting pin modes for ultrasonic sensor
  pinMode(TRIGGER, OUTPUT);
  pinMode(ECHO, INPUT);

  servo.attach(16); //D0
  servo.write(0);
  delay(2000);

  //setup a function to be called every second
  timer.setInterval(100L,sendSensor);
  //sensors.begin();
  timer.setInterval(1000L, sendAnalog);

  //printing value for serial monitor
  Serial.println("Humidity and temperature \n\n");
  Serial.println("==========================================");
  delay(1000);
}

void loop() 
{
    //dht11 part
    float hum = dht.readHumidity();
    float temp = dht.readTemperature();

    //ultrasonic part
    long duration, distance;
    digitalWrite(TRIGGER, LOW);  
    delayMicroseconds(2); 
      
    digitalWrite(TRIGGER, HIGH);
    delayMicroseconds(10); 
      
    digitalWrite(TRIGGER, LOW);
    duration = pulseIn(ECHO, HIGH);
    distance = (duration/2) / 29.1;
    //int counter=1;

    //0-20%
    if (distance <= 24    && distance >=17){
       servo.write(90);
       Blynk.virtualWrite(V0, 255);
       Blynk.email("md.mahbub.rahman.shwon@g.bracu.ac.bd","Water Level Alert!!!","No need to Switch OFF.");        
    }
    
    else{
        Blynk.virtualWrite(V0, 0);
    }

    //75%
    if (distance <= 16 && distance >=11){
        Blynk.virtualWrite(V1, 255);
        Blynk.email("md.mahbub.rahman.shwon@g.bracu.ac.bd","Water Level Alert!!!","It will be better if you switch OFF, unless it's OK.");    
    }
    
    else {
        Blynk.virtualWrite(V1, 0);
    }

    //50%
    if (distance <= 10 && distance >=6) {
        Blynk.virtualWrite(V2, 255);
        Blynk.email("md.mahbub.rahman.shwon@g.bracu.ac.bd","Water Level Alert!!!","Water level is filed up by 50% and it is reaching to 75% , would you like to Switch OFF?");
    }
    
    else {
        Blynk.virtualWrite(V2, 0);
    }

    //90%
     
     if (distance <= 5 && distance >=0) 
     {
        
          //counter +=counter;
     
         servo.write(180);
         delay(1000);
         Blynk.virtualWrite(V3, 255);
        Blynk.email("md.mahbub.rahman.shwon@g.bracu.ac.bd","Water Level Alert!!!","Emergency!You must need to Switch OFF.");  
        
//        Blynk.virtualWrite(V9, counter);
//        servo.write(180);
//        delay(1000);
        
     }
     
     
     else{
        delay(1000);
        Blynk.virtualWrite(V3, 0);
     }
     
     //counter +=counter;
     //..................................................................................................//
      
    //printing value for serial monitor of dht11
    Serial.print("Current humidity = ");
    Serial.print(hum);
    Serial.print("%  ");
    Serial.print("temperature = ");
    Serial.print(temp); 
    Serial.println("C  ");
    
    //printing value for serial monitor of ultrasonic sensor
    Serial.print(distance);
    Serial.println("Centimeter:");
    
    //blynk part for ultrasonic sensor
    Blynk.virtualWrite(V5, distance);
    delay(1000);

    //blynk
    Blynk.run();//initiates blynk
    timer.run();//initiates blynk
    Serial.println(TurbidityValue);
    delay(1000);
}
