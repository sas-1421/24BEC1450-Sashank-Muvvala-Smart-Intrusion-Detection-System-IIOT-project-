#include <WiFi.h>
#include <HTTPClient.h>

// WiFi credentials - Wokwi's built-in simulated WiFi
const char* ssid     = "Wokwi-GUEST";
const char* password = "";

// ThingSpeak
const char* apiKey   = "API KEY";  
const char* server   = "http://api.thingspeak.com/update";

const int PIR_PIN = 13;
const int LED_PIN = 2;

void setup() {
  Serial.begin(115200);
  pinMode(PIR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected!");
  delay(2000); // PIR warmup
}

void loop() {
  int motion = digitalRead(PIR_PIN);

  if (motion == HIGH) {
    Serial.println("ALERT: Motion Detected! Sending to ThingSpeak...");
    digitalWrite(LED_PIN, HIGH);
    sendToThingSpeak(1);  // 1 = motion
    delay(15000); // ThingSpeak free tier limit: 1 update per 15 sec
  } else {
    Serial.println("No motion.");
    digitalWrite(LED_PIN, LOW);
    sendToThingSpeak(0);  // 0 = clear
    delay(15000);
  }
}

void sendToThingSpeak(int value) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = String(server) + "?api_key=" + apiKey + "&field1=" + value;
    http.begin(url);
    int httpCode = http.GET();
    if (httpCode > 0) {
      Serial.println("ThingSpeak updated! Code: " + String(httpCode));
    } else {
      Serial.println("Failed to send.");
    }
    http.end();
  }
}
