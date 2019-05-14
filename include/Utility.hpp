#include <Arduino.h>
/*
 * header:      Utility.hpp
 * description: provides utility functions to be used by other classes
 * author:      Samuel Hild
 */

// Pinout Values
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

// Refresh rates and Delays
#define REFRESH_RATE 5
#define STARTUP_DELAY 15
#define STARTUP_CONST 1
#define DEPLOY_DELAY 500

// Sensor Settings
#define BATTERY_CAPACITY 500
#define PACKET_SIZE 104
#define ERRANT_PRESSUE -1

// Threshold Values
#define LAUNCH_ALTITUDE_THRESHOLD 100
#define LAUNCH_VELOCITY_THRESHOLD 10
#define LAUNCH_ACCELERATION_THRESHOLD 50

#define BURN_TIME_THRESHOLD 15
#define APOGEE_VELOCITY_THRESHOLD 5
#define MAIN_ALTITUDE_THRESHOLD 1000
#define ZERO_THRESHOLD 0

// Debug
#define RADIO_ENABLE 1


/*
 * enum:        Runmode
 * description: provides the runmode for performing specified actions and flight checks
 * author:      Samuel Hild
 */
enum Runmode {PREFLIGHT, BURN, ASCENT, APOGEE, DROGUE, MAIN, POSTFLIGHT};

/*
 * struct:      collectedData
 * description: provides a data structure for the data collected each loop
 * author:      Samuel Hild
 */
struct collectedData 
{
  // collection info  
  uint32_t timestamp;
  uint32_t number;

  // Sensor Data
  float altitude;
  float pressure;
  float tempurature;

  // Derived Data
  float velocity;
  float acceleration;

  // System Information
  uint16_t ram;
  uint16_t mainVoltage;
  uint16_t drogueVoltage;

  // Power System Information
  float lipoVoltage;
  float lipoCurrent;
  float lipoCapacity;
};

void fatal (const __FlashStringHelper *);
void error (const __FlashStringHelper *);
void alert (const __FlashStringHelper *);
void debug (const __FlashStringHelper *);

void blink(int);

int freeRam();
const char * dataToCStr(collectedData *);

void clearEEPROM();
