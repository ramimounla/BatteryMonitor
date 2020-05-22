#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>

#ifndef STASSID
#define STASSID ""
#define STAPSK  ""
#define WRITEKEY  ""
#endif

const char* ssid = STASSID;
const char* password = STAPSK;
const String writeKey = WRITEKEY;

float analogValue;
float inputVoltage;

WiFiClient espClient;
HTTPClient http;

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

  Serial.println("Ready to read");
}

void loop() {
  Serial.println("Reading");
  
  analogValue = 0;

  for(int i = 0 ; i < 10 ; i++){
    analogValue =  analogValue + analogRead(A0);
    delay(100); 
  }
  inputVoltage = (analogValue / 10) * (4.7 / 1023.00);

  Serial.print(analogValue/10);
  Serial.println(" Read");
  
  Serial.print(inputVoltage);
  Serial.println("V");

  std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
  client->setInsecure();

  String apiUrl = "https://api.thingspeak.com/update.json?api_key=" + writeKey + "&field3=" + inputVoltage;
  
  HTTPClient http;    //Declare object of class HTTPClient
  http.begin(*client, apiUrl);     //Specify request destination

  Serial.println("Request:");
  Serial.println(apiUrl);
              
  Serial.println("Sending request");
  int httpCode = http.GET();

  Serial.print("Resutl is: ");
  Serial.println(httpCode);

  http.end();  //Close connection

  digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level
  delay(500);                      // Wait for a second
  digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
  delay(300000);  

}
