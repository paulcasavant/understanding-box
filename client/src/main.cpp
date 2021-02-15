#include "WiFi.h"
#include "WiFiLogin.h" // Login information for WiFi network
#include <WebSocketsClient.h>

/* Declare constants */
const int BUTTON_PIN = 14;
const int LED_PIN = 2;

/* Initilize variables */
int buttonState = LOW;
int ledState = -1; // ledState < 0 if OFF, ledState > 0 if ON
long lastDebounceTime = 0;  // The last time the output pin was toggled
long debounceDelay = 200;    // The debounce time; increase if the output flickers

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
} /* webSocketEvent */

void setup() 
{ 
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
} /* setup */
 
void loop() 
{
  webSocket.loop();

  /* Sample button state */
  buttonState = digitalRead(BUTTON_PIN);

  /* Filter out any noise by setting a time buffer */
  if ( (millis() - lastDebounceTime) > debounceDelay) 
  {
    /* If the button has been pressed, lets toggle the LED from "off to on" or "on to off" */
    if ( (buttonState == HIGH) && (ledState < 0) ) 
    {
      webSocket.sendTXT("LED ON");
      digitalWrite(LED_PIN, HIGH); // Turn LED ON
      ledState = -ledState; // Now the LED is ON, change the state
      lastDebounceTime = millis(); // Set the current state
    }
    else if ( (buttonState == HIGH) && (ledState > 0) ) 
    {
      webSocket.sendTXT("LED OFF");
      digitalWrite(LED_PIN, LOW); // Turn LED OFF
      ledState = -ledState; // Now the LED is OFF, change the state
      lastDebounceTime = millis(); // Set the current time
    }
  }
} /* loop */