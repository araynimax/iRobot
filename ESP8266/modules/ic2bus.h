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

  uint8_t hindernis;
  //wii cam
  //unsigned int wii_cam_coord_X[4];
  //unsigned int wii_cam_coord_Y[4];
  //unsigned int wii_cam_size[4];
};
struct TData {
  struct TSensor sensor;
};

const int BUFFER_SIZE = sizeof(struct TData);

union TBuffer {
  unsigned char bytes[BUFFER_SIZE];
  struct TData data;
};


#pragma pack(pop)

union TBuffer rx_buffer;
union TBuffer tx_buffer;


unsigned char i2c_master_trans(unsigned char *tx_data,unsigned char tx_count,unsigned char *rx_data,unsigned char rx_count)
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
   //PREPARE HERE

  // transmit data from tx_buffer to the slave and receive response in rx_buffer
  i2c_result = i2c_master_trans(tx_buffer.bytes, BUFFER_SIZE, rx_buffer.bytes, BUFFER_SIZE);

  // display the transaction on the terminal
  if (i2c_result == 0)
  {
    // I2C transaction was performed without errors
    Serial.printf("I2C transaction result: %i\r\n", i2c_result);
    Serial.printf("Transmitted to MEGA128");
    Serial.printf("Received from MEGA128");
    Serial.printf("Content of Receive Buffer: ");
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
