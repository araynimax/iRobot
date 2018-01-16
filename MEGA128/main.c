/*******************************************************
This program was created by the
CodeWizardAVR V3.17 UL Advanced
Automatic Program Generator
� Copyright 1998-2014 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

Project : Mobile Robotik
Version : 0.0.1
Date    : 16.11.2017
Author  : ArayniMax
Company :
Comments:


Chip type               : ATmega128
Program type            : Application
AVR Core Clock frequency: 16,000000 MHz
Memory model            : Small
External RAM size       : 0
Data Stack size         : 1024
*******************************************************/
#define PRINT(x) {char str[10];itoa(x,str);lcd_puts(str);}
#include <mega128.h>
#include <delay.h>
#include <stdlib.h>

#include "modules/init.h"
#include "modules/demo.h"


void main(void)
{
  initialize(); 
  while(1){     
  char str[10];  
    lcd_clear();
    cyclestate();
    PRINT(mainstatemachinestate);
    delay_ms(50);            
}




}
