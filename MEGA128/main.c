/**
 * Created by ArayniMax
 */

#define PRINT(x) {char str[10];itoa(x,str);lcd_puts(str);}
#include <mega128.h>
#include <delay.h>
#include <stdlib.h>
#include "modules/init.h"

void main(void)
{
  initialize();
  while(1){
    lcd_clear();
    cyclestate();
    lcd_puts("Current State:");
    PRINT(mainstatemachinestate);
    delay_ms(50);
}

}
