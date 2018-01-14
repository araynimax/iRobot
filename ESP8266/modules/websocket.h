WebSocketsServer webSocket = WebSocketsServer(81);

bool con = false; //@todo umnennen ist ein sehr allgemeiner name


void processingJSON(JsonObject& root){
  setState = root["setState"] ? root["setState"] : -1;
  Serial.printf("Set State: %d\n", setState);
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t lenght) {
  switch (type) {
    case WStype_DISCONNECTED:
      Serial.println("Websocket disconnected!");
      con = false;
      break;
    case WStype_CONNECTED:
      {
      IPAddress ip = webSocket.remoteIP(num);
      Serial.println("Websocket Connected!");
      Serial.println(ip);
      con = true;
      }
      break;
    case WStype_TEXT:
      {
        DynamicJsonBuffer jsonBuf;
        String text = String((char *) &payload[0]);
        JsonObject& root = jsonBuf  .parseObject(text);
        if (root.success()) {
          processingJSON(root);
        }
        else{
          Serial.println("Failed to parse JSON!");
        }
      }
      break;
  }
}

void sendDataObject()
{
  DynamicJsonBuffer jsonBuf;
  JsonObject& root = jsonBuf.createObject();

  root["currentState"] = rx_buffer.data.currentState;
  //Wheelencoder
  root["wheelencoder_left"] = rx_buffer.data.sensor.wheelencoder_left ;
  root["wheelencoder_right"] = rx_buffer.data.sensor.wheelencoder_right ;
  //Ultrasonic
 root["ultrasonic_angle"] =  rx_buffer.data.sensor.ultrasonic_angle ;
 root["ultrasonic_distance"] =  rx_buffer.data.sensor.ultrasonic_distance ;
  //Distance sensor
 root["distance_right"] =  rx_buffer.data.sensor.distance_right;
 root["distance_frontright"] =  rx_buffer.data.sensor.distance_frontright ;
  root["distance_frontleft"] = rx_buffer.data.sensor.distance_frontleft ;
  root["distance_left"] = rx_buffer.data.sensor.distance_left ;
  //Line detector
  root["linedetector_right"] = rx_buffer.data.sensor.linedetector_right ;
  root["linedetector_middleright"] = rx_buffer.data.sensor.linedetector_middleright ;
  root["linedetector_middleleft"] = rx_buffer.data.sensor.linedetector_middleleft ;
  root["linedetector_left"] = rx_buffer.data.sensor.linedetector_left ;
  //light sensor
  root["light_right"] = rx_buffer.data.sensor.light_right ;
  root["light_left"] = rx_buffer.data.sensor.light_left ;
  //rc5value
  root["rc5_value"] = rx_buffer.data.sensor.rc5_value;
  //bumper
  root["bumper_left"] = rx_buffer.data.sensor.bumper_left;
  root["bumper_right"] = rx_buffer.data.sensor.bumper_right;



  //wii cam
  //root["wii_cam_coord_X"] = rx_buffer.data.sensor.wii_cam_coord_X;
  //root["wii_cam_coord_Y"] = rx_buffer.data.sensor.wii_cam_coord_Y;
  //root["wii_cam_size"] = rx_buffer.data.sensor.wii_cam_size;

  char buf[512];
  size_t size = root.printTo(buf, sizeof(buf));
  webSocket.sendTXT(0, buf, size);
  //Serial.println(buf);

}

void websocket_init(){
	  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  Serial.println("WebSocket gestartet!");
}
