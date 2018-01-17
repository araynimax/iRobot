/**
 * Created by ArayniMax
 */

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
int initRound = 1;

void resetRobot(){
  MainQueueLength = -1;
  ObstacleQueueLength = -1;
  MotorRight.force = 0;
  MotorRight.encoder_changes = -1;
  MotorLeft.force = 0;
  MotorLeft.encoder_changes = -1;
  ultrasonic_servo.angle = 0;
  initRound = 1;
}

void setMainStateMachineState(int state){
  mainstatemachinestate = state != -1 ? state: mainstatemachinestate;
  if(mainstatemachinestate != copymainstatemachinestate){
    resetRobot();
  }
  copymainstatemachinestate = mainstatemachinestate;
}

void HandleRC5(){
  int rc5temp = rc5_receive();
  setMainStateMachineState(rc5temp);
}

void fnMainStateMachineState_stop(){

}

void fnMainStateMachineState_moveL(){
    if(initRound==1){
      moveRobot(1000,50);
      rotateRobot(90,50);
      moveRobot(1000,50);
      rotateRobot(180,50);
      moveRobot(1000,50);
      rotateRobot(-90,50);
      moveRobot(1000,50);
      rotateRobot(180,50);
      initRound = 0;
    }
    else if(MainQueueLength == -1){
      setMainStateMachineState(MainStateMachineState_stop);
    }

}

void fnMainStateMachineState_bug(){
if(LIGHT_SENSOR_LEFT > 900 || LIGHT_SENSOR_RIGHT > 900){

    if(LIGHT_SENSOR_RIGHT>LIGHT_SENSOR_LEFT){
    	MotorRight.force = -55;
       MotorLeft.force = 0;
    }
    else if(LIGHT_SENSOR_RIGHT<LIGHT_SENSOR_LEFT){
    	MotorRight.force = 0;
        MotorLeft.force = -55;
    }
    else if(LIGHT_SENSOR_RIGHT==LIGHT_SENSOR_LEFT){
    	MotorRight.force = -55;
        MotorLeft.force = -55;
    }
    else{
	MotorRight.force = 0;
        MotorLeft.force = 0;
    }
}
else{
  MotorRight.force = 0;
  MotorLeft.force = 0;
}
}

void fnMainStateMachineState_moth(){
  if(LIGHT_SENSOR_LEFT > 900 || LIGHT_SENSOR_RIGHT >900){
  if(LIGHT_SENSOR_RIGHT>LIGHT_SENSOR_LEFT){
    MotorRight.force = 0;
    MotorLeft.force = 55;
  }

  else if(LIGHT_SENSOR_RIGHT<LIGHT_SENSOR_LEFT){
    MotorRight.force = 55;
    MotorLeft.force = 0;
  }
  else if(LIGHT_SENSOR_RIGHT==LIGHT_SENSOR_LEFT){
    MotorRight.force = 55;
    MotorLeft.force = 55;
  }
  else{
    MotorRight.force = 0;
    MotorLeft.force = 0;
  }
}
else{
  MotorRight.force = 0;
  MotorLeft.force = 0;
}
}
/*
void fnMainStateMachineState_lineFollow(){
if(LINE_DETECTOR_MID_LEFT || LINE_DETECTOR_LEFT || LINE_DETECTOR_MID_RIGHT ||LINE_DETECTOR_RIGHT){
initRound = 0;
}
if(initRound == 0){
  if(LINE_DETECTOR_BOTH_RIGHT < LINE_DETECTOR_BOTH_LEFT && LINE_DETECTOR_BOTH_RIGHT+LINE_DETECTOR_BOTH_LEFT > 500){
      MotorLeft.force  = -45;
      MotorRight.force = 65;
    }

    else if(LINE_DETECTOR_BOTH_RIGHT > LINE_DETECTOR_BOTH_LEFT && LINE_DETECTOR_BOTH_RIGHT+LINE_DETECTOR_BOTH_LEFT > 500){
    MotorLeft.force  =  65;
      MotorRight.force = -45;
    }
    else if(LINE_DETECTOR_BOTH_RIGHT == LINE_DETECTOR_BOTH_LEFT && LINE_DETECTOR_BOTH_RIGHT+LINE_DETECTOR_BOTH_LEFT > 500){
      MotorLeft.force  = 55;
      MotorRight.force  = 55;
    }
    else{
      MotorLeft.force   = 55;
      MotorRight.force  = -55;
    }
}
else{
      MotorLeft.force  = 60;
      MotorRight.force = 60;
}
}
*/
void fnMainStateMachineState_lineFollow(){
static int lastMove = 0;
  if(LINE_DETECTOR_MID_LEFT || LINE_DETECTOR_LEFT && !LINE_DETECTOR_MID_RIGHT || !LINE_DETECTOR_RIGHT ){
      MotorLeft.force  = -45;
      MotorRight.force = 60;
      lastMove = 1;
    }

    else if(!LINE_DETECTOR_MID_LEFT || !LINE_DETECTOR_LEFT && LINE_DETECTOR_MID_RIGHT || LINE_DETECTOR_RIGHT){
    MotorLeft.force  =  60;
      MotorRight.force = -45;
      lastMove = 2;
    }
    else if(LINE_DETECTOR_MID_LEFT || LINE_DETECTOR_LEFT && LINE_DETECTOR_MID_RIGHT || LINE_DETECTOR_RIGHT){
      MotorLeft.force  = 55;
      MotorRight.force  = 55;
      lastMove = 3;
    }
    else{
      switch(lastMove){
      case 1:
          MotorLeft.force  =  60;
      MotorRight.force = -45;
      break;
      case 2:
          MotorLeft.force  = -45;
      MotorRight.force = 60;
      break;
      default:
            MotorLeft.force  = -45;
            MotorRight.force = 60;
      }
    }
}

int followSignal(){
wii_cam_read();
if(WiiCamData.coord_X[0] == 1023 && WiiCamData.coord_Y[0] == 1023 && WiiCamData.size[0] == 15)
return 0;
else if(!DISTANCE_SENSOR_FRONT_LEFT || !DISTANCE_SENSOR_FRONT_RIGHT || !BUMPER_LEFT || !BUMPER_RIGHT)
return 0;
else return 1;
}

void fnMainStateMachineState_robotFollow(){
if(followSignal() == 1){
	if(WiiCamData.coord_X[0] < 200){
      MotorLeft.force =   55;
      MotorRight.force =  0;
	}
	else if(WiiCamData.coord_X[0] >= 1023-200){
	  MotorRight.force =   55;
      MotorRight.force =   0;
	}
    else{
         MotorRight.force =   55;
         MotorLeft.force =   55;
    }

}
else{
 MotorLeft.force =   55;
 MotorRight.force = -55;
}

}




 int mapping_counter = -90;
 unsigned int mapping_plus = 0;
 int mapping_arrValues;//[180];

 //
 // if(mapping_plus == 1  && mapping_counter == 90){
 //   //finished
 //   setMainStateMachineState(MainStateMachineState_stop);
 // }else if(mapping_plus == 0 && mapping_counter == -90){
 //   mapping_counter = 0;
 //   mapping_plus = 1;
 // }
 // if(mapping_plus == 1 && mapping_counter < 90)mapping_counter++;
 // else if(mapping_counter > 0)mapping_counter--;

void fnMainStateMachineState_mapping(){
  // tx_buffer.data.ultrasonic_mapping[mapping_counter+89] = ultrasonic_data.distance;

if(mapping_counter == 90){
  mapping_counter = -90;
  setMainStateMachineState(MainStateMachineState_stop);
  return;
}

 ultrasonic_servo.angle = mapping_counter++;
 delay_ms(15);
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
