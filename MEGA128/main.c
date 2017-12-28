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

//#define max(a,b) a>b ? a : b

#include "modules/init/init.h"
#include "modules/demo.h"



void main(void)
{
  char str[10];
  int changes = 0,i;
      initialize();
      delay_ms(5000);
      move(3000,200);
       rotate(90,150);
       move(1000,200);
       rotate(180,150);  
       move(1000,200);
       rotate(-90,150);
       move(3000,200);
             






while(1){


}



}
