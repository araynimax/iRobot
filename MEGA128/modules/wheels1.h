int move(float mm,unsigned int force);
int rotate(float mm,unsigned int force);
void clearQueue();
void stopMove();
void execLastQueue();
#include "obstaclehandler.h"
int WheelMovingQueueLength = -1;
int WheelObstacleQueueLength = -1;

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

#define wheel_start_value 50

struct structWheelMoving{
  int encoder_changes;
  int copy_encoder_changes;
  int backwards;
  int clockwise;
  int movement_type;
  int force;
  int copy_force;
} WheelMoving;

struct structWheelManual{
  int enabled;
  int force;
}WheelManual;


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
struct structQueueItem WheelObstacleQueue[25];

void execNextQueue(){
    int movement_type;
if((checkSensors() == 2 ? WheelObstacleQueueLength : WheelMovingQueueLength)  > 0){
  int i;
 delay_ms(1500);

  WheelMoving.encoder_changes = checkSensors() == 2 ? WheelObstacleQueue[0].encoder_changes : WheelMovingQueue[0].encoder_changes;
  WheelMoving.backwards       = checkSensors() == 2 ? WheelObstacleQueue[0].backwards : WheelMovingQueue[0].backwards;
  WheelMoving.clockwise       = checkSensors() == 2 ? WheelObstacleQueue[0].clockwise : WheelMovingQueue[0].clockwise;
  movement_type   =             checkSensors() == 2 ? WheelObstacleQueue[0].movement_type : WheelMovingQueue[0].movement_type;
  WheelMoving.force           = checkSensors() == 2 ? WheelObstacleQueue[0].force : WheelMovingQueue[0].force;

  for(i = 0; i < (checkSensors() == 2 ? WheelObstacleQueueLength : WheelMovingQueueLength); i++){
    if( checkSensors() == 2){
      WheelObstacleQueue[i].encoder_changes = WheelObstacleQueue[i + 1].encoder_changes;
      WheelObstacleQueue[i].movement_type = WheelObstacleQueue[i + 1].movement_type;
      WheelObstacleQueue[i].clockwise =  WheelObstacleQueue[i + 1].clockwise;
      WheelObstacleQueue[i].backwards =  WheelObstacleQueue[i + 1].backwards;
      WheelObstacleQueue[i].force =  WheelObstacleQueue[i + 1].force;
    }
    else{
      WheelMovingQueue[i].encoder_changes = WheelMovingQueue[i + 1].encoder_changes;
      WheelMovingQueue[i].movement_type = WheelMovingQueue[i + 1].movement_type;
      WheelMovingQueue[i].clockwise =  WheelMovingQueue[i + 1].clockwise;
      WheelMovingQueue[i].backwards =  WheelMovingQueue[i + 1].backwards;
      WheelMovingQueue[i].force =  WheelMovingQueue[i + 1].force;
    }
}

  wheelEncoder.left = 0;
  wheelEncoder.right = 0;
  Wheel_Compare.left  = wheel_start_value;
  Wheel_Compare.right = wheel_start_value;
  Wheel_Compare.leftcpy  = wheel_start_value;
  Wheel_Compare.rightcpy = wheel_start_value;
  WheelMoving.movement_type = movement_type;

  if(checkSensors() == 2)
    WheelObstacleQueueLength--;
  else
    WheelMovingQueueLength--;
}
else{
  nextObstacleState();
  WheelMovingQueueLength = -1;
  WheelObstacleQueueLength = -1;

}
}

int calculateBrakeDistance(int x){
  return 26.0 / (1.0 + 862.79 * pow(2.7182818284590452353602874713526625,-0.05 * x));
}

void addQueue(int encoder_changes,int movement_type,int value,int force){
  if(checkSensors() == 2){
    if(WheelObstacleQueueLength == -1)WheelObstacleQueueLength++;
    WheelObstacleQueue[WheelObstacleQueueLength].encoder_changes = encoder_changes;
    WheelObstacleQueue[WheelObstacleQueueLength].movement_type = movement_type;
    WheelObstacleQueue[WheelObstacleQueueLength].clockwise = value < 0 ? 0 : 1;
    WheelObstacleQueue[WheelObstacleQueueLength].backwards = value < 0 ? 1 : 0;
    WheelObstacleQueue[WheelObstacleQueueLength].force = force;
    WheelObstacleQueueLength++;
  }
  else{
    if(WheelMovingQueueLength == -1)WheelMovingQueueLength++;
    WheelMovingQueue[WheelMovingQueueLength].encoder_changes = encoder_changes;
    WheelMovingQueue[WheelMovingQueueLength].movement_type = movement_type;
    WheelMovingQueue[WheelMovingQueueLength].clockwise = value < 0 ? 0 : 1;
    WheelMovingQueue[WheelMovingQueueLength].backwards = value < 0 ? 1 : 0;
    WheelMovingQueue[WheelMovingQueueLength].force = force;
    WheelMovingQueueLength++;
  }

}

void clearQueue(){
  if(checkSensors() == 2)
    WheelObstacleQueueLength = 0;
  else
    WheelMovingQueueLength = 0;
}

void stopMove(){
   WheelMoving.encoder_changes = 0;
}



/**
 * @param mm distance to travel (negative is backwards)
 * @param force if its 1 the queue will be cleared to force this action
 */
//@todo brakedistance
int move(float mm,unsigned int force){
    int encoder_changes = ((mm < 0 ? mm * (-1) : mm)  / (float) ((float) wheel_circumference /  (float) wheel_encoder_amount)); //-calculateBrakeDistance(force);
    addQueue(encoder_changes,wheel_movement_type_move,mm,force);
    return  encoder_changes;
}

void execLastQueue(){
  move(1000,150);
  WheelMoving.encoder_changes = WheelMoving.copy_encoder_changes;
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
interrupt [TIM1_COMPA] void timer1_compa_isr(void)
{
  if(WheelMoving.movement_type == wheel_movement_type_move){
    ENGINE_ENABLE_RIGHT = WheelMoving.encoder_changes > wheelEncoder.right ? 1 : 0;
     ENGINE_ENABLE_LEFT = WheelMoving.encoder_changes > wheelEncoder.left ? 1 : 0;
     ENGINE_BACKWARDS_RIGHT = WheelMoving.backwards == 1  ? 1 : 0;
     ENGINE_BACKWARDS_LEFT  = WheelMoving.backwards == 1  ? 1 : 0;
     if(ENGINE_ENABLE_LEFT == 0 && ENGINE_ENABLE_RIGHT == 0)
       WheelMoving.movement_type = wheel_movement_type_stop;
  }
  else if(WheelMoving.movement_type == wheel_movement_type_rotate)
  {
    ENGINE_ENABLE_RIGHT = WheelMoving.encoder_changes > wheelEncoder.right ? 1 : 0;
     ENGINE_ENABLE_LEFT = WheelMoving.encoder_changes > wheelEncoder.left ? 1 : 0;
     ENGINE_BACKWARDS_RIGHT = WheelMoving.clockwise == 1 ? 1 : 0;
     ENGINE_BACKWARDS_LEFT  = WheelMoving.clockwise == 1 ? 0 : 1;
     if(ENGINE_ENABLE_LEFT == 0 && ENGINE_ENABLE_RIGHT == 0)
       WheelMoving.movement_type = wheel_movement_type_stop;
  }
  else{
     execNextQueue();
  }

  if(checkSensors() == 1){
    ENGINE_ENABLE_RIGHT = 0;
    ENGINE_ENABLE_LEFT = 0;
    WheelMoving.copy_encoder_changes = WheelMoving.encoder_changes;
    WheelMoving.encoder_changes = 0;
    WheelMoving.copy_force = WheelMoving.force;
    handleCollisionObject(WheelMovingQueueLength+ WheelMoving.encoder_changes);
    Wheel_Compare.hindernis = 1;
  }
  else if(checkSensors() == 2){
    handleCollisionObject(WheelMovingQueueLength+WheelMoving.encoder_changes);
  }
  else{
    Wheel_Compare.hindernis = 0;
  }

}



interrupt [TIM1_COMPB] void timer1_compb_isr(void)
{
  ENGINE_ENABLE_LEFT = 0;
}


interrupt [TIM1_COMPC] void timer1_compc_isr(void)
{
ENGINE_ENABLE_RIGHT = 0;
}


interrupt [TIM1_OVF] void timer1_ovf_isr(void)
{
  if(Wheel_Compare.left < WheelMoving.force){
     Wheel_Compare.left++;
	 Wheel_Compare.leftcpy++;
  }
  if(Wheel_Compare.right < WheelMoving.force){
     Wheel_Compare.right++;
	 Wheel_Compare.rightcpy++;
  }
  if(wheelEncoder.left >= 100||wheelEncoder.right >= 100) {
  wheelEncoder.leftMove = 0;
  wheelEncoder.rightMove = 0;
  }

  if (5+wheelEncoder.leftMove<wheelEncoder.rightMove){
   if (Wheel_Compare.left<255)
     Wheel_Compare.left++;

   else if (Wheel_Compare.right>100)
     Wheel_Compare.right--;
  }
  else if (wheelEncoder.leftMove>wheelEncoder.rightMove+5){
   if (Wheel_Compare.right<255)
     Wheel_Compare.right++;

   else if (Wheel_Compare.left>100)
     Wheel_Compare.left--;
  }

  OCR1BL = Wheel_Compare.left;
  OCR1CL = Wheel_Compare.right;
}


void wheels_init(){
  TCCR1A  = (0 << COM1A1) |(0 << COM1A0) |(0 << COM1B1) |(0 << COM1B0) |(0 << COM1C1) |(0 << COM1C0) |(0 << WGM11) |(1 << WGM10);
  TCCR1B  = (0 << ICNC1) |(0 << ICES1) |(0 << WGM13) |(0 << WGM12) |(1 << CS12) |(0 << CS11) |(0 << CS10);
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
  TIMSK  |= (1 << TICIE1) |(1 << OCIE1A) |(1 << OCIE1B) |(1 << TOIE1);
  ETIMSK |= (1 << OCIE1C);

  WheelMoving.movement_type = wheel_movement_type_stop;
  Wheel_Compare.left  = wheel_start_value;
  Wheel_Compare.right = wheel_start_value;
  Wheel_Compare.leftcpy  = wheel_start_value;
  Wheel_Compare.rightcpy = wheel_start_value;
  WheelMoving.copy_encoder_changes = 0;
  WheelMoving.copy_force = 0;

  WheelManual.enabled = 0;
  WheelManual.force = 150;
}
