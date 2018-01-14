#define HandleCollisionState_noObstacle 0
#define HandleCollisionState_Obstacle 1
#define HandleCollisionState_calculating 2
#define HandleCollisionState_movingRight 3
#define HandleCollisionState_movingLeft 4
#define HandleCollisionState_movingForward 5
#define HandleCollisionState_movingBackward 6


unsigned int HandleCollisionState = HandleCollisionState_noObstacle;
unsigned int HandleCollisionStateChangeable = 0;
unsigned int NextHandleCollisionState = 0;
unsigned int LastHandleCollisionState = 0;
unsigned int AvoidObstacleSideMoveCount = 0;

struct structMotorStates{
  struct structMotor right;
  struct structMotor left;
  int encoderLeft;
  int encoderRight;
} MotorStates;

void HandleCollisionStateMachine();
void FN_HandleCollisionState_Obstacle();
void FN_HandleCollisionState_calculating();
void FN_HandleCollisionState_movingRight();
void FN_HandleCollisionState_movingLeft();
void FN_HandleCollisionState_movingForward();

void setHandleCollisionState(int);

void getMotorStates(){
  MotorStates.right = *getMotorStatesRight();
  MotorStates.left =  *getMotorStatesLeft();
  MotorStates.encoderLeft = wheelEncoder.leftMove;
  MotorStates.encoderRight = wheelEncoder.rightMove;
}

void restoreMotorStates(){
  *getMotorStatesRight() = MotorStates.right;
  *getMotorStatesRight() = MotorStates.left;
   wheelEncoder.leftMove = MotorStates.encoderLeft;
   wheelEncoder.rightMove = MotorStates.encoderRight;
}

int checkSensors(){
  if(HandleCollisionState != HandleCollisionState_noObstacle)
    return 2;
  else if(!BUMPER_LEFT || !BUMPER_RIGHT || !DISTANCE_SENSOR_FRONT_LEFT || !DISTANCE_SENSOR_FRONT_RIGHT)
    return 1;
  else
    return 0;
}

int isInMotion(){
  return MotorStates.right.finished == 0 || MotorStates.left.finished == 0;
}

void avoidCollisions(){
  if(checkSensors() == 1){
     int* queueLength = getQueueLength(0);
     if(*queueLength > 0 && isInMotion()){
       getMotorStates();
       stopRobot();
       setHandleCollisionState(HandleCollisionState_Obstacle);
       AvoidObstacleSideMoveCount = 0;
     }
  }else if(checkSensors() == 2){
    HandleCollisionStateMachine();
  }
}

void HandleCollisionStateMachine(){
  if(!HandleCollisionStateChangeable){
  switch(HandleCollisionState){

    case HandleCollisionState_Obstacle:
      FN_HandleCollisionState_Obstacle();
    break;

    case HandleCollisionState_movingRight:
      FN_HandleCollisionState_movingRight();
    break;

    case HandleCollisionState_movingLeft:
      FN_HandleCollisionState_movingLeft();
    break;

    case HandleCollisionState_movingForward:
      FN_HandleCollisionState_movingForward();
    break;


    default:
      HandleCollisionState = HandleCollisionState_noObstacle;

  }
}
}


void setHandleCollisionState(int state){
  LastHandleCollisionState = HandleCollisionState;
  HandleCollisionState = state;
  if(state == HandleCollisionState_noObstacle)
    restoreMotorStates();
}

void setNextHandleCollisionState(int state){
  HandleCollisionStateChangeable = 1;
  NextHandleCollisionState = state;
}


int checkFront(){
  return (!BUMPER_LEFT || !BUMPER_RIGHT || !DISTANCE_SENSOR_FRONT_LEFT || !DISTANCE_SENSOR_FRONT_RIGHT);
}

void nextObstacleState(){
  if(HandleCollisionStateChangeable)
    setHandleCollisionState(NextHandleCollisionState);
  HandleCollisionStateChangeable = 0;
}

void FN_HandleCollisionState_Obstacle(){
if(BUMPER_LEFT && BUMPER_RIGHT){
    if(!DISTANCE_SENSOR_FRONT_LEFT && DISTANCE_SENSOR_FRONT_RIGHT){
      moveRobot(-20,150);
      rotateRobot(90,150);
      setNextHandleCollisionState(HandleCollisionState_movingRight);
    }
    else if(DISTANCE_SENSOR_FRONT_LEFT && !DISTANCE_SENSOR_FRONT_RIGHT){
      moveRobot(-20,150);
      rotateRobot(-90,150);
      setNextHandleCollisionState(HandleCollisionState_movingLeft);
    }
    else if(!DISTANCE_SENSOR_FRONT_LEFT && !DISTANCE_SENSOR_FRONT_RIGHT){
      moveRobot(-20,150);
      rotateRobot(90,150);
      setNextHandleCollisionState(HandleCollisionState_movingRight);
    }
  }
  else if(!BUMPER_LEFT && BUMPER_RIGHT){
    moveRobot(-40,150);
    rotateRobot(90,150);
    setNextHandleCollisionState(HandleCollisionState_movingRight);
  }
  else if(BUMPER_LEFT && !BUMPER_RIGHT){
    moveRobot(-40,150);
    rotateRobot(-90,150);
    setNextHandleCollisionState(HandleCollisionState_movingLeft);
  }
  else if(!BUMPER_LEFT && !BUMPER_RIGHT){
    moveRobot(-40,150);
    rotateRobot(90,150);
    setNextHandleCollisionState(HandleCollisionState_movingRight);
  }

}

void FN_HandleCollisionState_movingRight(){
if(!DISTANCE_SENSOR_LEFT){
   if(checkFront()){
      clearQueue();
    }
   else{
     AvoidObstacleSideMoveCount++;
     moveRobot(100,150);
     setNextHandleCollisionState(HandleCollisionState_movingRight);
   }
}
else{
    stopRobot();
    rotateRobot(-90,150);
    moveRobot(200,150);
    setNextHandleCollisionState(HandleCollisionState_movingForward);
}

}

void FN_HandleCollisionState_movingLeft(){
  if(!DISTANCE_SENSOR_RIGHT){
     if(checkFront()){
        clearQueue();
      }
     else{
       AvoidObstacleSideMoveCount++;
       moveRobot(100,150);
       setNextHandleCollisionState(HandleCollisionState_movingLeft);
     }
  }
  else{
     stopRobot();
     rotateRobot(90,150);
     moveRobot(200,150);
     setNextHandleCollisionState(HandleCollisionState_movingForward);
  }
}

void FN_HandleCollisionState_movingForward(){
    if(LastHandleCollisionState == HandleCollisionState_movingLeft ? !DISTANCE_SENSOR_RIGHT : !DISTANCE_SENSOR_LEFT){
       if(checkFront()){
          clearQueue();
        }
       else{
         moveRobot(100,150);
         MotorStates.encoderLeft +=calculateEncoderChanges(100,0);
         MotorStates.encoderRight+=calculateEncoderChanges(100,0);
         setNextHandleCollisionState(HandleCollisionState_movingForward);
       }
    }
    else{
    int i;
      rotateRobot(LastHandleCollisionState == HandleCollisionState_movingLeft ? 90 : -90,150);
      for(i = 0;i< AvoidObstacleSideMoveCount; i++)
       moveRobot(100,150);
      rotateRobot(LastHandleCollisionState == HandleCollisionState_movingLeft ? -90 : 90,150);
     setNextHandleCollisionState(HandleCollisionState_noObstacle);
    }
}
