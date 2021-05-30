#include <ThingSpeak.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#include <SoftwareSerial.h>

#define WLAN_SSID       "sanjiva"
#define WLAN_PASS       "sanjiva1819"
#define brokerUser      "setikaraj@gmail.com"
#define brokerPass      "7505d6ae"

#define broker          "mqtt.dioty.co"
#define topic          "/setikaraj@gmail.com/smartEnergy"

SoftwareSerial transmit(14, 12); //D5=rx,D6=tx
long channel = 1391162;
unsigned int bulb1 = 1; //field names
const char* server = "api.thingspeak.com";
String WRITE_API_KEY = "HHVBIFFBRPNRVRTK";
int power,predicted;
float cost;
String crossed;

WiFiClient espclient;
PubSubClient client(espclient);
void connectback() {

  while (!client.connected()) {
    Serial.println("connecting to broker");
    if (client.connect("iot", brokerUser, brokerPass)) {

      Serial.println("connected ");
    }
    else {
      Serial.println("trying");
      delay(1000);
    }
  }

}

void setup() {
  pinMode(A0, INPUT);
  pinMode(D2, OUTPUT);
  pinMode(D4, OUTPUT);
  Serial.begin(9600);
  transmit.begin(9600);
  
  delay(1000);
  Serial.println("connecting to wifi....");
  Serial.println(WLAN_SSID);
  WiFi.begin(WLAN_SSID, WLAN_PASS);

  while (WiFi.status() != WL_CONNECTED)
  {
delay(500);
    Serial.print("........");
  }
  Serial.println("connected");
  client.setServer(broker, 1883);

  ThingSpeak.begin(espclient);

}

void loop() {

if (transmit.available()) {
power=transmit.read();
cost=power*0.31;
Serial.println(power);
if(cost>50){
   crossed="You crossed your budget limit!!";
  }
  else{
     crossed="";
    
    }


//publishing to mqtt broker

    String complete_msg = "Power usage=" + String(power) + "\nCost=" + String(cost) + "\n" + crossed;
    int len = complete_msg.length() + 1;
    char msg[len];
    complete_msg.toCharArray(msg, len);
    Serial.println(msg);
    if (!client.connected()) {
      connectback();
    }
    client.loop();
   

    Serial.println("sending");

   client.publish(topic, msg);
  }
if (espclient.connect(server, 80))  //   "184.106.153.149" or api.thingspeak.com
    {

     String postStr = WRITE_API_KEY ;
      postStr += "&field2=";
      postStr += String(power);
      
      postStr += "\r\n\r\n";

     espclient.print("POST /update HTTP/1.1\n");
      espclient.print("Host: api.thingspeak.com\n");
      espclient.print("Connection: close\n");
    espclient.print("X-THINGSPEAKAPIKEY: " + WRITE_API_KEY + "\n");
      espclient.print("Content-Type: application/x-www-form-urlencoded\n");
     espclient.print("Content-Length: ");
      espclient.print(postStr.length());
     espclient.print("\n\n");
espclient.print(postStr);
   }
   espclient.stop();

int after_bulb1 = ThingSpeak.readFloatField(channel, bulb1);

  //automated control
  if (after_bulb1 == 2) {
    int ldr = analogRead(A0);
    if (ldr < 327) {
      digitalWrite(D2, 0);
      digitalWrite(D4, 0);
      //active low connection of relay module
    }
    else {
      digitalWrite(D2, 1);
      digitalWrite(D4, 1);
    }
  }
  //OFF
  else if (after_bulb1 == 1) {
   //active low connection of relay module
   digitalWrite(D2, 0);
   digitalWrite(D4, 0);
 }
 //ON
  else if (after_bulb1 == 0) {
    digitalWrite(D2, 1);
    digitalWrite(D4, 1);
  }











}
