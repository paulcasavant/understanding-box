#include "WiFi.h"
#include <WebSocketsClient.h>
#include "password.h"
#include <WebServer.h>
#include <ESP32Ping.h>
 
const int BUTTON_PIN = 14;
const int LED_PIN = 2;
const char* SSID = "FoxFi62";
const char* PASSWORD =  WIFI_PASS;

int buttonState = 0;

WebSocketsClient webSocket;

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
  Serial.println("Websocket Event of type " + String(type));
  
  switch (type) {
    case WStype_DISCONNECTED:
      Serial.printf("[WSc] Disconnected!\n");
      break;
    case WStype_CONNECTED:
      Serial.printf("[WSc] Connected to url: %s\n",  payload);
      // send message to server when Connected
      webSocket.sendTXT("Connected");
      //webSocket.sendTXT("#E1");
      break;
    case WStype_TEXT:
      Serial.printf("[WSc] get text: %s\n", payload);
      // send message to server
      // webSocket.sendTXT("message here");
      break;
    // case WStype_BIN:
    //   Serial.printf("[WSc] get binary length: %u\n", length);
    //   hexdump(payload, length);
    //   // send data to server
    //   // webSocket.sendBIN(payload, length);
    //   break;
  }
}

void setup() {
 
  // Init LED and button
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);

  // Connect to Wi-Fi
  Serial.begin(115200);
  WiFi.begin(SSID, PASSWORD);
 
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  
  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("Connected to the WiFi network");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    // Start websocket
    delay(500);
    Serial.println("Starting WebSockets...");
    webSocket.begin("10.55.25.121", 8080, "/", "arduino");

    // Event handler
    webSocket.onEvent(webSocketEvent);
  }
}
 
void loop() 
{
  webSocket.loop();

  buttonState = digitalRead(BUTTON_PIN);

  if (buttonState)
  {
    digitalWrite(LED_PIN, HIGH);
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP()); 
    Serial.print("ESP Board MAC Address:  ");
    Serial.println(WiFi.macAddress());

    // Ping test
    // bool success = Ping.ping("10.55.25.121", 3);
    // if(!success){
    //   Serial.println("Ping failed");
    //   return;
    // }
    // Serial.println("Ping successful.");
  }
  else
  {
    digitalWrite(LED_PIN, LOW);
  }
}