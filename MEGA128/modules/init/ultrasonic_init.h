/* union declaration */
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


// Timer3 input capture interrupt service routine
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


// Timer3 output compare A interrupt service routine
interrupt [TIM3_COMPA] void timer3_compa_isr(void)
{
  // Trigger is fired every 100 ms
  ULTRASONIC_SENSOR_TRIGGER = 1;
  delay_us(10);
  ULTRASONIC_SENSOR_TRIGGER = 0;
}

void ultrasonic_init(){
    ultrasonic_data.distance = -1;
}
