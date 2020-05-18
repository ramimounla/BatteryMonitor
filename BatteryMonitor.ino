#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>

#ifndef STASSID
#define STASSID ""
#define STAPSK  ""
#endif

const char* ssid = STASSID;
const char* password = STAPSK;


WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  // put your setup code here, to run once:
    pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, 1);
  Serial.begin(115200);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  WiFi.hostname("ledhost"); //set hostname
  
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  pinMode(D5, INPUT);
}

void loop() {
  client.loop();

  //TODO reading voltage is not correct
  raw = analogRead(D5);
  volt=raw/1023.0;
  volt=volt*3.3;

  Serial.print(String(volt));
  Serial.println("V");
}
