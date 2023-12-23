#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <PubSubClient.h>
#include <AsyncTCP.h>
#include "SPIFFS.h"
#include <DHT.h>


// Smartpot info
String potID = "smartpot/123";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

//***Set server***
const char* mqttServer = "broker.hivemq.com";
int port = 1883;
WiFiClient espClient;
PubSubClient client(espClient);

// Setup cloud server
const char* host = "api.thingspeak.com";
const char* apiKey = "E8VKD7CQPJCKZI3I";

// Search for parameter in HTTP POST request
const char* PARAM_INPUT_1 = "ssid";
const char* PARAM_INPUT_2 = "pass";

//Variables to save values from HTML form
String ssid;
String pass;

// File paths to save input values permanently
const char* ssidPath = "/ssid.txt";
const char* passPath = "/pass.txt";

IPAddress localIP;
//IPAddress localIP(192, 168, 1, 200); // hardcoded

// Set your Gateway IP address
IPAddress localGateway;
//IPAddress localGateway(192, 168, 1, 1); //hardcoded
IPAddress subnet(255, 255, 0, 0);

// Timer variables
unsigned long previousMillis = 0;
const long interval = 10000;  // interval to wait for Wi-Fi connection (milliseconds)

// Time between each request
const unsigned long requestTime = 15000;
unsigned long lastRequestTime = 0;

// Set up pin
const int ledPin = 2;
const int waterLevelPin = 34;
const int soilMoistPin = 35;
const int pumpPin = 13;
#define DHTPIN 4 
#define DHTTYPE DHT11
float humi = 0;
float temp = 0;
int waterLevel = 0;
int soilMoist = 0;

int waterLevelLimit = 2;
int soilMoistLimit = 0;

DHT dht(DHTPIN, DHTTYPE);

void setPin() {
  pinMode(ledPin, OUTPUT);
  pinMode(waterLevelPin, INPUT);
  pinMode(soilMoistPin, INPUT);
  pinMode(pumpPin, OUTPUT);
  dht.begin();
}

void setup() {
  // Serial port for debugging purposes
  Serial.begin(115200);

  initSPIFFS();
  // Load values saved in SPIFFS
  ssid = readFile(SPIFFS, ssidPath);
  pass = readFile(SPIFFS, passPath);

  Serial.println(ssid);
  Serial.println(pass);


  client.setServer(mqttServer, 1883);
  client.setCallback(callback);
  client.setKeepAlive( 300 );
  if(initWiFi()) {
    Serial.println("Setting up pin");
  }
  else {
    noWifiMode();
    Serial.println("Setting up pin");
  }
  setPin();
}

void loop() {
  if(!client.connected()) {
    mqttConnect();
  }
  client.loop();

  waterLevel = readWater();
  soilMoist = readMoist();

  humi = dht.readHumidity();
  temp = dht.readTemperature();

  sendRequest();
  // Serial.println(waterLevel);
  // Serial.println(soilMoist);

  pumpWater();

  // Only blinks when connected to Wifi
  if(WiFi.status() == WL_CONNECTED) {
    digitalWrite(ledPin, HIGH);
    delay(1000);
    digitalWrite(ledPin, LOW);
    delay(1000);
  }else {
    delay(2000);
  }
}
