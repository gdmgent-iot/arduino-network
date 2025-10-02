#include "secrets.h"
#include <WiFiNINA.h>
#include <ArduinoHttpClient.h>
#include <Arduino_JSON.h>

// de api declareren
const char api[] = "official-joke-api.appspot.com";
const int api_port = 80;

// code uit de les, te vinden op: https://github.com/gdmgent-iot/arduino-network
WiFiServer server(5000);
WiFiClient wifi;
HttpClient http(wifi, api, api_port);

// interval en last request
unsigned long lastrequest = 0; // initieel is er nog geen last request
unsigned long interval = 10000; // = 10 seconden

// IP of another arduino
const char* other_arduino_ip = "192.168.1.126";


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

  listenToCatFact(); 
  listenToClientMessages();
}

void listenToCatFact() {
  
  // wacht tot interval gepasseerd is
  if(millis() - lastrequest < interval) return;
  lastrequest = millis(); // update lastrequest


  Serial.println("Requesting a fact from the API");

  // vraag een fact op
  http.get("/jokes/random");
  int statusCode = http.responseStatusCode();
  String response = http.responseBody();

  Serial.print("Status code: ");
  Serial.println(statusCode);

  if(statusCode == 200) {
    JSONVar jsonResponse = JSON.parse(response);
    String fact = (const char*) jsonResponse["setup"];
    fact += " - ";
    fact += (const char*) jsonResponse["punchline"];
    Serial.print("Fact: ");
    Serial.println(fact);

    // stuur de fact naar een andere arduino
    WiFiClient client;
    if (client.connect(other_arduino_ip, 5000)) {
      Serial.println("Connected to other arduino, sending fact");
      client.println(fact);
      client.stop();
    } else {
      Serial.println("Connection to other arduino failed");
    }
    
  } else {
    Serial.println("Error in retrieving fact");
  }
  http.stop();



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
