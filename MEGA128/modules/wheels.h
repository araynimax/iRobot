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




void moveRobot(int mm,int force);
void rotateRobot(float degree, int force);
void clearQueue();
void stopRobot();
int* getQueueLength(int ForObstacleQueue);
int calculateEncoderChanges(int value,int isRotation);

struct structMotor{
  int force;
  int encoder_changes;
  int finished;
};

struct structQueue{
  int queueIsEmpty;
  int right_force;
  int right_encoder_changes;
  int left_force;
  int left_encoder_changes;
};

struct structMotor* getMotorStatesLeft();
struct structMotor* getMotorStatesRight();

#include "obstaclehandler.h"


int WheelTimerCounter = 0;

struct structMotor MotorRight;
struct structMotor MotorLeft;

#define MAX_QUEUE_LENGTH 20

struct structQueue MainQueue[MAX_QUEUE_LENGTH];
int MainQueueLength = -1;
struct structQueue ObstacleQueue[MAX_QUEUE_LENGTH];
int ObstacleQueueLength = -1;

int queueModifing = 0;

struct structMotor* getMotorStatesLeft(){
  return &MotorLeft;
}

struct structMotor* getMotorStatesRight(){
  return &MotorRight;
}

int* getQueueLength(int ForObstacleQueue){
  return ForObstacleQueue ? &ObstacleQueueLength : &MainQueueLength;
}

struct structQueue* getQueue(int ForObstacleQueue){
  return ForObstacleQueue ? ObstacleQueue : MainQueue;
}

int calculateEncoderChanges(int value,int isRotation){
  return isRotation ? (robot_circumference / 360.0 / ( (float) wheel_circumference /  (float) wheel_encoder_amount) *  (value < 0 ? value * (-1.0): value) - 4) : ((value < 0 ? value * (-1) : value)  / (float) ((float) wheel_circumference /  (float) wheel_encoder_amount));
}

void addToQueue(int force,int value,int isRotation,int isForObstacleQueue){
  if(*getQueueLength(isForObstacleQueue) < MAX_QUEUE_LENGTH){
    int encoderChange = calculateEncoderChanges(value,isRotation);
    int* queueLength = getQueueLength(isForObstacleQueue);
    struct structQueue* queue = getQueue(isForObstacleQueue);
    queueModifing = 1;
    if(*queueLength == -1)(*queueLength)++;
    queue[*queueLength].right_force = isRotation ? (value < 0 ? force: force  * (-1))  : force;
    queue[*queueLength].left_force  = isRotation ? (value < 0 ? force *  (-1) : force) : force;
    queue[*queueLength].right_encoder_changes = encoderChange;
    queue[*queueLength].left_encoder_changes  = encoderChange;
    (*queueLength)++;
  }
  queueModifing = 0;
}

void clearQueue(){
  int* queueLength = getQueueLength(checkSensors() != 0);
  *queueLength = 0;
}

void moveRobot(int mm,int force){
  addToQueue(force,mm,0,checkSensors() != 0);
}

void rotateRobot(float degree, int force){
  addToQueue(force,degree,1,checkSensors() != 0);
}

void stopRobot(){
  MotorRight.encoder_changes = 0;
  MotorLeft.encoder_changes = 0;
}

struct structQueue getNextQueueObject(int shift,int isForObstacleQueue){
  int* queueLength = getQueueLength(isForObstacleQueue);
  struct structQueue queueItem;                                             
  queueItem.queueIsEmpty = 1;
  if(*queueLength > 0){
     struct structQueue* queue = getQueue(isForObstacleQueue);
     queueItem = queue[0];
     if(shift){
       int i;
       for(i=0;i<*queueLength;i++)
         queue[i] = queue[i+1];
       (*queueLength)--;
     }
  }
  else if(*queueLength == 0){
    *queueLength = -1;
  }
  return queueItem;
}

void execNextQueue(){
  if(MotorRight.finished == 1 && MotorLeft.finished == 1 && queueModifing == 0){
    struct structQueue queueItem;
    queueItem = getNextQueueObject(1, HandleCollisionState != HandleCollisionState_noObstacle);                                              
    if(queueItem.queueIsEmpty != 1){    
      MotorRight.encoder_changes = queueItem.right_encoder_changes;
      MotorLeft.encoder_changes = queueItem.left_encoder_changes;
      wheelEncoder.leftMove  = 0;
      wheelEncoder.rightMove = 0;
      MotorRight.force = queueItem.right_force;
      MotorLeft.force = queueItem.left_force;
      MotorRight.finished = 0;
      MotorLeft.finished = 0;
    }      
    
    else if(queueItem.queueIsEmpty == 1 && HandleCollisionState != HandleCollisionState_noObstacle){
    nextObstacleState();
    }
  }
}


int getRightMotorEnableState(){
  if(MotorRight.finished == 0){
     if(MotorRight.encoder_changes > wheelEncoder.rightMove)
       return 1;
     else {
       MotorRight.force = 0;
       MotorRight.finished  = 1;  
     }
  }
  else
   return 1;
}

int getLeftMotorEnableState(){
  if(MotorLeft.finished == 0){
    if(MotorLeft.encoder_changes > wheelEncoder.leftMove)
      return 1;
    else {
      MotorLeft.force = 0;
      MotorLeft.finished = 1;
    }
  }
  else
   return 1;
}

interrupt [TIM1_OVF] void timer1_ovf_isr(void)
{
  TCNT1H=0xFB00 >> 8;
  TCNT1L=0xFB00 & 0xff;

   WheelTimerCounter++;
   if(WheelTimerCounter > 255)
    WheelTimerCounter = 0;

   if(WheelTimerCounter >= 255 - (MotorLeft.force < 0 ? MotorLeft.force * (-1) : MotorLeft.force)){
     ENGINE_ENABLE_LEFT = getLeftMotorEnableState();
     ENGINE_BACKWARDS_LEFT = MotorLeft.force < 0 ? 1 : 0;
   }
   else
     ENGINE_ENABLE_LEFT = 0;

     if(WheelTimerCounter >= 255 - (MotorRight.force < 0 ? MotorRight.force * (-1) : MotorRight.force)){
       ENGINE_ENABLE_RIGHT = getRightMotorEnableState();
       ENGINE_BACKWARDS_RIGHT = MotorRight.force < 0 ? 1 : 0;
     }
     else
       ENGINE_ENABLE_RIGHT = 0;

      if(WheelTimerCounter == 0){
      execNextQueue();
      avoidCollisions();
      }

}

void wheels_init(){
  TCCR1A  = (0 << COM1A1) |(0 << COM1A0) |(0 << COM1B1) |(0 << COM1B0) |(0 << COM1C1) |(0 << COM1C0) |(0 << WGM11) |(0 << WGM10);
  TCCR1B  = (0 << ICNC1) |(0 << ICES1) |(0 << WGM13) |(0 << WGM12) |(0 << CS12) |(0 << CS11) |(1 << CS10);
  TCNT1H  = 0xFB00 >> 8;
  TCNT1L  = 0xFB00 & 0xff;
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

  MotorRight.force = 0;
  MotorLeft.force = 0;
  MotorRight.encoder_changes = -1;
  MotorLeft.encoder_changes = -1;  
  wheelEncoder.rightMove = 1;
  wheelEncoder.leftMove = 1;
  MotorRight.finished = 1;
  MotorLeft.finished = 1;
}
