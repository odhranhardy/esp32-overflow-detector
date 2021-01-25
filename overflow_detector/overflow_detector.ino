#include "WiFi.h"
#include <WiFiClientSecure.h>

// Potentiometer is connected to GPIO 34 (Analog ADC1_CH6) 
const int detectorPin = 34;
const int LEDPin = 22;
const char ssid[] = "BT-9ZAJ5Z";
const char password[] =  "Enf7At4u7NtpVJ";
const char* host = "maker.ifttt.com";
const int httpsPort = 443;
const String url = "/trigger/ESP32_overflow/with/key/br73HW2dB1v93vjaqWpIHE";

// variable for storing the potentiometer value
int potValue = 0;
WiFiClientSecure client;



void setup() {
  // Set LEDPin to output
  pinMode(LEDPin, OUTPUT);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
    Serial.println(WiFi.status());
  }
  Serial.println("Connected to the WiFi network");

  Serial.print("connecting to ");
  Serial.println(host);
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }
}

void loop() {
 
  // Reading potentiometer value
  potValue = analogRead(detectorPin);
  Serial.println(potValue);
  if(potValue > 4000){
    digitalWrite(LEDPin, HIGH);
    call_webhook();
  }else{
    digitalWrite(LEDPin, LOW); 
  }
  delay(300000);
}

void call_webhook() {
  Serial.print("requesting URL: ");
  Serial.println(url);
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: BuildFailureDetectorESP32\r\n" +
               "Connection: close\r\n\r\n");
  Serial.println("request sent");

  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("headers received");
      break;
    }
  }
   String line = client.readStringUntil('\n');

  Serial.println("reply was:");
  Serial.println("==========");
  Serial.println(line);
  Serial.println("==========");
  Serial.println("closing connection");
}
