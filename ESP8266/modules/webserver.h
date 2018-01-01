#include "website.h"
AsyncWebServer server(80);

void webserver_init(){
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", html);
  });
  server.begin();
  Serial.println("HTTP server gestartet!");
}