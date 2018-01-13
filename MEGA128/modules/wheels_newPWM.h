//#include "obstaclehandler.h"

// All defined datas are represented in millimetres!

// wheel
#define wheel_diameter 100
#define wheel_rubber   0.5

// wheel encoder paper
#define wheel_encoder_amount_white 30
#define wheel_encoder_amount_black 30

//robot
#define robot_diameter 134  //Without wheels
//#define robot_diameter 143  //With half wheels
//#define robot_diameter 150  //With  wheels

// Calculations
#define wheel_circumference (PI * (wheel_diameter + wheel_rubber))
#define robot_circumference (PI * robot_diameter)
#define wheel_encoder_amount (wheel_encoder_amount_white + wheel_encoder_amount_black)

#define wheel_movement_type_stop 1
#define wheel_movement_type_move 2
#define wheel_movement_type_rotate 3

#define wheel_timer_counter_ms 8.16

#define wheel_start_value 130

struct structWheelMoving{
  int encoder_changes;
  int backwards;
  int clockwise;
  int movement_type;
  int force;
} WheelMoving;



struct structCompares{
  int left;
  int right;
  int leftcpy;
  int rightcpy;
  int hindernis;
} Wheel_Compare;

char debugstr[10];
#define debug(integer) itoa(integer,debugstr); lcd_puts(debugstr);
#define debugFloat(floatValue) ftoa(floatValue,4,debugstr); lcd_puts(debugstr);

struct structQueueItem{
  int encoder_changes;
  int backwards;
  int movement_type;
  int force;
  int clockwise;
};

struct structQueueItem WheelMovingQueue[25];

int WheelMovingQueueLength = 0;

void execNextQueue(){
  int movement_type;
if(WheelMovingQueueLength > 0){
  int i;
  //lcd_clear();

  delay_ms(5000);
  WheelMoving.encoder_changes = WheelMovingQueue[0].encoder_changes;
  WheelMoving.backwards       = WheelMovingQueue[0].backwards;
  WheelMoving.clockwise       = WheelMovingQueue[0].clockwise;
  movement_type   =             WheelMovingQueue[0].movement_type;
  WheelMoving.force           = WheelMovingQueue[0].force;

  for(i = 0; i < WheelMovingQueueLength; i++){
    WheelMovingQueue[i].encoder_changes = WheelMovingQueue[i + 1].encoder_changes;
    WheelMovingQueue[i].movement_type = WheelMovingQueue[i + 1].movement_type;
    WheelMovingQueue[i].clockwise = WheelMovingQueue[i + 1].clockwise;
    WheelMovingQueue[i].backwards = WheelMovingQueue[i + 1].backwards;
    WheelMovingQueue[i].force = WheelMovingQueue[i + 1].force;
}

  wheelEncoder.left = 0;
  wheelEncoder.right = 0;
  Wheel_Compare.left  = wheel_start_value;
  Wheel_Compare.right = wheel_start_value;
  Wheel_Compare.leftcpy  = wheel_start_value;
  Wheel_Compare.rightcpy = wheel_start_value;
  WheelMoving.movement_type = movement_type;
  WheelMovingQueueLength--;
}
}

int calculateBrakeDistance(int x){
  return 26.0 / (1.0 + 862.79 * pow(2.7182818284590452353602874713526625,-0.05 * x));
}

void addQueue(int encoder_changes,int movement_type,int value,int force){
  WheelMovingQueue[WheelMovingQueueLength].encoder_changes = encoder_changes;
  WheelMovingQueue[WheelMovingQueueLength].movement_type = movement_type;
  WheelMovingQueue[WheelMovingQueueLength].clockwise = value < 0 ? 0 : 1;
  WheelMovingQueue[WheelMovingQueueLength].backwards = value < 0 ? 1 : 0;
  WheelMovingQueue[WheelMovingQueueLength].force = force;
  WheelMovingQueueLength++;
}

void clearQueue(){
  WheelMovingQueueLength = 0;
}







/*
void calculateEngineSpeed(){
  WheelSpeed.timer_counter++;
  if(WheelSpeed.encoderLeft + 10 < wheelEncoder.left || WheelSpeed.encoderRight + 10 < wheelEncoder.right ){
    WheelSpeed.speed = wheel_circumference / (float) wheel_encoder_amount / (wheel_timer_counter_ms * WheelSpeed.timer_counter) / 10;

    lcd_clear();
    debugFloat(WheelSpeed.speed);
    lcd_puts(" ");
    debug(WheelSpeed.timer_counter);
    lcd_gotoxy(0,1);
    debug(WheelSpeed.encoderRight);
    lcd_puts(":");
    debug(WheelSpeed.encoderLeft);
    WheelSpeed.encoderLeft =  wheelEncoder.left;
    WheelSpeed.encoderRight =  wheelEncoder.right;
    WheelSpeed.timer_counter = 0;
  }
}
*/

/**
 * @param mm distance to travel (negative is backwards)
 * @param force if its 1 the queue will be cleared to force this action
 */
//@todo geschwindigkeit
int move(float mm,unsigned int force){
    int encoder_changes = ((mm < 0 ? mm * (-1) : mm)  / (float) ((float) wheel_circumference /  (float) wheel_encoder_amount)) - calculateBrakeDistance(force);
    addQueue(encoder_changes,wheel_movement_type_move,mm,force);
    return  encoder_changes;
}
/**
 * @param degree
 * @param clockwise [optional] change the rotation direction
 * @param force if its 1 the queue will be cleared to force this action
 */
//@todo geschwindigkeit
int rotate(float degree,unsigned int force){
  int encoder_changes =  robot_circumference / 360.0 / ( (float) wheel_circumference /  (float) wheel_encoder_amount) *  (degree < 0 ? degree * (-1.0): degree) - 4;  // -calculateBrakeDistance(force);
  addQueue(encoder_changes,wheel_movement_type_rotate,degree,force);
  return  encoder_changes;
}


//@todo bremsen
int ipwmcounter = 0;
int ipwmcompareleft = 12;
int ipwmcompareright = 12;

interrupt [TIM1_OVF] void timer1_ovf_isr(void)
{
  // Reinitialize Timer 1 value
  TCNT1L=0x00;
  ipwmcounter++;
  if (ipwmcounter>255)
    ipwmcounter=0;
 if(ipwmcounter >= ipwmcompareleft){
 ENGINE_ENABLE_LEFT=0;
  }else{
  ENGINE_ENABLE_LEFT=1;
  }
 if(ipwmcounter >= ipwmcompareright){
 ENGINE_ENABLE_RIGHT=0;
  }else{
  ENGINE_ENABLE_RIGHT=1;}
}


void wheels_init(){
  TCCR1A  = (0 << COM1A1) |(0 << COM1A0) |(0 << COM1B1) |(0 << COM1B0) |(0 << COM1C1) |(0 << COM1C0) |(0 << WGM11) |(1 << WGM10);
  TCCR1B  = (0 << ICNC1) |(0 << ICES1) |(0 << WGM13) |(0 << WGM12) |(0 << CS12) |(1 << CS11) |(0 << CS10);
  TCNT1H  = 0x00;
  TCNT1L  = 0x00;
  ICR1H   = 0x00;
  ICR1L   = 0x00;
  OCR1AH  = 0x00;
  OCR1AL  = 0x00;
  OCR1BH  = 0x00;
  OCR1BL  = 0x00;
  OCR1CH  = 0x00;
  OCR1CL  = 0x00;
  TIMSK  |= (0 << TICIE1) |(0 << OCIE1A) |(0 << OCIE1B) |(1 << TOIE1);
  ETIMSK |= (0 << OCIE1C);

  WheelMoving.movement_type = wheel_movement_type_stop;
  Wheel_Compare.left  = wheel_start_value;
  Wheel_Compare.right = wheel_start_value;
  Wheel_Compare.leftcpy  = wheel_start_value;
  Wheel_Compare.rightcpy = wheel_start_value;
}
