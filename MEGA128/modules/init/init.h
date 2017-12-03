#include <i2c.h>
#include "port_init.h"
#include "interrupt_init.h"
#include "timer_init.h"
#include "adc_int.h"
#include "wii_cam_init.h"
#include "wheelencoder_init.h"
#include "ultrasonic_init.h"
#include "rc5_init.h";

void initialize(){
   i2c_init();
   port_init();
   timer_init();
   interrupt_init();
   wii_cam_init();
   adc_init();
   wheelencoder_init();
   ultrasonic_init();
}
