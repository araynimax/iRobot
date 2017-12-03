void timer_init()
{
    /**
     *  >>>>>>>>>> Timer/Counter 0 initialization <<<<<<<<<<<<
     *  @INFO USED BY RC5-RECEIVER!!!
     */
    /**
     *
     * >>>ASSR<<<
     *
     * BIT 7:
     * BIT 6:
     * BIT 5:
     * BIT 4:
     * BIT 3: AS0
     * BIT 2: TCN0UB
     * BIT 1: OCR0UB
     * BIT 0: TCR0UB
     */
    ASSR = 0b00000000;

    /**
     *
     * >>>TCCR0<<<
     *
     * BIT 7: CS00
     * BIT 6: CS01
     * BIT 5: CS02
     * BIT 4: WGM01
     * BIT 3: COM00
     * BIT 2: COM01
     * BIT 1: WGM00
     * BIT 0: FOC0
     */
    TCCR0 = 0b00000110;

    /**
   * >>> Counter Register <<<
   */
    TCNT0 = 0x00;

    /**
   * >>> Output Compare Register <<<
   */
    OCR0 = 0x00;

    /**
     * >>>>>>>>>> Timer/Counter 1 initialization <<<<<<<<<<<<
     */

    /**
     *
     * >>> TCCR1A <<<
     *
     * BIT 7: COM1A1
     * BIT 6: COM1A0
     * BIT 5: COM1B1
     * BIT 4: COM1B0
     * BIT 3: COM1C1
     * BIT 2: COM1C0
     * BIT 1: WGM11
     * BIT 0: WGM10
     */
    TCCR1A = 0b00000000;

    /**
     *
     * >>> TCCR1B <<<
     *
     * BIT 7: ICNC1
     * BIT 6: ICES1
     * BIT 5:
     * BIT 4: WGM13
     * BIT 3: WGM12
     * BIT 2: CS12
     * BIT 1: CS11
     * BIT 0: CS10
     */
    TCCR1B = 0b00000000;

    /**
   * >>> Counter Register (High/Low Byte) <<<
   */
    TCNT1H = 0x00;
    TCNT1L = 0x00;

    /**
   * >>> Input Capture Register (High/Low Byte) <<<
   */
    ICR1H = 0x00;
    ICR1L = 0x00;

    /**
   * >>> Output Compare Register (High/Low Byte)<<<
   */
    //A
    OCR1AH = 0x00;
    OCR1AL = 0x00;
    //B
    OCR1BH = 0x00;
    OCR1BL = 0x00;
    //C
    OCR1CH = 0x00;
    OCR1CL = 0x00;

    /**
     *  >>>>>>>>>> Timer/Counter 2 initialization <<<<<<<<<<<<
     */

    /**
     *
     * >>> TCCR2 <<<
     *
     * BIT 7: FOC2
     * BIT 6: WGM20
     * BIT 5: COM21
     * BIT 4: COM20
     * BIT 3: WGM21
     * BIT 2: CS22
     * BIT 1: CS21
     * BIT 0: CS20
     */
    TCCR2 = 0b00000000;

    /**
    * >>> Counter Register <<<
    */
    TCNT2 = 0x00;

    /**
    * >>> Output Compare Register<<<
    */
    OCR2 = 0x00;

    /**
     * >>>>>>>>>> Timer/Counter 3 initialization <<<<<<<<<<<<
     * @INFO USED BY ULTRASONIC_SENSOR!!!
     */

    /**
     *
     * >>> TCCR3A <<<
     *
     * BIT 7: COM3A1
     * BIT 6: COM3A0
     * BIT 5: COM3B1
     * BIT 4: COM3B0
     * BIT 3: COM3C1
     * BIT 2: COM3C0
     * BIT 1: WGM31
     * BIT 0: WGM30
     */
    TCCR3A = 0b00000000;

    /**
     *
     * >>> TCCR3B <<<
     *
     * BIT 7: ICNC3
     * BIT 6: ICES3
     * BIT 5:
     * BIT 4: WGM33
     * BIT 3: WGM32
     * BIT 2: CS32
     * BIT 1: CS31
     * BIT 0: CS30
     */
    TCCR3B = 0b11001011;

    /**
   * >>> Counter Register (High/Low Byte) <<<
   */
    TCNT3H = 0x00;
    TCNT3L = 0x00;

    /**
   * >>> Input Capture Register (High/Low Byte) <<<
   */
    ICR3H = 0x00;
    ICR3L = 0x00;

    /**
   * >>> Output Compare Register (High/Low Byte)<<<
   */
    //A
    OCR3AH = 0x61;
    OCR3AL = 0xA8;
    //B
    OCR3BH = 0x00;
    OCR3BL = 0x00;
    //C
    OCR3CH = 0x00;
    OCR3CL = 0x00;

    /**
   * Timer interrupt Register
   */

    /**
     *
     * >>> TIMSK <<<
     *
     * BIT 7: OCIE2
     * BIT 6: TOIE2
     * BIT 5: TICIE1
     * BIT 4: OCIE1A
     * BIT 3: OCIE1B
     * BIT 2: TOIE1
     * BIT 1: OCIE0
     * BIT 0: TOIE0
     */
    TIMSK = 0b00000001;

    /**
     *
     * >>> ETIMSK <<<
     *
     * BIT 7:
     * BIT 6:
     * BIT 5: TICIE3
     * BIT 4: OCIE3A
     * BIT 3: OCIE3B
     * BIT 2: TOIE3
     * BIT 1: OCIEC
     * BIT 0: OCIE1C
     */
    ETIMSK = 0b00110000;
}
