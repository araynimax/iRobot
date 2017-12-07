// All defined datas are represented in millimetres!

// wheel
#define wheel_diameter 100
#define wheel_rubber   0.5

// wheel encoder paper
#define wheel_encoder_amount_white 30
#define wheel_encoder_amount_black 30

//robot
#define robot_diameter 150  //With wheels

//Math
#define PI 3.141592654

// Calculations
#define wheel_circumference PI * (wheel_diameter + wheel_rubber * 2)
#define robot_circumference PI * robot_diameter
#define wheel_encoder_amount wheel_encoder_amount_white + wheel_encoder_amount_black

#define wheel_movement_type_stop 1
#define wheel_movement_type_move 2
#define wheel_movement_type_rotate 3


struct structWheelMoving{
  int encoder_changes;
  int backwards;
  int clockwise;
  int movement_type;
  int force;
} WheelMoving;


/**
 * @param mm distance to travel (negative is backwards)
 * @param force if its 1 the queue will be cleared to force this action
 */
//@todo geschwindigkeit
void move(float mm,unsigned int force){
    WheelMoving.encoder_changes = wheel_circumference / (mm < 0.0 ? mm * (-1.0) : mm) * wheel_encoder_amount;
    WheelMoving.backwards = mm < 0 ? 1 : 0;
    WheelMoving.movement_type = wheel_movement_type_move;
    wheelEncoder.left = 0;
    wheelEncoder.right = 0;
    WheelMoving.force = force;
}
/**
 * @param degree
 * @param clockwise [optional] change the rotation direction
 * @param force if its 1 the queue will be cleared to force this action
 */
//@todo geschwindigkeit
void rotate(float degree,unsigned int force){
     WheelMoving.encoder_changes =  robot_circumference / (float) wheel_circumference / 360.0 * (degree < 0 ? degree * (-1) : degree) * wheel_encoder_amount;
     WheelMoving.clockwise = degree < 0 ? 0 : 1;
     WheelMoving.movement_type = wheel_movement_type_rotate;
     wheelEncoder.left = 0;
     wheelEncoder.right = 0;
     WheelMoving.force = force;
}

void wheels_init(){
  WheelMoving.movement_type = wheel_movement_type_stop;
}



interrupt [TIM1_OVF] void timer1_ovf_isr(void)
{
   //TCNT1L
   ENGINE_ENABLE_RIGHT = 0;
   ENGINE_ENABLE_LEFT = 0;

   if(WheelMoving.movement_type == wheel_movement_type_move || WheelMoving.movement_type == wheel_movement_type_rotate || wheelEncoder.left + wheelEncoder.right < WheelMoving.encoder_changes * 2){
     if(WheelMoving.movement_type == wheel_movement_type_move){
       ENGINE_ENABLE_RIGHT = 1;
       ENGINE_ENABLE_LEFT = 1;
       if(WheelMoving.backwards == 1){
         ENGINE_DIRECTION_RIGHT = 1;
         ENGINE_DIRECTION_LEFT = 1;
       }
       else{
         ENGINE_DIRECTION_RIGHT = 0;
         ENGINE_DIRECTION_LEFT = 0;
       }
     }
     else if(WheelMoving.movement_type == wheel_movement_type_rotate){
       ENGINE_ENABLE_RIGHT = 1;
       ENGINE_ENABLE_LEFT = 1;
       if(WheelMoving.clockwise == 1){
         ENGINE_DIRECTION_RIGHT = 1;
         ENGINE_DIRECTION_LEFT = 0;
       }
       else{
         ENGINE_DIRECTION_RIGHT = 0;
         ENGINE_DIRECTION_LEFT = 1;
       }
     }

     TCNT1L = 255/100.0 * WheelMoving.force;
   }
   else{
     ENGINE_ENABLE_RIGHT = 0;
     ENGINE_ENABLE_LEFT = 0;
     ENGINE_DIRECTION_RIGHT = 0;
     ENGINE_DIRECTION_LEFT = 0;
   }

   lcd_clear();
   lcd_putsf(".");


}
