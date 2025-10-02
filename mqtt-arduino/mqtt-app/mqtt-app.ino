#include "secrets.h"
#include <WiFiNINA.h>
#include <ArduinoMqttClient.h>

// MQTT broker settings
const char* broker = "192.168.1.114";
const int port = 1883;
const char* topic = "dps1/nm/alert";

// CLIENTS
WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

void connectToWiFi() {
  int attempts = 0;

  // controleer of de connectie al in orde is, zoniet, maak connectie
  while (WiFi.status() != WL_CONNECTED && attempts < 5) {
    Serial.print("Verbinden met netwerk: ");
    Serial.println(WIFI_SSID);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    delay(2000);
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Connected :-)");
    Serial.println("IP-adres is: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("Kan niet verbinden...");
  }
}

void connectToMQTT() {
  Serial.print("Connecting to MQTT broker at ");
  Serial.print(broker);
  Serial.print(":");
  Serial.println(port);

  while (!mqttClient.connect(broker, port)) {
    Serial.print(".");
    delay(1000);
  }

  if (mqttClient.connected()) {
    Serial.println("\nConnected to MQTT broker!");
    // Subscribe to a topic
    mqttClient.subscribe(topic);
    // Send a message
    mqttClient.beginMessage(topic);
    mqttClient.print("Hello from Freddies Arduino MQTT client");
    mqttClient.endMessage();
  } else {
    Serial.println("\nFailed to connect to MQTT broker.");
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  Serial.println("Booting the arduino, hold your horses");
  delay(2000);

  connectToWiFi();
  connectToMQTT();
}

unsigned long lastPoll = 0;
const unsigned long pollInterval = 1000; // 1 seconde

void loop() {
  if (!mqttClient.connected()) {
    connectToMQTT();
    return;
  }

  mqttClient.poll();

  while (mqttClient.available()) {
    String message = mqttClient.readString();
    Serial.print("Received message: ");
    Serial.println(message);
  }
}
