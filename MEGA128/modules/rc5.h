#define RC5TIME 	1.778e-3		// 1.778msec
#define	XTAL		16.0E6
#define PULSE_MIN	(unsigned char)(XTAL / 512 * RC5TIME * 0.2 + 0.5)
#define PULSE_1_2	(unsigned char)(XTAL / 512 * RC5TIME * 0.8 + 0.5)
#define PULSE_MAX	(unsigned char)(XTAL / 512 * RC5TIME * 1.2 + 0.5)


//@todo umstrukturieren
 bit	         rc5_bit=1;			// bit value
 unsigned char rc5_time=0;			// count bit time
 unsigned int  rc5_data=0;			// store result
 unsigned int  rc5_tmp;
 unsigned char rc5_ucToggle;
 unsigned char rc5_ucAdress;
 unsigned char rc5_ucData;



//Timer 0 overflow interrupt service routine
interrupt [TIM0_OVF] void timer0_ovf_isr(void)
{
  TCNT0 = 254;			
  		// 2 * 256 = 512 cycle
  if( ++rc5_time > PULSE_MAX )                  // count pulse time
  {
    if( !(rc5_tmp & 0x4000) && (rc5_tmp & 0x2000) )	// only if 14 bits received
      rc5_data = rc5_tmp;
    rc5_tmp = 0;
  }

  if (rc5_bit != REMOTE_CONTROL)                       // change detect
  {
    rc5_bit = !rc5_bit;	                        // 0x00 -> 0xFF -> 0x00

    if( rc5_time < PULSE_MIN )			// too short
      rc5_tmp = 0;

    if( !rc5_tmp || rc5_time > PULSE_1_2 )          // start or long pulse time
    {
      if( !(rc5_tmp & 0x4000) )			// not to many bits
        rc5_tmp = rc5_tmp << 1;				// shift
      if(!rc5_bit)		                // inverted bit
        rc5_tmp = rc5_tmp | 1;				// insert new bit
      rc5_time = 0;				// count next pulse time
    }
  }
}


int rc5_receive()
{
  unsigned int temp_rc5_data;

  #asm("cli")
  temp_rc5_data = rc5_data;			// read two bytes from interrupt !
  rc5_data = 0;
  #asm("sei")       
  if( temp_rc5_data )
  {
    rc5_ucToggle = temp_rc5_data >> 11 & 1;
    rc5_ucAdress = temp_rc5_data >> 6 & 0x1F;
    rc5_ucData = (temp_rc5_data & 0x3F) | (~temp_rc5_data >> 7 & 0x40);
    return rc5_ucData;
  }
  else
    return -1;
}

void rc5_init(){
  ASSR   = (0 << AS0) | (0 << TCN0UB) | (0 << OCR0UB) | (0 << TCR0UB);
  TCCR0  = (1 << CS02) | (1 << CS01) | (0 << CS00);
  TCNT0  = 0x00;
  OCR0   = 0x00;
  TIMSK |= (1 << TOIE0 ) | (0 << OCIE0);
}
