#include <WiFiManager.h>
#include <strings_en.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#ifndef STASSID
#define STASSID "cadr"
#define STAPSK "socketswitch"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;
WiFiManager wm;
ESP8266WebServer server(80);

const int led = D1;
const int power_pin = D2;
const int reset_button = D7;

void handleRoot() {
  digitalWrite(led, HIGH);
  delay(200);
  digitalWrite(led, LOW);
  server.send(200, "text/plain", "Yay");
}

void setup(void) {
  pinMode(led, OUTPUT);
  pinMode(power_pin,OUTPUT);
  pinMode(reset_button,INPUT);
  Serial.begin(115200);
  digitalWrite(power_pin,HIGH);
  Serial.println(" ");
  if(digitalRead(reset_button) == HIGH){
    Serial.println("Config mode");
    wm.startConfigPortal("Zamok_ot_VC_snova_sdoh", "very_hard_password"); 
    
  }
  Serial.println("ESP MAC address:");
  Serial.print(WiFi.macAddress());
  bool res;
  // res = wm.autoConnect(); // auto generated AP name from chipid
  // res = wm.autoConnect("AutoConnectAP"); // anonymous ap
  res = wm.autoConnect("Zamok_ot_VC_snova_sdoh", "very_hard_password");  // password protected ap

  if (!res) {
    Serial.println("Failed to connect");
    // ESP.restart();
  } else {
    //if you get here you have connected to the WiFi
    Serial.println("connected...yeey :)");
    Serial.println("");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    if (MDNS.begin("esp8266")) { Serial.println("MDNS responder started"); }

    server.on("/gpio/2", handleRoot);

    server.begin();
    Serial.println("HTTP server started");
  }
}

void loop(void) {
  server.handleClient();
  MDNS.update();
}
