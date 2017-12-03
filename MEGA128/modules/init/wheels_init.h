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

/**
 * @param mm distance to travel (negative is backwards)
 * @param force if its 1 the queue will be cleared to force this action
 */
//@todo geschwindigkeit
void move(float mm,unsigned int force){
    int encode_changes = wheel_circumference / (mm < 0 ? mm * (-1) : mm) * wheel_encoder_amount;
    bit backwards = mm < 0 ? 1 : 0;

}

/**
 * @param degree
 * @param clockwise [optional] change the rotation direction
 * @param force if its 1 the queue will be cleared to force this action
 */
//@todo geschwindigkeit
void rotate(float degree,unsigned int force){
     int encoder_changes =  robot_circumference / wheel_circumference / 360 * (degree < 0 ? degree * (-1) : degree) * wheel_encoder_amount;
     bit clockwise = degree < 0 ? 0 : 1;
}
