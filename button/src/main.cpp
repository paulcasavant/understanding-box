#include "WiFi.h"
#include "WiFiLogin.h" // Login information for WiFi network
#include <WebSocketsClient.h>
#include <ArduinoJson.h>

/* Declare constants */
const int SWITCH_PIN = 14;
const int LED_PIN = 2;
const int BAUD_RATE = 115200;
const int SERVER_PORT = 8080;
const char* SERVER_IP = "";

/* The number of times the switch must read HIGH before sending
a WebSockets message notifying the server to change state to HIGH. */
const int highCount = 500; 

/* Delay determines the rate at which WebSockets messages are 
dispatched to the server */
int msgDelay = 300;

/* Counts the number iterations the switch is
HIGH without going LOW */
int count = 0; 

int reading = LOW; // Current reading from the switch
int lastTime = 0; // The last time a message was sent

/* JSON data payload */
StaticJsonDocument<200> doc;
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
    webSocket.begin(SERVER_IP, SERVER_PORT, "/", "arduino");
    Serial.println("[WebSocketsClient] Started");

    /* Assign event WebSockets event handler */
    webSocket.onEvent(webSocketEvent);
  }
} /* setup */
  
void loop()
{
  webSocket.loop();

  /* Read the current status of the switch */
  reading = digitalRead(SWITCH_PIN);

  /* If switch is HIGH, increment a count */
  if (reading)
  {
    count++;
  }
  /* Otherwise, reset the count */
  else
  {
    count = 0;
  }

  /* If the message delay time has passed, send 
  a message. Limiting messages may not be necessary,
  however, there is no benefit to sending one every
  loop. */
  if (millis() - lastTime > msgDelay)
  {
    /* If the switch is HIGH and has been high
    for the required number of iterations, send
    UNDERSTAND. This ensures that a LOW will not
    be mistaken for a HIGH due to switch bouncing. */
    if (reading && count > highCount)
    {
      webSocket.sendTXT("understand");
    }
    /* Otherwise, send CONFUSED */
    else
    {
      webSocket.sendTXT("confused");
    }

    /* Sample the current time */
    lastTime = millis();
  }

} /* loop */