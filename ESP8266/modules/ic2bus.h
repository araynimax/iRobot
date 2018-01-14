#define TWI_SLAVE_ADDR 0x50
//#define TBUFFER_SIZE 64

#pragma pack(push, 1)

unsigned char i2c_result = 0;



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


const int BUFFER_SIZE = sizeof(struct TData);

union TBuffer {
  char bytes[BUFFER_SIZE];
  struct TData data;
};

int setState = -1;

#pragma pack(pop)

union TBuffer rx_buffer;
union TBuffer tx_buffer;


unsigned char i2c_master_trans(char *tx_data,char tx_count,char *rx_data,char rx_count)
{
  unsigned char i = 0;
  unsigned char i2c_error = 0;

  Wire.beginTransmission(TWI_SLAVE_ADDR);
  Wire.write(tx_data, tx_count);
  i2c_error = Wire.endTransmission();
  if (i2c_error != 0) return i2c_error;

  // Receive rx_buffer
  if (Wire.requestFrom(TWI_SLAVE_ADDR, rx_count) != rx_count)
  {
    i2c_error = 9;
    return i2c_error;
  }
  while (Wire.available())
  {
    rx_data[i] = Wire.read();
    i++;
  }
  return 0;
}

void i2c_exchange_data(void)
{
  char charBuf[16];

  // prepare the data in the tx_buffer
   tx_buffer.data.setState = setState;
   setState = -1;

  // transmit data from tx_buffer to the slave and receive response in rx_buffer
  i2c_result = i2c_master_trans(tx_buffer.bytes, BUFFER_SIZE, rx_buffer.bytes, BUFFER_SIZE);

  // display the transaction on the terminal
  if (i2c_result == 0)
  {
    // I2C transaction was performed without errors
    Serial.printf("I2C transaction result: %i\r\n", i2c_result);
    Serial.printf("Received from MEGA128\n\r");
    Serial.printf("Content of Receive Buffer: \n\r");
    for (int i = 0; i < BUFFER_SIZE; i++)
      Serial.printf("%2X ", rx_buffer.bytes[i]);
    Serial.printf("\r\n");
	Serial.printf("Lighsensor Right %i\n",rx_buffer.data.sensor.light_right);
	Serial.printf("Lighsensor Left %i\n",rx_buffer.data.sensor.light_left);
	Serial.printf("Bumper Right %i\n",rx_buffer.data.sensor.bumper_right);
	Serial.printf("Bumper Left %i\n",rx_buffer.data.sensor.bumper_left);

  }
  else
  {
    // I2C transaction was performed with errors
    Serial.printf("I2C transaction error: %i\r\n\r\n", i2c_result);
	 Serial.printf("Content of Receive Buffer: ");
    for (int i = 0; i < BUFFER_SIZE; i++)
      Serial.printf("%2X ", rx_buffer.bytes[i]);
    Serial.printf("\r\n");
  }
}



void ic2bus_init(){
  Wire.setClock(400000);
  Wire.begin();
  Serial.printf("BUFFER_SIZE: %i\n",BUFFER_SIZE);
}
