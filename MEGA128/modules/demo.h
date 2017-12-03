/**
 *  States
 */

#define demostate_distance_sensor  0
#define demostate_engine  1
#define demostate_lightsensor  2
#define demostate_linedetector  3
#define demostate_wiicam  4
#define demostate_ultrasonic  5
#define demostate_rc5  6
#define demostate_stop 7

int state = demostate_stop;
int state_info = 0;

int engine_dir = 0;
int wiicamstate = 0;

void DEMOSTATE_MACHINE();

void demo(){
  lcd_puts(" Demo - Program");
  delay_ms(1000);
lcd_clear();
  while (1)
        {
         DEMOSTATE_MACHINE();

         if(!BUMPER_LEFT){
         delay_ms(250);
         lcd_clear();

         if(state+1 > demostate_stop) {
         state = 0;
         }
         else {
         state++;
         }

         state_info = 0;
        }
  }
}

void DEMOSTATE_RC5(){
  int rc5 = rc5_receive();
  char str[16];
   lcd_clear();
   if(rc5 != -1){
     itoa(rc5,str);
     lcd_puts(str);
     delay_ms(1000);
 }
 else
 {
   lcd_puts("NO SIGNAL");
 }
   delay_ms(50);
}

void DEMOSTATE_ULTRASONIC(){
   char str[16];
    lcd_clear();
    itoa(ultrasonic_data.distance,str);
    lcd_puts(" Distance in CM");
    lcd_gotoxy(0,1);
    lcd_puts(str);
    delay_ms(50);
}

void DEMOSTATE_DISTANCE_SENSOR(){
  lcd_clear();

  if(!DISTANCE_SENSOR_LEFT)
    lcd_puts("LEFT: 1");
  else
    lcd_puts("LEFT: 0");
  if(!DISTANCE_SENSOR_RIGHT)
    lcd_puts(" RIGHT: 1");
  else
    lcd_puts(" RIGHT: 0");

  lcd_gotoxy(0,1);

  if(!DISTANCE_SENSOR_FRONT_LEFT)
    lcd_puts("FLEFT: 1");
  else
    lcd_puts("FLEFT: 0");
  if(!DISTANCE_SENSOR_FRONT_RIGHT)
    lcd_puts("FRIGHT:1");
  else
    lcd_puts("FRIGHT:0");
}

void DEMOSTATE_ENGINE(){
char str[10];
     ENGINE_ENABLE_RIGHT = 1;
     ENGINE_ENABLE_LEFT = 1;
            lcd_clear();
     if(engine_dir == 0){
       lcd_puts("     ENCODER  >>");
       ENGINE_DIRECTION_LEFT = 0;
       ENGINE_DIRECTION_RIGHT = 0;
       }
     else{
       lcd_puts("<<   ENCODER    ");
       ENGINE_DIRECTION_LEFT = 1;
       ENGINE_DIRECTION_RIGHT = 1;
       }
      lcd_gotoxy(0,1);


       lcd_puts("L:");
        itoa(wheelEncoder.left,str);
        lcd_puts(str);
        lcd_puts(" R:");
        itoa(wheelEncoder.right,str);
        lcd_puts(str);

      if(!BUMPER_RIGHT){
      delay_ms(250);
      engine_dir = !engine_dir;
      }
}

void DEMOSTATE_LIGHTSENSOR(){
int right = LIGHT_SENSOR_RIGHT;
int left = LIGHT_SENSOR_LEFT;
char str[10];

lcd_clear();

        lcd_puts("LEFT:");
        itoa(left,str);
        lcd_puts(str);
        lcd_gotoxy(0,1);
        lcd_puts("RIGHT:");
        itoa(right,str);
        lcd_puts(str);
}

void DEMOSTATE_LINE_SENSOR(){
        lcd_clear();

  if(!LINE_DETECTOR_LEFT)
    lcd_puts("LEFT: 1");
  else
    lcd_puts("LEFT: 0");
  if(!LINE_DETECTOR_RIGHT)
    lcd_puts(" RIGHT: 1");
  else
    lcd_puts(" RIGHT: 0");

  lcd_gotoxy(0,1);

  if(!LINE_DETECTOR_MID_LEFT)
    lcd_puts("MLEFT: 1");
  else
    lcd_puts("MLEFT: 0");
  if(!LINE_DETECTOR_MID_RIGHT)
    lcd_puts("MRIGHT:1");
  else
    lcd_puts("MRIGHT:0");
}

void DEMOSTATE_WIICAM(){
     char str[10];
     lcd_clear();
     wii_cam_read();
     itoa(wiicamstate,str);
     lcd_puts(str);
     lcd_puts(":");
     itoa(WiiCamData.coord_X[wiicamstate],str);
     lcd_puts("  X:");
     lcd_puts(str);
     itoa(WiiCamData.coord_Y[wiicamstate],str);
     lcd_puts("Y:");
     lcd_puts(str);
     itoa(WiiCamData.size[wiicamstate],str);
     lcd_gotoxy(0,1);
     lcd_puts("Size:");
     lcd_puts(str);
     delay_ms(50);
      if(!BUMPER_RIGHT){
      delay_ms(250);
      wiicamstate = wiicamstate < 3 ? wiicamstate + 1 : 0;
      }
}


void DEMOSTATE_MACHINE(){

       ENGINE_ENABLE_RIGHT = 0;
       ENGINE_ENABLE_LEFT = 0;

      switch(state){

      case demostate_distance_sensor:
             if(state_info == 0){
               lcd_puts("DISTANZ SENSOR");
               delay_ms(1000);
               lcd_clear();
               state_info = 1;
               }
             DEMOSTATE_DISTANCE_SENSOR();
      break;
      case demostate_engine:
             if(state_info == 0){
               lcd_puts("MOTOREN");
               delay_ms(1000);
               lcd_clear();
               state_info = 1;
               }
             DEMOSTATE_ENGINE();
      break;
      case demostate_lightsensor:
             if(state_info == 0){
                 lcd_puts("LICHT SENSOR");
                 delay_ms(1000);
                 lcd_clear();
                 state_info = 1;
               }
             DEMOSTATE_LIGHTSENSOR();
      break;
      case demostate_linedetector:
             if(state_info == 0){
               lcd_puts("LINIEN SENSOR");
               delay_ms(1000);
               lcd_clear();
               state_info = 1;
               }
             DEMOSTATE_LINE_SENSOR();
      break;
      case demostate_wiicam:
               if(state_info == 0){
               lcd_puts("WII CAM");
               delay_ms(1000);
               lcd_clear();
               state_info = 1;
               }
             DEMOSTATE_WIICAM();
      break;
      case demostate_ultrasonic:
            if(state_info == 0){
            lcd_puts("Ultrasonic");
            delay_ms(1000);
            lcd_clear();
            state_info = 1;
            }
            DEMOSTATE_ULTRASONIC();
      break;
      case demostate_rc5:
            if(state_info == 0){
            lcd_puts("RC5");
            delay_ms(1000);
            lcd_clear();
            state_info = 1;
            }
            DEMOSTATE_RC5();
      break;
      default:
         lcd_clear();
          lcd_puts("STOP");
     }
      delay_ms(50);
}
