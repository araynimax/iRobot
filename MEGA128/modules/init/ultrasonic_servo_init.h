#define ULTRASONIC_SERVO_MAX 224 //BIGGEST TIMER VALUE
#define ULTRASONIC_SERVO_MID 181
#define ULTRASONIC_SERVO_MIN 125 //SMALLEST TIMER VALUE

#define ULTRASONIC_RANGE_DEGREE 180

#define ULTRASONIC_CALC_TIMER_VALUE(angle) (ULTRASONIC_SERVO_MAX - ULTRASONIC_SERVO_MIN) / ULTRASONIC_RANGE_DEGREE * angle + ULTRASONIC_SERVO_MIN + (ULTRASONIC_SERVO_MAX - ULTRASONIC_SERVO_MIN) / 2


int ultrasonic_servo_angle = 0;  //range is -90° to +90°
int ultrasonic_servo_angle_copy = 0;

int checkAngle(int*);

interrupt [TIM2_OVF] void timer2_ovf_isr(void){
  ULTRASONIC_SENSOR_SERVO = 0;
  if(checkAngle(&ultrasonic_servo_angle) != checkAngle(&ultrasonic_servo_angle_copy)){
     ultrasonic_servo_angle_copy = ultrasonic_servo_angle;
     TCNT2 = ULTRASONIC_CALC_TIMER_VALUE(ultrasonic_servo_angle);
     ULTRASONIC_SENSOR_SERVO = 1;
  }
}


int checkAngle(int* angle){
  int min = ULTRASONIC_RANGE_DEGREE / 2 * (-1);
  int max = ULTRASONIC_RANGE_DEGREE / 2;
  if(*angle > max)
     *angle = max;
  else if(*angle < min)
     *angle = min;
  return *angle;
}
