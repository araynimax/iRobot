struct structWheelEncoder{
  int left;
  int right;
} wheelEncoder;

// External Interrupt 4 Wheel encoder Left
interrupt [EXT_INT4] void ext_int4_isr(void)
{
 wheelEncoder.left++;
}

// External Interrupt 6 Wheel encoder Right
interrupt [EXT_INT6] void ext_int6_isr(void)
{
  wheelEncoder.right++;
}

void wheelencoder_init(){
 wheelEncoder.left = 0;
 wheelEncoder.right = 0;
}
