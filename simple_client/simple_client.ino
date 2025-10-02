#include "secrets.h"
#include <WiFiNINA.h>
#include <ArduinoHTTPClient>
#include <Arduino_JSON.h>

// de api declareren
const char api[] = "catfact.ninja";
const int api_port = 80;

// code uit de les, te vinden op: https://github.com/gdmgent-iot/arduino-network
WiFiServer server(5000);
WiFiClient wifi
HttpClient client(wifi, api, api_port);

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

void connectToServer() {
  if (WiFi.status() == WL_CONNECTED) {
    server.begin();
    Serial.println("TCP Server is actief op poort 5000");
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  Serial.println("Booting the arduino, hold your horses");
  delay(2000);

  connectToWiFi();
  connectToServer();
}

void loop() {

  listenToClientMessages();
  
}

void listenToCatFact() {
  
}


void listenToClientMessages() {
  wifi = server.available();

  if (wifi) {

    // get client ip address
    IPAddress clientIp = wifi.remoteIP();
    Serial.print("Client connected from IP: ");
    Serial.println(clientIp);

    while (wifi.connected()) {
      if (wifi.available()) {
        String msg = wifi.readStringUntil('\n');
        Serial.print("Bericht van client: ");
        Serial.println(msg);
      }
    }
  } else {
    wifi.stop();
    // Serial.print("." );
  }
}
