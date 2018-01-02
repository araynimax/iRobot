#define ULTRASONIC_SERVO_MAX 225 //BIGGEST TIMER VALUE
#define ULTRASONIC_SERVO_MID 180
#define ULTRASONIC_SERVO_MIN 125 //SMALLEST TIMER VALUE
#define ULTRASONIC_RANGE_DEGREE 180

int checkAngle(int*);
int ULTRASONIC_CALC_TIMER_VALUE(int);

union unionUltraSonic
{
  unsigned char byte[2];
  unsigned int  word;
};

struct structUltraSonic{
	unsigned int iRisingEdge, iFallingEdge;
	unsigned int iTime;
	int distance;
	union unionUltraSonic icr3;
} ultrasonic_data;

struct structUltraSonicServo{
  int angle;  //range is -90° to +90°
  int angle_copy;
  int breaks;
  unsigned int init;
} ultrasonic_servo;

//TODO RENAME STRUCTURES AND PUT IT TOGETHER

interrupt [TIM3_CAPT] void timer3_capt_isr(void)
{
  ultrasonic_data.icr3.byte[0] = ICR3L;
  ultrasonic_data.icr3.byte[1] = ICR3H;
  if (ULTRASONIC_SENSOR_ECHO)
  {
    // Rising Edge of ECHO
    ultrasonic_data.iRisingEdge = ultrasonic_data.icr3.word;
    // Capture next Input on falling edge
    TCCR3B &= ~(1<<ICES3);
  }
  else
  {
    // Faling Edge of ECHO
    ultrasonic_data.iFallingEdge = ultrasonic_data.icr3.word;
     // Capture next Input on rising edge
    TCCR3B |= (1<<ICES3);
    // Calculate length of ECHO (time per count is 4 us)
    ultrasonic_data.iTime = (ultrasonic_data.iFallingEdge-ultrasonic_data.iRisingEdge)*4;
    ultrasonic_data.distance = ultrasonic_data.iTime/52.2;
  }
}

interrupt [TIM3_COMPA] void timer3_compa_isr(void)
{
  // Trigger is fired every 100 ms
  ULTRASONIC_SENSOR_TRIGGER = 1;
  delay_us(10);
  ULTRASONIC_SENSOR_TRIGGER = 0;
}

interrupt [TIM2_OVF] void timer2_ovf_isr(void){
  ULTRASONIC_SENSOR_SERVO = 0;
  if(checkAngle(&ultrasonic_servo.angle) != checkAngle(&ultrasonic_servo.angle_copy) || ultrasonic_servo.init == 1 ||ultrasonic_servo.breaks == 5){
     ultrasonic_servo.angle_copy = ultrasonic_servo.angle;
     ultrasonic_servo.init = 0;
     ultrasonic_servo.breaks = 1;
     TCNT2 = ULTRASONIC_CALC_TIMER_VALUE(ultrasonic_servo.angle);
     ULTRASONIC_SENSOR_SERVO = 1;
  }
  else
  {
    TCNT2 = 0;
    ultrasonic_servo.breaks++;
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


void ultrasonic_init(){
    //Ultrasonic Data Timer
    TCCR3A  = (0 << COM3A1) | (0 << COM3A0) | (0 <<COM3B1) | (0 << COM3B0) | (0 << COM3C1) | (0 << COM3C0) | (0 << WGM31) | (0 << WGM30);
    TCCR3B  = (1 << ICNC3) | (1 << ICES3) | (0 << WGM33) | (0 << WGM32) | (0 << CS32) | (1 << CS31) | (1 << CS30);
    TCNT3H  = 0x00;
    TCNT3L  = 0x00;
    ICR3H   = 0x00;
    ICR3L   = 0x00;
    OCR3AH  = 0x61;
    OCR3AL  = 0xA8;
    OCR3BH  = 0x00;
    OCR3BL  = 0x00;
    OCR3CH  = 0x00;
    OCR3CL  = 0x00;
    ETIMSK |= (1 << TICIE3) | (1 << OCIE3A) | (0 << OCIE3B) | (0 << TOIE3);

    //Ultrasonic Servo Timer
    TCCR2   = (0 << FOC2) | (0 << WGM20) | (0 <<COM21) | (0 << COM20) | (0 << WGM21) | (1 << CS22) | (0 << CS21) | (0 << CS20);
    TCNT2   = 0xB5;
    OCR2    = 0x00;
    TIMSK  |= (0 << OCIE2) | (1 << TOIE2) ;

    ultrasonic_data.distance = -1;
    ultrasonic_servo.init = 1;
}
