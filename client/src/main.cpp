#include "WiFi.h"
#include <WebSocketsClient.h>
#include "password.h"
 
const int BUTTON_PIN = 14;
const int LED_PIN = 2;
const char* SSID = "FoxFi62";
const char* PASSWORD =  WIFI_PASS;

int buttonState = 0;

WebSocketsClient webSocket;

void setup() {
 
  // Init LED and button
  pinMode(LEDD_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);

  // Connect to Wi-Fi
  Serial.begin(115200);
  WiFi.begin(SSID, PASSWORD);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  
  Serial.println("Connected to the WiFi network");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP()); 
 
 // Start WebSockets server
  webSocket.begin("192.168.43.131", 5000);
  webSocket.setReconnectInterval(5000);  // try ever 5000 again if connection has failed
}
 
void loop() 
{
  webSocket.loop();

  
  buttonState = digitalRead(buttonPin);

  if (buttonState)
  {
    digitalWrite(ledPin, HIGH);
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP()); 
    Serial.print("ESP Board MAC Address:  ");
    Serial.println(WiFi.macAddress());
    Serial.println(webSocket.isConnected());
    webSocket.sendTXT("{\"status\":\"OK\"}");
  }
  else
  {
    digitalWrite(ledPin, LOW);
  }
}