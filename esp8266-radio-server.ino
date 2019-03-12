#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();
ESP8266WebServer server(80);

const char* ssid = "Marphus";
const char* password = "";

const int receiver_pin = 4; //D2 
const int transmit_pin = 0; //D3 

String received_radio = ""; 

void connectWifi() {
  
    WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
    Serial.println("Connecting WIFI ");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, 1);

    connectWifi(); 

  mySwitch.enableReceive(receiver_pin);  // Receiver on interrupt 0 => that is pin #2
  mySwitch.enableTransmit(transmit_pin);

  pinMode(LED_BUILTIN, OUTPUT);

  server.on("/", transmitRadio);
  server.on("/read", receivedRadio);

  server.begin();
    Serial.println("HTTP server started");
  digitalWrite(LED_BUILTIN, 0);

}

void transmitRadio() {
  digitalWrite(LED_BUILTIN, 1);

  mySwitch.send(6916879, 24);
  
  server.send(200, "text/plain", "Sent radio at " + String(millis()));
  digitalWrite(LED_BUILTIN, 0);

}

void receivedRadio() {
    digitalWrite(LED_BUILTIN, 1);

  if (received_radio == "") {
    server.send(404, "text/plain", "No radio received");
  } else {
    server.send(200, "text/plain", String(received_radio) );
  }
   
  digitalWrite(LED_BUILTIN, 0);
}
void loop() {

  server.handleClient();

  if (mySwitch.available()) {
    received_radio = "Received " + String(mySwitch.getReceivedValue()) + " / " + String(mySwitch.getReceivedBitlength()) + "bit, Protocol: " + String(mySwitch.getReceivedProtocol());
    Serial.println(received_radio );
    mySwitch.resetAvailable();
  } 
  
}
