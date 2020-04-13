

#include <ArduinoOTA.h>

#include <ClusterDuck.h>

#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <WiFiClientSecure.h>
#include "timer.h"

#define SSID        "raspi-webgui"
#define PASSWORD    "ChangeMe"

#define ORG         ""                  // "quickstart" or use your organisation
#define DEVICE_ID   ""
#define DEVICE_TYPE "PAPA"                // your device type or not used for "quickstart"
#define TOKEN       ""      // your device token or not used for "quickstart"#define SSID        "nick_owl" // Type your SSID

char server[]           = ORG ".messaging.internetofthings.ibmcloud.com";
char topic[]            = "iot-2/evt/status/fmt/json";
char authMethod[]       = "use-token-auth";
char token[]            = TOKEN;
char clientId[]         = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;

// variables for papapi
char papapiServer[]           = "10.3.141.1";


ClusterDuck duck;

auto timer = timer_create_default(); // create a timer with default settings

WiFiClient wifiClient;
PubSubClient client(server, 8883, wifiClient);

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
  //setupMQTT();
  setupMQTTPapapi();

  int packetSize = LoRa.parsePacket();
  if (packetSize != 0) {
    byte whoIsIt = LoRa.peek();
    if(whoIsIt != ping) {
      Serial.println(packetSize);
      String * val = duck.getPacketData(packetSize);
      //quackJson();
    }
  }

  sendMQTTTestMessage();

  
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
  if (!!!client.connected()) {
    Serial.print("Reconnecting client to "); Serial.println(server);
    while ( ! (ORG == "quickstart" ? client.connect(clientId) : client.connect(clientId, authMethod, token)))
    {
      timer.tick(); //Advance timer to reboot after awhile
      Serial.print("i");
      delay(500);
    }
  }
}

void setupMQTTPapapi()
{
  papapiClient.setServer(papapiServer, 1883);
  papapiClient.setCallback(callback);
  if (!!!papapiClient.connected()) {
    Serial.print("Reconnecting client to "); Serial.println(papapiServer);
    while ( ! papapiClient.connect("me") )
    {
      timer.tick(); //Advance timer to reboot after awhile
      Serial.print("i");
      delay(500);
    }
  }
}

void quackJson() {
  const int bufferSize = 4 * JSON_OBJECT_SIZE(1);
  DynamicJsonBuffer jsonBuffer(bufferSize);

  JsonObject& root = jsonBuffer.createObject();

  Packet lastPacket = duck.getLastPacket();

  root["DeviceID"]        = lastPacket.senderId;
  root["MessageID"]       = lastPacket.messageId;
  root["Payload"]         = lastPacket.payload;

  root["path"]            = lastPacket.path + "," + duck.getDeviceId();

  String jsonstat;
  root.printTo(jsonstat);
  root.prettyPrintTo(Serial);

  if (client.publish(topic, jsonstat.c_str())) {
    Serial.println("Publish ok");
    root.prettyPrintTo(Serial);
    Serial.println("");
  }
  else {
    Serial.println("Publish failed");
  }
}

// Code below added for raspberry pi analytics test
void sendMQTTTestMessage(){
  delay(2000);
  const int bufferSize = 4 * JSON_OBJECT_SIZE(1);
  DynamicJsonBuffer jsonBuffer(bufferSize);

  JsonObject& root = jsonBuffer.createObject();

  //Packet lastPacket = duck.getLastPacket();

  root["DeviceID"]        = "Test";
  root["MessageID"]       = "Test";
  root["Payload"]         = "Test";

  root["path"]            = duck.getDeviceId();

  String jsonstat;
  root.printTo(jsonstat);
  root.prettyPrintTo(Serial);

  if (papapiClient.publish(topic, jsonstat.c_str())) {
    Serial.println("Publish ok");
    root.prettyPrintTo(Serial);
    Serial.println("");
  }
  else {
    Serial.println("Publish failed");
  }
}
void callback(char* topic, byte* payload, unsigned int length){
  Serial.println("Here");
  
  }
