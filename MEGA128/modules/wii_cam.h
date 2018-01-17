/**
 * Created by ArayniMax
 */

#define WIICAM_SLAVEADRESS 0xB0
#define WIICAM_SLAVEREAD   0xB1

struct structWiiCam{
       unsigned int coord_X[4];
       unsigned int coord_Y[4];
       unsigned int size[4];
       unsigned int data[16];
} WiiCamData;

void wii_cam_write(char b1, char b2)
{
  	i2c_start();
        i2c_write(WIICAM_SLAVEADRESS); //I2C-Adresse der Kamera, hier 0xBO
        i2c_write(b1);
        i2c_write(b2);
        i2c_stop();
        delay_ms(10);
}

void wii_cam_init(void)
{
 //Kamera initialisieren
	wii_cam_write(0x30,0x01);        	//Camera on
	wii_cam_write(0x30,0x08);         	//set sensitivity Block 1 und 2
	wii_cam_write(0x06,0x90);          //sensitivity part1
	wii_cam_write(0x08,0xC0);          //sensitivity part2
	wii_cam_write(0x1A,0x40);
	wii_cam_write(0x33,0x33);  	//setting Mode : hier extended
	delay_ms(100);
}


void wii_cam_read(void) {
  int i = 0;
  unsigned int temp;
  i2c_start();
  i2c_write(WIICAM_SLAVEADRESS); //I2C-Adresse der Kamera
  i2c_write(0x36);
  i2c_stop();
  delay_ms(1);
  i2c_start();
  i2c_write(WIICAM_SLAVEREAD); //I2C-Adresse der Kamera Lesemodus 0xB1
  for (i = 0; i < 15; i++) {
    WiiCamData.data[i] = i2c_read(1);
  }
  WiiCamData.data[15] = i2c_read(0);
  i2c_stop();
  for (i = 0; i < 4; i++) {
    temp = (WiiCamData.data[3 + 3 * i] & 0x30) << 4;
    WiiCamData.coord_X[i] = WiiCamData.data[1 + 3 * i] + temp;
    temp = (WiiCamData.data[3 + 3 * i] & 0xC0) << 2;
    WiiCamData.coord_Y[i] = WiiCamData.data[2 + 3 * i] + temp;
    WiiCamData.size[i] = WiiCamData.data[3 + 3 * i] & 0x0F;
  }
}
