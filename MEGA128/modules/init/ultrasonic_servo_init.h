#define ULTRASONIC_SERVO_MAX 225 //BIGGEST TIMER VALUE
#define ULTRASONIC_SERVO_MID 180
#define ULTRASONIC_SERVO_MIN 125 //SMALLEST TIMER VALUE

#define ULTRASONIC_RANGE_DEGREE 180

int ultrasonic_servo_angle = 0;  //range is -90° to +90°
int ultrasonic_servo_angle_copy = 0;
int ultrasonic_servo_breaks;
bit ultrasonic_servo_init = 1;

int checkAngle(int*);
int ULTRASONIC_CALC_TIMER_VALUE(int);

char str[10];

interrupt [TIM2_OVF] void timer2_ovf_isr(void){
  ULTRASONIC_SENSOR_SERVO = 0;
  if(checkAngle(&ultrasonic_servo_angle) != checkAngle(&ultrasonic_servo_angle_copy) || ultrasonic_servo_init == 1 ||ultrasonic_servo_breaks == 5){
     ultrasonic_servo_angle_copy = ultrasonic_servo_angle;
     ultrasonic_servo_init = 0;
     ultrasonic_servo_breaks = 1;
     TCNT2 = ULTRASONIC_CALC_TIMER_VALUE(ultrasonic_servo_angle);
     ULTRASONIC_SENSOR_SERVO = 1;
  }
  else
  {
    TCNT2 = 0;
    ultrasonic_servo_breaks++;
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

int ULTRASONIC_CALC_TIMER_VALUE(int angle){
  if(angle < 0){
     return ULTRASONIC_SERVO_MIN + (ULTRASONIC_SERVO_MID - ULTRASONIC_SERVO_MIN) -( (ULTRASONIC_SERVO_MID - ULTRASONIC_SERVO_MIN) / (ULTRASONIC_RANGE_DEGREE / 2.0)) * angle * (-1);
  }
  else {
      return ULTRASONIC_SERVO_MID + ((ULTRASONIC_SERVO_MAX - ULTRASONIC_SERVO_MID) / (ULTRASONIC_RANGE_DEGREE / 2.0)) * angle;
  }
}
