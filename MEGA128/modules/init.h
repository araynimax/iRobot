/**
 * Created by ArayniMax
 */
 
#include <i2c.h>
#include <math.h>
#include "port_init.h"
#include "adc.h"
#include "wii_cam.h"
#include "wheelencoder.h"
#include "ultrasonic.h"
#include "rc5.h"
#include "wheels.h"
int get_StateMachineState();
void set_StateMachineState(int);
#include "esp_datatransfer.h"
#include "statemachine.h"

void initialize(){
   i2c_init();
   port_init();
   wii_cam_init();
   adc_init();
   wheelencoder_init();
   wheels_init();
   ultrasonic_init();
   esp_datatransfer_init();
   rc5_init();
   #asm("sei")
}

int get_StateMachineState(){
  return mainstatemachinestate;
}
void set_StateMachineState(int state){
  setMainStateMachineState(state);
}
