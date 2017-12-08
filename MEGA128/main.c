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

#include <mega128.h>
#include <delay.h>
#include <stdlib.h>
#include "modules/init/init.h"
#include "modules/demo.h"

void main(void)
{
  int counter = 0;
initialize();


move(10000,100);
while(1){
  counter = counter < 300 ? counter+1 : 0;
  OCR1BL = counter < 255 ? counter : 255;
  OCR1CL = counter < 255 ? counter : 255;
  delay_ms(50);
  lcd_clear();
  debug(counter < 255 ? counter : 255);
  lcd_gotoxy(0,1);
  debug(wheelEncoder.left);
  lcd_puts(":");
  debug(wheelEncoder.right);
}
}
