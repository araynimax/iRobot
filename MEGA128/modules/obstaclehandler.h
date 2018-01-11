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


void FN_HandleCollisionState_noObstacle(int);
void FN_HandleCollisionState_Obstacle();
void FN_HandleCollisionState_calculating();
void FN_HandleCollisionState_movingRight();
void FN_HandleCollisionState_movingLeft();
void FN_HandleCollisionState_movingForward();

int checkForCollision();

void setHandleCollisionState(int);

void handleCollisionObject(int length){
  if(!HandleCollisionStateChangeable){
  switch(HandleCollisionState){

    case HandleCollisionState_noObstacle:
      FN_HandleCollisionState_noObstacle(length);
    break;

    case HandleCollisionState_Obstacle:
      FN_HandleCollisionState_Obstacle();
    break;
    //
    // case HandleCollisionState_calculating:
    //   FN_HandleCollisionState_calculating();
    // break;

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
    execLastQueue();
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

int checkSensors(){
  if(HandleCollisionState != HandleCollisionState_noObstacle)
    return 2;
  else if(!BUMPER_LEFT || !BUMPER_RIGHT || !DISTANCE_SENSOR_FRONT_LEFT || !DISTANCE_SENSOR_FRONT_RIGHT)
    return 1;
  else
    return 0;
}

void FN_HandleCollisionState_noObstacle(int length){
  if(length > -1)
     HandleCollisionState = HandleCollisionState_Obstacle;
  AvoidObstacleSideMoveCount = 0;
}

void FN_HandleCollisionState_Obstacle(){
if(BUMPER_LEFT && BUMPER_RIGHT){
    if(!DISTANCE_SENSOR_FRONT_LEFT && DISTANCE_SENSOR_FRONT_RIGHT){
      move(-20,150);
      rotate(90,150);
      setNextHandleCollisionState(HandleCollisionState_movingRight);
    }
    else if(DISTANCE_SENSOR_FRONT_LEFT && !DISTANCE_SENSOR_FRONT_RIGHT){
      move(-20,150);
      rotate(-90,150);
      setNextHandleCollisionState(HandleCollisionState_movingLeft);
    }
    else if(!DISTANCE_SENSOR_FRONT_LEFT && !DISTANCE_SENSOR_FRONT_RIGHT){
      move(-20,150);
      rotate(90,150);
      setNextHandleCollisionState(HandleCollisionState_movingRight);
    }
  }
  else if(!BUMPER_LEFT && BUMPER_RIGHT){
    move(-40,150);
    rotate(90,150);
    setNextHandleCollisionState(HandleCollisionState_movingRight);
  }
  else if(BUMPER_LEFT && !BUMPER_RIGHT){
    move(-40,150);
    rotate(-90,150);
    setNextHandleCollisionState(HandleCollisionState_movingLeft);
  }
  else if(!BUMPER_LEFT && !BUMPER_RIGHT){
    move(-40,150);
    rotate(90,150);
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
     move(100,150);
     setNextHandleCollisionState(HandleCollisionState_movingRight);
   }
}
else{
    stopMove();
    rotate(-90,150);
    move(200,150);
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
       move(100,150);
       setNextHandleCollisionState(HandleCollisionState_movingLeft);
     }
  }
  else{
     stopMove();
     rotate(90,150);
     move(200,150);
     setNextHandleCollisionState(HandleCollisionState_movingForward);
  }
}

void FN_HandleCollisionState_movingForward(){
  if(LastHandleCollisionState == HandleCollisionState_movingRight){
    if(!DISTANCE_SENSOR_LEFT){
       if(checkFront()){
          clearQueue();
        }
       else{
         move(100,150);
       }
    }
    else{
      int i;
       rotate(-90,150);
       for(i = 0;i< AvoidObstacleSideMoveCount; i++)
        move(100,150);
      rotate(90,150);
      setNextHandleCollisionState(HandleCollisionState_noObstacle);
    }
  }
  else if(LastHandleCollisionState == HandleCollisionState_movingLeft){
    if(!DISTANCE_SENSOR_RIGHT){
       if(checkFront()){
          clearQueue();
        }
       else{
         move(100,150);
       }
    }
    else{
    int i;
      rotate(90,150);
      for(i = 0;i< AvoidObstacleSideMoveCount; i++)
       move(100,150);
      rotate(-90,150);
     setNextHandleCollisionState(HandleCollisionState_noObstacle);
    }
  }
}
