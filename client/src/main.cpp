#include "WiFi.h"
#include "WiFiLogin.h" // Login information for WiFi network
#include <WebSocketsClient.h>

/* Declare constants */
const int BUTTON_PIN = 14;
const int LED_PIN = 2;

/* Initilize variables */
int buttonState = 0;

WebSocketsClient webSocket;

/**
 * A websocket event handler.
 * 
 * @param type the type of event
 * @param payload the payload sent
 * @param length size of payload
 */
void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
  switch (type) {
    case WStype_ERROR:
      break;

    case WStype_DISCONNECTED:
      Serial.printf("[WebSocketsClient] Disconnected!\n");
      break;

    case WStype_CONNECTED:
      Serial.printf("[WebSocketsClient] Connected to url: %s\n",  payload);
      
      /* Send message to the server when connected */
      webSocket.sendTXT("Client connected");
      break;

    case WStype_TEXT:
      Serial.printf("[WebSocketsClient] Get text: %s\n", payload);
      
      /* Send message to the server */
      webSocket.sendTXT("message here");
      break;

    case WStype_BIN:
      Serial.printf("[WebSocketsClient] Get binary length: %u\n", length);
      // hexdump(payload, length);
      
      /* Send data to server */
      // webSocket.sendBIN(payload, length);
      break;

    case WStype_FRAGMENT_TEXT_START:
      break;

    case WStype_FRAGMENT_BIN_START:
      break;

    case WStype_FRAGMENT:
      break;

    case WStype_FRAGMENT_FIN:
      break;

    case WStype_PING:
      break;

    case WStype_PONG:
      break;
  }
}

void setup() {
 
  /* Initialize pins */
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);

  /* Connect to Wi-Fi network */
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
 
  /* Print status message until connected */
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.println("[WiFi] Connecting...");
  }
  
  /* Once connected, print info and start WebSockets */
  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.printf("[WiFi] Connection Success: %s\n", WIFI_SSID);
    Serial.printf("[WiFi] IP Address: %s\n", WiFi.localIP().toString().c_str());

    /* Start WebSockets */
    delay(500);
    Serial.println("[WebSocketsClient] Initializing...");
    webSocket.begin("192.168.43.131", 8080, "/", "arduino");
    Serial.println("[WebSocketsClient] Started");

    /* Assign event WebSockets event handler */
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
      webSocket.sendTXT("Button pressed!");
      delay(500)
  }
  else
  {
    digitalWrite(LED_PIN, LOW);
  }
}