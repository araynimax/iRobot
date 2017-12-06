#include <lcd.h>

/**
 * Port A
 */

//NOT IN USE #define PORTA.0
#define ENGINE_ENABLE_RIGHT         PORTA.1
#define REMOTE_CONTROL               PINA.2
#define ENGINE_ENABLE_LEFT          PORTA.3
#define BUMPER_LEFT                  PINA.4
#define ENGINE_DIRECTION_RIGHT      PORTA.5
#define BUMPER_RIGHT                 PINA.6
#define ENGINE_DIRECTION_LEFT       PORTA.7

/**
 * Port B
 */

#define DISTANCE_SENSOR_LEFT         PINB.0
#define ULTRASONIC_SENSOR_TRIGGER   PORTB.1
#define DISTANCE_SENSOR_FRONT_LEFT   PINB.2
#define WII_CAM_SCL                 PORTB.3
#define DISTANCE_SENSOR_FRONT_RIGHT  PINB.4
#define WII_CAM_SDA                  PINB.5
#define DISTANCE_SENSOR_RIGHT        PINB.6
//NOT IN USE #define PORTB.7

/**
 * Port C
 */

 #asm
    .equ __lcd_port=0x15 ;PORTC //Registers PortC for LCD
 #endasm

//LCD USES THIS  #define PORTC.0
//LCD USES THIS  #define PortC.1
//LCD USES THIS  #define PortC.2
//LCD USES THIS  #define PortC.3
//LCD USES THIS  #define PortC.4
//LCD USES THIS  #define PortC.5
//LCD USES THIS  #define PortC.6
//LCD USES THIS  #define PortC.7

/**
 * Port D
 */

#define WIFI_BOARD_SDA               PIND.0
#define WIFI_BOARD_SCL              PORTD.1
//NOT IN USE #define PORTD.2
//NOT IN USE #define PORTD.3
//NOT IN USE #define PORTD.4
//NOT IN USE #define PORTD.5
//NOT IN USE #define PORTD.6
#define ULTRASONIC_SENSOR_SERVO     PORTD.7

/**
 * Port E
 */

//NOT IN USE #define PORTE.0
//NOT IN USE #define PORTE.1
//NOT IN USE #define PORTE.2
//NOT IN USE #define PORTE.3
#define WHEEL_ENCODER_LEFT           PINE.4
//NOT IN USE #define PORTE.5
#define WHEEL_ENCODER_RIGHT          PINE.6
#define ULTRASONIC_SENSOR_ECHO       PINE.7

/**
 * Port F
 */

#define LINE_DETECTOR_LEFT           PINF.0
#define LIGHT_SENSOR_LEFT            adc_data[1]
#define LINE_DETECTOR_MID_LEFT       PINF.2
#define LIGHT_SENSOR_RIGHT           adc_data[3]
#define LINE_DETECTOR_MID_RIGHT      PINF.4
//NOT IN USE #define PORTF.5
#define LINE_DETECTOR_RIGHT          PINF.6
//NOT IN USE #define PORTF.7



void port_init(){

// Port initialization
DDRA  = 0b10101010;
DDRB  = 0b00001010;
//LCD USES THIS PORT DDRC  = 0b11111111;
DDRD  = 0b00000010;
DDRE  = 0b00000000;
DDRF  = 0b00100000;

// LCD initialization
lcd_init(16);
}
