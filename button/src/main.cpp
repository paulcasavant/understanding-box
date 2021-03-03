#include "WiFi.h"
#include "WiFiLogin.h" // Login information for WiFi network
#include <WebSocketsClient.h>
#include <ArduinoJson.h>

/* Declare constants */
const int SWITCH_PIN = 14;
const int LED_PIN = 2;
const int BAUD_RATE = 115200;

/* Switch variables */
int state = HIGH; // the current state of the output pin
int reading; // the current reading from the input pin
int previous = LOW; // the previous reading from the input pin
long millisTime = 0; // the last time the output pin was toggled
long debounce = 200; // the debounce time, increase if the output flickers

/* JSON data payload */
StaticJsonDocument<200> doc; // TODO: Consider resizing
String stringJSON;

WebSocketsClient webSocket;

/**
 * A websocket event handler.
 * 
 * @param type the type of event
 * @param payload the payload sent
 * @param length size of payload
 */ 
void webSocketEvent(WStype_t type, uint8_t * payload, size_t length)
{
  switch (type) 
  {
    case WStype_ERROR:
      break;

    case WStype_DISCONNECTED:
      Serial.printf("[WebSocketsClient] Disconnected!\n");
      break;

    case WStype_CONNECTED:
      Serial.printf("[WebSocketsClient] Connected to url: %s\n",  payload);
      
      /* Send message to the server when connected */
      //webSocket.sendTXT("Client connected");
      break;

    case WStype_TEXT:
      Serial.printf("[WebSocketsClient] Received: %s\n", payload);
      
      /* Send message to the server */
      //webSocket.sendTXT("message here");
      break;

    case WStype_BIN:
      Serial.printf("[WebSocketsClient] Get binary length: %u\n", length);
      break;

    case WStype_FRAGMENT_TEXT_START:
      Serial.printf("[WebSocketsClient] Event: WStype_FRAGMENT_TEXT_START\n");
      break;

    case WStype_FRAGMENT_BIN_START:
      Serial.printf("[WebSocketsClient] Event: WStype_FRAGMENT_BIN_START\n");
      break;

    case WStype_FRAGMENT:
      Serial.printf("[WebSocketsClient] Event: WStype_FRAGMENT\n");
      break;

    case WStype_FRAGMENT_FIN:
      Serial.printf("[WebSocketsClient] Event: WStype_FRAGMENT_FIN\n");
      break;

    case WStype_PING:
      Serial.printf("[WebSocketsClient] Event: WStype_PING\n");
      break;

    case WStype_PONG:
      Serial.printf("[WebSocketsClient] Event: WStype_PONG\n");
      break;
  }
} /* webSocketEvent */

void setup() 
{ 
  /* Initialize pins */
  pinMode(LED_PIN, OUTPUT);
  pinMode(SWITCH_PIN, INPUT);

  /* Serialize JSON data */
  doc["device"] = "button";
  doc["identifier"] = 1;
  serializeJson(doc, stringJSON);

  Serial.printf("[ESP] MAC Address: %s\n", WiFi.macAddress().c_str());
  
  /* Initialize serial connection */
  Serial.begin(BAUD_RATE);
  Serial.printf("[ESP] Baud Rate: %d\n", BAUD_RATE);

  /* Print MAC address */
  Serial.printf("[ESP] MAC Address: %s\n", WiFi.macAddress().c_str());

  /* Initialize WPA2 WiFi connection */
  WiFi.begin(WIFI_SSID, WIFI_PASS);
 
  /* Print status message until connected */
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.printf("[WiFi] Connecting...\n");
  }
  
  /* Once connected, print info and start WebSockets */
  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.printf("[WiFi] Connection Success: %s\n", WIFI_SSID);
    Serial.printf("[WiFi] IP Address: %s\n", WiFi.localIP().toString().c_str());

    /* Start WebSockets */
    delay(500);
    Serial.println("[WebSocketsClient] Initializing...");
    webSocket.begin("10.55.11.213", 8080, "/", "arduino");
    Serial.println("[WebSocketsClient] Started");

    /* Assign event WebSockets event handler */
    webSocket.onEvent(webSocketEvent);
  }


} /* setup */
  

void loop() 
{
  webSocket.loop();

  reading = digitalRead(SWITCH_PIN);

  // if the input just went from LOW and HIGH and we've waited long enough
  // to ignore any noise on the circuit, toggle the output pin and remember
  // the time
  if (reading == HIGH && previous == LOW && millis() - millisTime > debounce) {
    if (state == HIGH)
    {
      state = LOW;
      webSocket.sendTXT(stringJSON);
    }
    else
    {
      state = HIGH;
      webSocket.sendTXT("CONFUSED");
    }

    millisTime = millis();    
  }

  digitalWrite(LED_PIN, state);

  previous = reading;
} /* loop */