#define MainStateMachineState_stop 0
#define MainStateMachineState_moveL 1
#define MainStateMachineState_bug 2
#define MainStateMachineState_moth 3
#define MainStateMachineState_lineFollow 4
#define MainStateMachineState_robotFollow 5
#define MainStateMachineState_mapping 6
#define MainStateMachineState_autonomic 7


int mainstatemachinestate = MainStateMachineState_stop;
int copymainstatemachinestate = MainStateMachineState_stop;

void fnMainStateMachineState_stop(){
  MainQueueLength = -1;
  ObstacleQueueLength = -1;
  MotorRight.force = 0;
  MotorRight.encoder_changes = -1;
  MotorLeft.force = 0;
  MotorLeft.encoder_changes = -1;
}

void HandleRC5(){
  int rc5temp = rc5_receive();
  mainstatemachinestate = rc5temp != -1 ? rc5temp: mainstatemachinestate;
  if(mainstatemachinestate != copymainstatemachinestate){
    fnMainStateMachineState_stop();
  }
  copymainstatemachinestate = mainstatemachinestate;
}

void fnMainStateMachineState_moveL(){

}

void fnMainStateMachineState_bug(){
if(LIGHT_SENSOR_LEFT > 800 || LIGHT_SENSOR_RIGHT > 800){

    if(LIGHT_SENSOR_RIGHT>LIGHT_SENSOR_LEFT){
    	MotorRight.force = -55;
       MotorLeft.force = 0;
    }
    else if(LIGHT_SENSOR_RIGHT<LIGHT_SENSOR_LEFT){
    	MotorRight.force = 0;
        MotorLeft.force = -55;
    }
    else{
    	MotorRight.force = -55;
        MotorLeft.force = -55;
    }
}
}

void fnMainStateMachineState_moth(){
  if(LIGHT_SENSOR_LEFT > 800 || LIGHT_SENSOR_RIGHT > 800){
  if(LIGHT_SENSOR_RIGHT>LIGHT_SENSOR_LEFT){
    MotorRight.force = 0;
    MotorLeft.force = 55;
  }

  else if(LIGHT_SENSOR_RIGHT<LIGHT_SENSOR_LEFT){
    MotorRight.force = 55;
    MotorLeft.force = 0;
  }
  else{
    MotorRight.force = 55;
    MotorLeft.force = 55;
  }
}
else{
  MotorRight.force = 0;
  MotorLeft.force = 0;
}
}

void fnMainStateMachineState_lineFollow(){
  if(LINE_DETECTOR_MID_LEFT || LINE_DETECTOR_LEFT && !LINE_DETECTOR_MID_RIGHT ||!LINE_DETECTOR_RIGHT ){
      MotorLeft.force  = -45;
      MotorRight.force = 55;
    }

    else if(!LINE_DETECTOR_MID_LEFT || !LINE_DETECTOR_LEFT && LINE_DETECTOR_MID_RIGHT || LINE_DETECTOR_RIGHT){
    MotorLeft.force  =  55;
      MotorRight.force = -45;
    }
    else if(LINE_DETECTOR_MID_LEFT || LINE_DETECTOR_LEFT && LINE_DETECTOR_MID_RIGHT || LINE_DETECTOR_RIGHT){
      MotorLeft.force  = 55;
      MotorRight.force  = 55;
    }
    else{
      MotorLeft.force  = 0;
      MotorRight.force  = 0;
    }
}

void fnMainStateMachineState_robotFollow(){

}

void fnMainStateMachineState_mapping(){

}

void fnMainStateMachineState_autonomic(){

}

void cyclestate() {
  HandleRC5();
  switch (mainstatemachinestate) {
  case MainStateMachineState_stop:
      fnMainStateMachineState_stop();
    break;

  case MainStateMachineState_moveL:
      fnMainStateMachineState_moveL();
    break;

  case MainStateMachineState_bug:
      fnMainStateMachineState_bug();
    break;

  case MainStateMachineState_moth:
      fnMainStateMachineState_moth();
    break;

  case MainStateMachineState_lineFollow:
      fnMainStateMachineState_lineFollow();
    break;

  case MainStateMachineState_robotFollow:
      fnMainStateMachineState_robotFollow();
    break;

  case MainStateMachineState_mapping:
      fnMainStateMachineState_mapping();
    break;

  case MainStateMachineState_autonomic:
      fnMainStateMachineState_autonomic();
    break;

  default:
    mainstatemachinestate = MainStateMachineState_stop;

  }
}
