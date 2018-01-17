/**
 * Created by ArayniMax
 */

struct structWheelEncoder{
  int left;
  int right;
  int leftMove;
  int rightMove;
} wheelEncoder;


// External Interrupt 4 Wheel encoder Left
interrupt [EXT_INT4] void ext_int4_isr(void)
{
 wheelEncoder.left++;
 wheelEncoder.leftMove++;
}

// External Interrupt 6 Wheel encoder Right
interrupt [EXT_INT6] void ext_int6_isr(void)
{
  wheelEncoder.right++;
  wheelEncoder.rightMove++;
}

void wheelencoder_init(){
 EICRB |= (0 << ISC61) | (1 << ISC60) | (0 <<ISC41) | (1 << ISC40);
 EIMSK |= (1 << INT6) | (1 << INT4);

 wheelEncoder.left = 0;
 wheelEncoder.right = 0;
 wheelEncoder.leftMove = 0;
 wheelEncoder.rightMove = 0;
}
