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


//
#define HandleCollisionState 0
//


int WheelTimerCounter = 0;

struct structMotor{
  int force;
  int encoder_changes;
};

struct structQueue{
  int right_force;
  int right_encoder_changes;
  int left_force;
  int left_encoder_changes;
};

struct structMotor MotorRight;
struct structMotor MotorLeft;

struct structQueue MainQueue[10];
int MainQueueLength     = -1;
struct structQueue ObstacleQueue[10];
int ObstacleQueueLength = -1;


void setMotorDistance(int mm,int force){
  if(HandleCollisionState){
    if(ObstacleQueueLength < 10){
      if(ObstacleQueueLength == -1)ObstacleQueueLength++;
      ObstacleQueue[ObstacleQueueLength].right_force = force;
      ObstacleQueue[ObstacleQueueLength].left_force  = force;
      ObstacleQueue[ObstacleQueueLength].right_encoder_changes =  ((mm < 0 ? mm * (-1) : mm)  / (float) ((float) wheel_circumference /  (float) wheel_encoder_amount));
      ObstacleQueue[ObstacleQueueLength++].left_encoder_changes  =  ((mm < 0 ? mm * (-1) : mm)  / (float) ((float) wheel_circumference /  (float) wheel_encoder_amount));
    }
  }
  else{
    if(MainQueueLength < 10){
      if(MainQueueLength == -1)MainQueueLength++;
      MainQueue[MainQueueLength].right_force = force;
      MainQueue[MainQueueLength].left_force  = force;
      MainQueue[MainQueueLength].right_encoder_changes =  ((mm < 0 ? mm * (-1) : mm)  / (float) ((float) wheel_circumference /  (float) wheel_encoder_amount));
      MainQueue[MainQueueLength++].left_encoder_changes  =  ((mm < 0 ? mm * (-1) : mm)  / (float) ((float) wheel_circumference /  (float) wheel_encoder_amount));
    }
  }

}

void rotateRobot(float degree, int force){
  if(HandleCollisionState){
    if(ObstacleQueueLength < 10){
      if(ObstacleQueueLength == -1)ObstacleQueueLength++;
      ObstacleQueue[ObstacleQueueLength].right_force =  degree < 0 ? force: force  * (-1);
      ObstacleQueue[ObstacleQueueLength].left_force  = degree < 0 ? force *  (-1) : force;
      ObstacleQueue[ObstacleQueueLength].right_encoder_changes = robot_circumference / 360.0 / ( (float) wheel_circumference /  (float) wheel_encoder_amount) *  (degree < 0 ? degree * (-1.0): degree) - 4;
      ObstacleQueue[ObstacleQueueLength].left_encoder_changes  =  robot_circumference / 360.0 / ( (float) wheel_circumference /  (float) wheel_encoder_amount) *  (degree < 0 ? degree * (-1.0): degree) - 4;
      ObstacleQueueLength++;
    }
  }
  else{
    if(MainQueueLength < 10){
      if(MainQueueLength == -1)MainQueueLength++;
      MainQueue[MainQueueLength].right_force = force;
      MainQueue[MainQueueLength].left_force  = force;
      MainQueue[MainQueueLength].right_encoder_changes =   robot_circumference / 360.0 / ( (float) wheel_circumference /  (float) wheel_encoder_amount) *  (degree < 0 ? degree * (-1.0): degree) - 4;
      MainQueue[MainQueueLength].left_encoder_changes  =  robot_circumference / 360.0 / ( (float) wheel_circumference /  (float) wheel_encoder_amount) *  (degree < 0 ? degree * (-1.0): degree) - 4;
      MainQueueLength++;
    }
  }
}

int getRightDistanceEnable(){
  if(MotorRight.encoder_changes != -1){
     if(MotorRight.encoder_changes > wheelEncoder.rightMove)
       return 1;
     else {
       MotorRight.force = 0;
       MotorRight.encoder_changes = -1;
     }
  }
  else
   return 1;
}

int getLeftDistanceEnable(){
  if(MotorLeft.encoder_changes != -1){
    if(MotorLeft.encoder_changes > wheelEncoder.leftMove)
      return 1;
    else {
      MotorLeft.force = 0;
      MotorLeft.encoder_changes = -1;
    }
  }
  else
   return 1;
}

void execNextQueue(){
  if(HandleCollisionState){
    if(ObstacleQueueLength > 0){
      int i;
      int leftForce = ObstacleQueue[0].left_force;
      int rightForce = ObstacleQueue[0].right_force;
      wheelEncoder.leftMove = 0;
      wheelEncoder.rightMove = 0;
      MotorRight.encoder_changes = ObstacleQueue[0].right_encoder_changes;
      MotorLeft.encoder_changes = ObstacleQueue[0].left_encoder_changes ;
      for(i=0;i<ObstacleQueueLength;i++){
        ObstacleQueue[i].right_force = ObstacleQueue[i+1].right_force;
        ObstacleQueue[i].left_force = ObstacleQueue[i+1].left_force;
        ObstacleQueue[i].right_encoder_changes = ObstacleQueue[i+1].right_encoder_changes;
        ObstacleQueue[i].left_encoder_changes = ObstacleQueue[i+1].left_encoder_changes;
      }
      MotorRight.force = rightForce;
      MotorLeft.force = leftForce;
      ObstacleQueueLength--;
    }
    else{
      //nextObstacleState
      ObstacleQueueLength = -1;
    }
  }
  else{
    if(MainQueueLength > 0){
      int i;
      int leftForce = MainQueue[0].left_force;
      int rightForce = MainQueue[0].right_force;
      wheelEncoder.leftMove = 0;
      wheelEncoder.rightMove = 0;
      MotorRight.encoder_changes = MainQueue[0].right_encoder_changes;
      MotorLeft.encoder_changes = MainQueue[0].left_encoder_changes ;
      for(i=0;i<MainQueueLength;i++){
        MainQueue[i].right_force = MainQueue[i+1].right_force;
        MainQueue[i].left_force = MainQueue[i+1].left_force;
        MainQueue[i].right_encoder_changes = MainQueue[i+1].right_encoder_changes;
        MainQueue[i].left_encoder_changes = MainQueue[i+1].left_encoder_changes;
      }
      MotorRight.force = rightForce;
      MotorLeft.force = leftForce;
      MainQueueLength--;
    }
    else{
      //nextObstacleState
      MainQueueLength = -1;
    }
  }
}


interrupt [TIM1_OVF] void timer1_ovf_isr(void)
{
  TCNT1H=0xFB00 >> 8;
  TCNT1L=0xFB00 & 0xff;

   WheelTimerCounter++;
   if(WheelTimerCounter > 255)
    WheelTimerCounter = 0;

   if(WheelTimerCounter >= 255 - (MotorLeft.force < 0 ? MotorLeft.force * (-1) : MotorLeft.force)){
     ENGINE_ENABLE_LEFT = getLeftDistanceEnable();
     ENGINE_BACKWARDS_LEFT = MotorLeft.force < 0 ? 1 : 0;
   }
   else
     ENGINE_ENABLE_LEFT = 0;

     if(WheelTimerCounter >= 255 - (MotorRight.force < 0 ? MotorRight.force * (-1) : MotorRight.force)){
       ENGINE_ENABLE_RIGHT = getRightDistanceEnable();
       ENGINE_BACKWARDS_RIGHT = MotorRight.force < 0 ? 1 : 0;
     }
     else
       ENGINE_ENABLE_RIGHT = 0;

  if(MotorRight.encoder_changes == -1 && MotorLeft.encoder_changes == -1 && (MainQueueLength > 0 || ObstacleQueueLength > 0)){
     execNextQueue();
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
}
