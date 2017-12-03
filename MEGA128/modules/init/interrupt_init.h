void interrupt_init(){
  /**
   *
   * >>>EICRA<<<
   *
   * BIT 7: ISC31
   * BIT 6: ISC30
   * BIT 5: ISC21
   * BIT 4: ISC20
   * BIT 3: ISC11
   * BIT 2: ISC10
   * BIT 1: ISC01
   * BIT 0: ISC00
   */
  EICRA = 0b00000000;

  /**
   *
   * >>>EICRB<<<
   *
   * BIT 7: ISC71
   * BIT 6: ISC70
   * BIT 5: ISC61
   * BIT 4: ISC60
   * BIT 3: ISC51
   * BIT 2: ISC50
   * BIT 1: ISC41
   * BIT 0: ISC40
   */
  EICRB = 0b00010001;

  /**
   *
   * >>>EIMSK<<<
   *
   * BIT 7: INT7
   * BIT 6: INT6
   * BIT 5: INT5
   * BIT 4: INT4
   * BIT 3: INT3
   * BIT 2: INT2
   * BIT 1: INT1
   * BIT 0: INT0
   */
  EIMSK = 0b01010000;

  /**
   *
   * >>>EIFR<<<
   *
   * BIT 7: INTF7
   * BIT 6: INTF6
   * BIT 5: INTF5
   * BIT 4: INTF4
   * BIT 3: INTF3
   * BIT 2: INTF2
   * BIT 1: INTF1
   * BIT 0: INTF0
   */
  EIFR  = 0b00000000;

  // Global enable interrupts
  #asm("sei")
}
