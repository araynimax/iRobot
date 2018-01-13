#include <ESP8266WebServer.h>

ESP8266WebServer server(80);

void webserver_init(){
  SPIFFS.begin();
  server.serveStatic("/", SPIFFS, "/index.html");
  server.serveStatic("/app.js" , SPIFFS, "/app.js" );
  server.serveStatic("/ionicons.woff" , SPIFFS, "/ionicons.woff" );
  server.serveStatic("/fontawesome-webfont.woff" , SPIFFS, "/fontawesome-webfont.woff");
  server.serveStatic("/fontawesome-webfont.woff2", SPIFFS, "/fontawesome-webfont.woff2" );
  server.serveStatic("/Material-Design-Iconic-Font.woff" , SPIFFS, "/Material-Design-Iconic-Font.woff" );
  server.serveStatic("/Material-Design-Iconic-Font.woff2"     , SPIFFS, "/Material-Design-Iconic-Font.woff2"     );
  server.begin();
  Serial.println("HTTP server gestartet!");
}
