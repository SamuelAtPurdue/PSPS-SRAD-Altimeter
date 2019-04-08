#include <Arduino.h>
/*
 * header:      Utility.hpp
 * description: provides utility functions to be used by other classes
 * author:      Samuel Hild
 */
#define SDCS 4
#define FRAMCS 9
#define BMPCS 10
#define MAIN_DEPLOY A1
#define MAIN_READ A0
#define DROGUE_DEPLOY A3
#define DROGUE_READ A2
#define BUZZER 3
#define LOOP_STOP 2
#define ERROR_LED 5
#define LOOP_LED 6
#define MAIN_LED 7
#define DROGUE_LED 8

#define RADIO_INTERFACE_RX 0
#define RADIO_INTERFACE_TX 1

#define REFRESH_RATE 5
#define STARTUP_DELAY 15
#define STARTUP_CONST 10
#define BATTERY_CAPACITY 500

/*
 * enum:        Runmode
 * description: provides the runmode for performing specified actions and flight checks
 * author:      Samuel Hild
 */
enum Runmode {PREFLIGHT, BURN, ASCENT, APOGEE, DROGUE, MAIN, POSTFLIGHT};

void fatal (String);
void error (String);
void alert (String);
void debug (String);

void blink(int);

int freeRam();
