#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>
#include <FS.h>
#include <Wire.h>

#include "modules/ic2bus.h"
#include "modules/websocket.h"
#include "modules/webserver.h"



//@todo ins richtige file packen
unsigned char i = 0;
int16_t iRGB = 0;

void setup() {
  // put your setup code here, to run once:

 Serial.begin(115200);
  Serial.println();
  delay(200);

ic2bus_init();

  IPAddress local_IP(192, 168, 2, 100);
  IPAddress gateway(192, 168, 2, 1);
  IPAddress subnet(255, 255, 255, 0);

  Serial.print("Setting soft-AP configuration ... ");
  Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");

  Serial.print("Setting soft-AP ... ");
  Serial.println(WiFi.softAP("iRobot-WLAN","123456789") ? "Ready" : "Failed!");

  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());

  Serial.print("Station IP address = ");
  Serial.println(WiFi.localIP());

websocket_init();
  webserver_init();

}




void loop() {
  unsigned long currentMillis = millis();
  webSocket.loop();
  //avoid overloading the serial pipe which causes hanging up the terminal and watchdog
  if (currentMillis % 1000 == 0) {
    if (con == true) {
      i2c_exchange_data();
      sendDataObject(); //Daten über Websocket nur schicken, wenn Verbindung besteht!
    }
  }
  server.handleClient();
}
