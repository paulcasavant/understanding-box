#include "WiFi.h"
#include <WebSocketsClient.h> // TODO: Is this the right one?
#include "password.h"
 
const int buttonPin = 14;
const int ledPin = 2;

const char* ssid = "FoxFi62";
const char* password =  WIFI_PASS;

int buttonState = 0;

WebSocketsClient webSocket;

void setup() {
 
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);

  // Connect to Wi-Fi
  Serial.begin(115200);
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  
  // Connected to Wi-Fi
  Serial.println("Connected to the WiFi network");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP()); 
 
 // Start WebSockets server
 webSocket.begin("192.168.43.131", 5000);
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
  }
  else
  {
    digitalWrite(ledPin, LOW);
  }
}