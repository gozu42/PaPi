#include <ClusterDuck.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <WiFiClientSecure.h>
#include "timer.h"

#define SSID        "raspi-webgui"
#define PASSWORD    "ChangeMe"

char topic[]            = "/";
char papapiServer[]           = "10.3.141.1";


ClusterDuck duck;

auto timer = timer_create_default(); // create a timer with default settings

WiFiClient wifiClient;

// papapi client
PubSubClient papapiClient(wifiClient);

byte ping = 0xF4;

void setup() {
  // put your setup code here, to run once:

  duck.begin();
  duck.setDeviceId("Papa");

  duck.setupLoRa();
  LoRa.receive();
  duck.setupDisplay("Papa");

  setupWiFi();
  
  Serial.println("PAPA Online");
}

void loop() {
  // put your main code here, to run repeatedly:
  if(WiFi.status() != WL_CONNECTED)
  {
    Serial.print("WiFi disconnected, reconnecting to local network: ");
    Serial.print(SSID);
    setupWiFi();

  }
  setupMQTT();

  int packetSize = LoRa.parsePacket();
  if (packetSize != 0) {
    byte whoIsIt = LoRa.peek();
    if(whoIsIt != ping) {
      Serial.println(packetSize);
      String * val = duck.getPacketData(packetSize);
        sendMQTTTestMessage();
    }
  }



  
  timer.tick();
}

void setupWiFi()
{
  Serial.println();
  Serial.print("Connecting to ");
  Serial.print(SSID);

  // Connect to Access Poink
  WiFi.begin(SSID, PASSWORD);

  while (WiFi.status() != WL_CONNECTED)
  {
    timer.tick(); //Advance timer to reboot after awhile
    //delay(500);
    Serial.print(".");
  }

  // Connected to Access Point
  Serial.println("");
  Serial.println("WiFi connected - PAPA ONLINE");
}


void setupMQTT()
{
  papapiClient.setServer(papapiServer, 1883);
  papapiClient.setCallback(callback);
  if (!!!papapiClient.connected()) {
    Serial.print("Reconnecting client to "); Serial.println(papapiServer);
    while ( ! papapiClient.connect("PAPI") )
    {
   
    }
  }
}



// Code below added for raspberry pi analytics test
void sendMQTTTestMessage(){
  const int bufferSize = 4*  JSON_OBJECT_SIZE(4);
  StaticJsonDocument<bufferSize> doc;

  JsonObject root = doc.as<JsonObject>();

  Packet lastPacket = duck.getLastPacket();

  doc["DeviceID"]        = lastPacket.senderId;
  doc["MessageID"]       = lastPacket.messageId;
  doc["Payload"]     .set(lastPacket.payload);
  doc["path"]         .set(lastPacket.path + "," + duck.getDeviceId());


  String jsonstat;
  serializeJson(doc, jsonstat);

  if (papapiClient.publish(topic, jsonstat.c_str())) {
    
    serializeJsonPretty(doc, Serial);
    Serial.println("");
    Serial.println("Publish ok");
   
  }
  else {
    Serial.println("Publish failed");
  }

}
void callback(char* topic, byte* payload, unsigned int length){
  
  }
