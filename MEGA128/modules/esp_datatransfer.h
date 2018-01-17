/**
 * Created by ArayniMax
 */

#include <twi.h>
#include <stdint.h>

// 7bit slave I2C address
#define TWI_SLAVE_ADDR 0x50

// flag that signals that the TWI slave reception was OK
bit esp_datatransfer_received_ok = false;


struct TSensor {
  //Wheelencoder value
  uint8_t  wheelencoder_left;
  uint8_t  wheelencoder_right;
  //Ultrasonic
  int16_t ultrasonic_angle;
  int16_t  ultrasonic_distance;
  //Distance sensor
  uint8_t  distance_right;
  uint8_t  distance_frontright;
  uint8_t  distance_frontleft;
  uint8_t  distance_left;
  //Line detector
  uint8_t  linedetector_right;
  uint8_t  linedetector_middleright;
  uint8_t  linedetector_middleleft;
  uint8_t  linedetector_left;
  //light sensor
  int16_t  light_right;
  int16_t  light_left;
  //rc5value
  uint8_t  rc5_value;
    //bumper
  uint8_t  bumper_left;
  uint8_t  bumper_right;

  //wii cam
  // uint8_t wii_cam_coord_X[4];
  // uint8_t wii_cam_coord_Y[4];
  // uint8_t wii_cam_size[4];
};

struct TControl{
  uint8_t isForward;
  uint8_t isRotate;
  uint8_t degree;
  uint8_t force;
  uint8_t stop;
};

struct TData {
  struct TSensor sensor;
  struct TControl manualControl;
  uint8_t currentState;
  int16_t setState;
  //uint8_t  ultrasonic_mapping[180];
};

const int TWI_BUFFER_SIZE = sizeof(struct TData);

union TBuffer {
  char bytes[TWI_BUFFER_SIZE];
  struct TData data;
};

union TBuffer rx_buffer; // slave receive buffer
union TBuffer tx_buffer; // slave transmission buffer

flash char * flash status_msg[8] = {
  "OK",
  "Buffer overflow",
  "Arbitration lost",
  "Bus error",
  "NACK received",
  "Bus timeout",
  "Fail",
  "Unknown error"
};

bool slave_rx_handler(bool rx_complete) {
  if (twi_result == TWI_RES_OK)
    esp_datatransfer_received_ok = true; // signal that data was received without errors
  else {
    // TWI receive error, display the twi_result value
    lcd_clear();
    lcd_putsf("Receive error:\n");
    lcd_putsf(status_msg[twi_result]);
    esp_datatransfer_received_ok = false; // signal that data was received with errors
    return false; // stop reception
  }

  if (rx_complete)
    return false; // no more bytes to receive

  // signal to the TWI master that the TWI slave is ready to accept more data
  // as long as there is space in the receive buffer
  return (twi_rx_index < TWI_BUFFER_SIZE);
}

unsigned char slave_tx_handler(bool tx_complete) {
  if (tx_complete == false) {
  //Wheelencoder
  tx_buffer.data.sensor.wheelencoder_left = wheelEncoder.left;
  tx_buffer.data.sensor.wheelencoder_right = wheelEncoder.right;
  //Ultrasonic
  tx_buffer.data.sensor.ultrasonic_angle = ultrasonic_servo.angle;
  tx_buffer.data.sensor.ultrasonic_distance = ultrasonic_data.distance;
  //Distance sensor
  tx_buffer.data.sensor.distance_right = DISTANCE_SENSOR_RIGHT;
  tx_buffer.data.sensor.distance_frontright = DISTANCE_SENSOR_FRONT_RIGHT;
  tx_buffer.data.sensor.distance_frontleft = DISTANCE_SENSOR_FRONT_LEFT;
  tx_buffer.data.sensor.distance_left = DISTANCE_SENSOR_LEFT;
  //Line detector
  tx_buffer.data.sensor.linedetector_right = LINE_DETECTOR_RIGHT;
  tx_buffer.data.sensor.linedetector_middleright = LINE_DETECTOR_MID_RIGHT;
  tx_buffer.data.sensor.linedetector_middleleft = LINE_DETECTOR_MID_LEFT;
  tx_buffer.data.sensor.linedetector_left = LINE_DETECTOR_LEFT;
  //light sensor
  tx_buffer.data.sensor.light_right = LIGHT_SENSOR_RIGHT;
  tx_buffer.data.sensor.light_left = LIGHT_SENSOR_LEFT;
  //rc5value
  tx_buffer.data.sensor.rc5_value = rc5_data;
    //bumper
  tx_buffer.data.sensor.bumper_left = BUMPER_LEFT;
  tx_buffer.data.sensor.bumper_right = BUMPER_RIGHT;

  tx_buffer.data.currentState = get_StateMachineState();
  //Wheel_Compare.hindernis;
  //wii cam
   // {
   //   int i;
   //   for(i=0;i<4;i++){
   //     tx_buffer.data.sensor.wii_cam_coord_X[i] = WiiCamData.coord_X[i];
   //     tx_buffer.data.sensor.wii_cam_coord_Y[i] = WiiCamData.coord_Y[i];
   //     tx_buffer.data.sensor.wii_cam_size[i] = WiiCamData.size[i];
   //   }
   // }

    // number of bytes to transmit from the TWI slave to the TWI master
    return TWI_BUFFER_SIZE;
  }

  // transmission from slave to master has already started,
  // no more bytes to send in this transaction
  if (esp_datatransfer_received_ok) {
    if(rx_buffer.data.setState == 1){
     set_StateMachineState(rx_buffer.data.currentState);
    }
  }
  return 0;
}

void esp_datatransfer_init() {
  twi_slave_init(false, TWI_SLAVE_ADDR, rx_buffer.bytes, TWI_BUFFER_SIZE, tx_buffer.bytes, slave_rx_handler, slave_tx_handler);
}
