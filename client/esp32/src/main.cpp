#include "WiFi.h"
 
const int buttonPin = 14;
const int ledPin = 2;

const char* ssid = "FoxFi62";
const char* password =  "babyman123";

int buttonState = 0;
 
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
 
  Serial.println("Connected to the WiFi network");

    // print your WiFi shield's IP address:
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP()); 
 
}
 
void loop() 
{
  buttonState = digitalRead(buttonPin);

  if (buttonState)
  {
    digitalWrite(ledPin, HIGH);
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP()); 
    Serial.print("ESP Board MAC Address:  ");
    Serial.println(WiFi.macAddress());
  }
  else
  {
    digitalWrite(ledPin, LOW);
  }
}