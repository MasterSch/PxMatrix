
#include <PxMatrix.h>

#ifdef ESP32

#define P_LAT 22
#define P_A 19
#define P_B 23
#define P_C 18
#define P_D 5
#define P_E 15
#define P_OE 2
hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

#endif

#ifdef ESP8266

#include <Ticker.h>
Ticker display_ticker;
#define P_LAT 16
#define P_A 5
#define P_B 4
#define P_C 15
#define P_D 12
#define P_E 0
#define P_OE 2

#endif
// Pins for LED MATRIX

#define matrix_width 32
#define matrix_height 16

uint8_t display_draw_time=0;

PxMATRIX display(matrix_width,matrix_height,P_LAT, P_OE,P_A,P_B,P_C);
//PxMATRIX display(64,32,P_LAT, P_OE,P_A,P_B,P_C,P_D);
//PxMATRIX display(64,64,P_LAT, P_OE,P_A,P_B,P_C,P_D,P_E);

// Some standard colors
uint16_t myRED = display.color565(255, 0, 0);
uint16_t myGREEN = display.color565(0, 255, 0);
uint16_t myBLUE = display.color565(0, 0, 255);
uint16_t myWHITE = display.color565(255, 255, 255);
uint16_t myYELLOW = display.color565(255, 255, 0);
uint16_t myCYAN = display.color565(0, 255, 255);
uint16_t myMAGENTA = display.color565(255, 0, 255);
uint16_t myBLACK = display.color565(0, 0, 0);

uint16_t myCOLORS[8]={myRED,myGREEN,myBLUE,myWHITE,myYELLOW,myCYAN,myMAGENTA,myBLACK};

uint8_t static weather_icons[]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x00,0x00,0xff,0xe0,0x00,0x00,0x00,0x00,0xff,0xe0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0xff,0x07,0xff,0x07,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x00,0x00,0x07,0xff,0x07,0xff,0x07,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xe0,0xff,0xe0,0xff,0xe0,0xff,0xe0,0x00,0x00,0x00,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0xff,0xe0,0xff,0xe0,0xff,0xe0,0xff,0xe0,0xff,0xe0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xe0,0xff,0xe0,0xff,0xe0,0xff,0xe0,0xff,0xe0,0x00,0x00,0x00,0x00,0x00,0x00
  ,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xe0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0xff,0x07,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0xdf,0x07,0xdf,0x07,0xdf,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0xff,0x07,0xff,0x07,0xff,0x07,0xff,0x07,0xff,0x07,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0xff,0x07,0xff,0x07,0xff,0x07,0xff,0x07,0xff,0x07,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xe0,0xff,0xe0,0xff,0xe0,0xff,0xe0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x00,0x20,0x07,0xff,0x07,0xff,0x07,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0xff,0x00,0x00,0x00,0x00,0x07,0xff,0x07,0xff,0x07,0xdf,0x00,0x00,0x00,0x00,0x07,0xff,0x00,0x00,0x00,0x00,0xff,0xe0,0xff,0xe0,0xff,0xe0,0xff,0xe0,0xff,0xe0,0xff,0xe0,0xff,0xe0,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xe0,0xff,0xe0,0xff,0xe0,0xff,0xe0,0xff,0xe0,0xff,0xe0,0xff,0xe0,0x00,0x00,0x00,0x00
  ,0x00,0x00,0x00,0x00,0xff,0xe0,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xe0,0x00,0x00,0x00,0x00,0x00,0x20,0x00,0x00,0x00,0x00,0xff,0xe0,0x00,0x00,0xff,0xe0,0xff,0xe0,0xff,0xe0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0xff,0x07,0xdf,0x07,0xff,0x07,0xff,0x07,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0xff,0x07,0xff,0x07,0xff,0x07,0xff,0x07,0xff,0x07,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0xff,0x07,0xff,0x07,0xff,0x07,0xff,0x07,0xff,0x07,0xff,0x07,0xff,0x07,0xff,0x00,0x00,0x00,0x00,0x07,0xff,0x07,0xff,0x07,0xff,0x07,0xff,0x07,0xff,0x07,0xff,0x07,0xff,0x07,0xff,0x00,0x00,0x00,0x00,0x00,0x20,0x00,0x00,0xff,0xe0,0xff,0xe0,0xff,0xe0,0xff,0xe0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0xff,0x07,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0xff,0x07,0xff,0x00,0x00,0x00,0x00,0xff,0xe0,0xff,0xe0,0xff,0xe0,0xff,0xe0,0xff,0xe0,0xff,0xe0,0xff,0xe0,0xff,0xe0,0xff,0xe0,0x00,0x00,0xff,0xe0,0xff,0xe0,0xff,0xe0,0xff,0xe0,0xff,0xe0,0xff,0xe0,0xff,0xe0,0xff,0xe0,0xff,0xe0,0x00,0x00
  ,0x00,0x20,0x00,0x00,0x00,0x00,0xff,0xe0,0xff,0xe0,0xff,0xe0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0xff,0x07,0xff,0x07,0xff,0xff,0xe0,0xff,0xe0,0xff,0xe0,0x00,0x00,0xff,0xe0,0x00,0x00,0x07,0xff,0x07,0xff,0x07,0xdf,0x07,0xff,0x07,0xdf,0x07,0xff,0x07,0xff,0x00,0x00,0x00,0x00,0x07,0xff,0x07,0xff,0x07,0xff,0x07,0xff,0x07,0xff,0x07,0xff,0x07,0xff,0x07,0xff,0x00,0x00,0x00,0x00,0x07,0xff,0x07,0xff,0x07,0xff,0x07,0xff,0x07,0xdf,0x07,0xff,0x07,0xff,0x07,0xff,0x07,0xff,0x00,0x00,0x07,0xff,0x07,0xff,0x07,0xff,0x07,0xff,0x07,0xdf,0x07,0xff,0x07,0xff,0x07,0xff,0x07,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xe0,0xff,0xe0,0xff,0xe0,0xff,0xe0,0x00,0x00,0x00,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0xff,0x00,0x00,0x07,0xff,0x07,0xff,0x07,0xff,0x00,0x00,0x07,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xe0,0xff,0xe0,0xff,0xe0,0xff,0xe0,0xff,0xe0,0xff,0xe0,0xff,0xe0,0xff,0xe0,0xff,0xe0,0x00,0x00,0xff,0xe0,0xff,0xe0,0xff,0xe0,0xff,0xe0,0x07,0xdf,0x07,0xdf,0x07,0xff,0xff,0xe0,0xff,0xe0,0x00,0x00
  ,0x00,0x00,0xff,0xe0,0x00,0x00,0xff,0xe0,0xff,0xe0,0xff,0xe0,0x00,0x00,0xff,0xe0,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0xff,0x07,0xdf,0x07,0xff,0x07,0xff,0x07,0xff,0x07,0xff,0xff,0xe0,0x00,0x20,0x00,0x00,0x07,0xdf,0x07,0xdf,0x07,0xdf,0x07,0xff,0x07,0xff,0x07,0xff,0x07,0xff,0x07,0xff,0x07,0xff,0x00,0x00,0x07,0xff,0x07,0xff,0x07,0xff,0x07,0xff,0x07,0xff,0x07,0xff,0x07,0xff,0x07,0xff,0x07,0xff,0x00,0x00,0x00,0x00,0x07,0xff,0x07,0xff,0x07,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0xdf,0x07,0xdf,0x07,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xe0,0xff,0xe0,0xff,0xe0,0xff,0xe0,0xff,0xe0,0xff,0xe0,0xff,0xe0,0x00,0x00,0x00,0x00,0x07,0xff,0x07,0xff,0x07,0xff,0x07,0xff,0xff,0xff,0x07,0xff,0x07,0xff,0x07,0xdf,0x07,0xff,0x00,0x00,0x07,0xff,0x00,0x00,0x00,0x00,0x07,0xff,0x07,0xff,0x07,0xdf,0x00,0x00,0x00,0x00,0x07,0xff,0x00,0x00,0xff,0xe0,0xff,0xe0,0xff,0xe0,0xff,0xe0,0xff,0xe0,0xff,0xe0,0xff,0xe0,0xff,0xe0,0xff,0xe0,0x00,0x00,0xff,0xe0,0xff,0xe0,0xff,0xe0,0x07,0xff,0x07,0xff,0x07,0xff,0x07,0xff,0x07,0xff,0x07,0xff,0x00,0x00
  ,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xe0,0xff,0xe0,0xff,0xe0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0xff,0x07,0xff,0x07,0xff,0x07,0xdf,0x07,0xff,0x07,0xff,0x07,0xff,0x07,0xff,0x00,0x00,0x00,0x00,0x07,0xff,0x07,0xff,0x07,0xff,0x07,0xff,0x07,0xff,0x07,0xff,0x07,0xff,0x07,0xff,0x07,0xff,0x00,0x00,0x00,0x00,0x07,0xff,0x07,0xff,0x07,0xff,0x07,0xff,0x07,0xff,0x07,0xff,0x07,0xff,0x00,0x00,0x00,0x00,0x00,0x20,0x00,0x00,0x07,0xff,0x00,0x20,0xff,0xff,0x00,0x00,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0xff,0x00,0x00,0xff,0xff,0x00,0x00,0xff,0xff,0x00,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xe0,0xff,0xe0,0xff,0xe0,0xff,0xe0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0xdf,0x00,0x00,0x07,0xff,0x07,0xff,0x07,0xdf,0x00,0x00,0x07,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0xff,0x07,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0xff,0x07,0xdf,0x00,0x00,0x00,0x00,0xff,0xe0,0xff,0xe0,0xff,0xe0,0xff,0xe0,0xff,0xe0,0xff,0xe0,0xff,0xe0,0xff,0xe0,0xff,0xe0,0x00,0x00,0xff,0xe0,0xff,0xe0,0x07,0xdf,0x07,0xdf,0x07,0xff,0x07,0xff,0x07,0xff,0x07,0xff,0x07,0xff,0x00,0x00
  ,0x00,0x00,0x00,0x00,0xff,0xe0,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xe0,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0xff,0x07,0xff,0x07,0xff,0x07,0xff,0x07,0xff,0x07,0xff,0x07,0xff,0x00,0x00,0x00,0x00,0xff,0xe0,0x07,0xff,0x07,0xff,0x07,0xff,0x07,0xff,0x07,0xff,0x07,0xff,0x07,0xff,0x07,0xff,0x07,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0xff,0x07,0xff,0x07,0xff,0x07,0xff,0x07,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x00,0x00,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xe0,0x00,0x00,0xff,0xe0,0xff,0xe0,0xff,0xe0,0xff,0xe0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xe0,0xff,0xe0,0xff,0xe0,0xff,0xe0,0xff,0xe0,0xff,0xe0,0xff,0xe0,0xff,0xe0,0xff,0xe0,0x00,0x00,0xff,0xe0,0xff,0xe0,0x07,0xff,0x07,0xff,0x07,0xff,0x07,0xff,0x07,0xff,0x07,0xff,0x07,0xff,0x00,0x00
  ,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xe0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0xdf,0x07,0xdf,0x07,0xff,0x07,0xff,0x07,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0xff,0x07,0xff,0x07,0xff,0x07,0xff,0x07,0xff,0x07,0xff,0x07,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x00,0x00,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x00,0x00,0xff,0xff,0x00,0x00,0x00,0x00,0xff,0xe0,0xff,0xe0,0xff,0xe0,0xff,0xe0,0xff,0xe0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x00,0x00,0x07,0xff,0x07,0xff,0x07,0xff,0x00,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0xff,0x00,0x00,0x00,0x00,0x07,0xff,0x07,0xdf,0x07,0xff,0x00,0x00,0x00,0x00,0x07,0xff,0x00,0x00,0x00,0x00,0xff,0xe0,0xff,0xe0,0xff,0xe0,0xff,0xe0,0xff,0xe0,0xff,0xe0,0xff,0xe0,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xe0,0xff,0xe0,0x07,0xff,0x07,0xff,0x07,0xff,0x07,0xff,0x07,0xff,0x00,0x00,0x00,0x00
  ,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0xff,0x07,0xff,0x07,0xff,0x00,0x00,0x00,0x00,0x00,0x20,0x00,0x00,0x00,0x20,0x00,0x00,0x00,0x00,0x07,0xff,0x07,0xdf,0x07,0xdf,0x07,0xff,0x07,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x00,0x00,0xff,0xff,0x00,0x00,0x00,0x00,0xff,0xe0,0xff,0xe0,0xff,0xe0,0xff,0xe0,0x00,0x00,0x00,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0xff,0x07,0xdf,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0xff,0x07,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xe0,0xff,0xe0,0xff,0xe0,0xff,0xe0,0xff,0xe0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xe0,0xff,0xe0,0x07,0xdf,0x07,0xff,0x07,0xff,0x00,0x00,0x00,0x00,0x00,0x00
  ,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xe0,0xff,0xe0,0xff,0xe0,0xff,0xe0,0xff,0xe0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

#ifdef ESP8266
// ISR for display refresh
void display_updater()
{
  display.display(display_draw_time);
}
#endif

#ifdef ESP32
void IRAM_ATTR display_updater(){
  // Increment the counter and set the time of ISR
  portENTER_CRITICAL_ISR(&timerMux);
  display.display(display_draw_time);
  portEXIT_CRITICAL_ISR(&timerMux);
}
#endif


void display_update_enable(bool is_enable)
{

#ifdef ESP8266
  if (is_enable)
    display_ticker.attach(0.002, display_updater);
  else
    display_ticker.detach();
#endif

#ifdef ESP32
  if (is_enable)
  {
    timer = timerBegin(0, 80, true);
    timerAttachInterrupt(timer, &display_updater, true);
    timerAlarmWrite(timer, 2000, true);
    timerAlarmEnable(timer);
  }
  else
  {
    timerDetachInterrupt(timer);
    timerAlarmDisable(timer);
  }
#endif


}

void pixel_time_test(uint8_t draw_time)
{

  Serial.print("Pixel draw latency in us: ");
  unsigned long start_timer=micros();
  display.drawPixel(1,1,0);
  unsigned long delta_timer=micros()-start_timer;
  Serial.println(delta_timer);

  Serial.print("Display update latency in us: ");
  start_timer=micros();
  display.display(draw_time);
  delta_timer=micros()-start_timer;
  Serial.println(delta_timer);


  display.setBrightness(0);
  display.clearDisplay();
  display.setTextColor(myCYAN);
  display.setCursor(2,0);
  display.print("Pixel");
  display.setTextColor(myMAGENTA);
  display.setCursor(2,8);
  display.print("Time");

  display_update_enable(true);
  for (uint8_t brightness=0; brightness<255; brightness++)
  {
    display.setBrightness(brightness);
    delay(10);
  }

  yield();
  delay(3000);

  display_update_enable(false);
}


void setup() {
 Serial.begin(9600);
  // Define your display layout here, e.g. 1/8 step
  display.begin(8);

  // Define your scan pattern here {LINE, ZIGZAG, ZAGGIZ} (default is LINE)
  //display.setScanPattern(LINE);

  // Define multiplex implemention here {BINARY, STRAIGHT} (default is BINARY)
  //display.setMuxPattern(BINARY);

  // Compare draw latency at similar display brightness for standard and fast drawing
  display.setFastUpdate(false);
  Serial.println("Draw test without fast update");
  display_draw_time=15;
  pixel_time_test(display_draw_time);
  Serial.println("Draw test with fast update");
  display.setFastUpdate(true);
  display_draw_time=1;
  pixel_time_test(display_draw_time);

  display.setFastUpdate(false);
  display_draw_time=15;
  display.clearDisplay();
  display_update_enable(true);
  //display.setColorOffset(15,15,0);
}
union single_double{
  uint8_t two[2];
  uint16_t one;
} this_single_double;

// This draws the weather icons
void draw_weather_icon (uint8_t icon)
{
  if (icon>10)
  icon=10;
  for (int yy=0; yy<10;yy++)
  {
    for (int xx=0; xx<10;xx++)
    {
      uint16_t byte_pos=(xx+icon*10)*2+yy*220;
      this_single_double.two[1]=weather_icons[byte_pos];
      this_single_double.two[0]=weather_icons[byte_pos+1];
      display.drawPixel(1+xx,yy,this_single_double.one);
    }
  }
}

unsigned long last_draw=0;
void scroll_text(uint8_t ypos, unsigned long scroll_delay, String text, uint8_t colorR, uint8_t colorG, uint8_t colorB)
{
    uint16_t text_length = text.length();
    display.setTextWrap(false);  // we don't wrap text so it scrolls nicely
    display.setTextSize(1);
    display.setRotation(0);
    display.setTextColor(display.color565(colorR,colorG,colorB));

    // Asuming 5 pixel average character width
    for (int xpos=matrix_width; xpos>-(matrix_width+text_length*5); xpos--)
    {
      display.setTextColor(display.color565(colorR,colorG,colorB));
      display.clearDisplay();
      display.setCursor(xpos,ypos);
      display.println(text);
      delay(scroll_delay);
      yield();

      // This might smooth the transition a bit if we go slow

      display.setTextColor(display.color565(colorR/4,colorG/4,colorB/4));
      display.setCursor(xpos-1,ypos);
      display.println(text);




      delay(scroll_delay/5);
      yield();




    }
}


uint8_t icon_index=0;
void loop() {
  scroll_text(1,50,"Welcome to PxMatrix!",96,96,250);
  display.clearDisplay();

  draw_weather_icon(icon_index);
  icon_index++;
  if (icon_index>10)
    icon_index=0;

  for (int xx=0; xx<16;xx++)
  {
    display.drawLine(xx+16,0,xx+16,5,display.color565(xx*16,0,0));
    display.drawLine(xx+16,6,xx+16,10,display.color565(0,xx*16,0));
    display.drawLine(xx+16,11,xx+16,15,display.color565(0,0,xx*16));
  }
  for (uint8_t dimm=255; dimm>0; dimm--)
  {
    display.setBrightness(dimm);
    delay(5);
  }
  for (uint8_t dimm=0; dimm<255; dimm++)
  {
    display.setBrightness(dimm);
    delay(5);
  }


}
