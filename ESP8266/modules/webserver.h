#include <ESP8266WebServer.h>

ESP8266WebServer server(80);

void webserver_init(){
  SPIFFS.begin();
  server.serveStatic("/", SPIFFS, "/index.html");
  server.serveStatic("/onsenui.css", SPIFFS, "/onsenui.css");  
  server.serveStatic("/onsen-css-components.min.css", SPIFFS, "/onsen-css-components.min.css");  
  server.serveStatic("/jquery.min.js", SPIFFS, "/jquery.min.js");  
  server.serveStatic("/onsenui.min.js", SPIFFS, "/onsenui.min.js");  
  server.serveStatic("/style.css", SPIFFS, "/style.css");  
  server.serveStatic("/script.js", SPIFFS, "/script.js");
  server.serveStatic("/socket.js", SPIFFS, "/socket.js");
  server.serveStatic("/font_awesome/css/font-awesome.min.css", SPIFFS, "/font-awesome.min.css");
  server.serveStatic("/material-design-iconic-font/css/material-design-iconic-font.min.css", SPIFFS, "/material-design-iconic-font.min.css");
  server.serveStatic("/awesomespinner.min.css", SPIFFS, "/awesomespinner.min.css");

  
  server.begin();
  Serial.println("HTTP server gestartet!");
}