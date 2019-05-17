#include <Utility.hpp>
#include <EEPROM.h>

/*
 * function:    fatal
 * description: display fatal error and halt the system
 * input(s):    const char * message, the error message to display
 * output(s):   void
 * author:      Samuel Hild
 */
void fatal (const __FlashStringHelper * message)
{
  if (RADIO_ENABLE)
  {
    unsigned long time = millis();    // time for the messages

    // display error message
    Serial.print(time);
    Serial.print(F(" - [!!] FATAL: "));
    Serial.println(message);

    // display the serial output
    Serial.print(time);
    Serial.println(F(" - [!!] HALTING PROGRAM OPPERATION"));
  }

  // display fatal error on the led display
  digitalWrite(ERROR_LED, HIGH);

  for (;;);   // halt the system
}

/*
 * function:    error
 * description: display error to serial monitor and led
 * input(s):    const char * message, the error message to display
 * output(s):   void
 * author:      Samuel Hild
 */
void error (const __FlashStringHelper * message)
{
  // display error message
  if (RADIO_ENABLE)
  {
    Serial.print(millis());
    Serial.print(F(" - [!!] err: "));
    Serial.println(message);
  }
  // blink ERROR_LED twice
  blink(ERROR_LED);
  blink(ERROR_LED);
}

/*
 * function:    alert
 * description: displays an alert to the serial monitor 
 * input(s):    const char * message, the message to display
 * output(s):   void 
 * author:      Samuel Hild
 */
void alert(const __FlashStringHelper * message)
{
  // display alert message
  if (RADIO_ENABLE)
  {
    Serial.print(millis());
    Serial.print(F(" - [**] alert: "));
    Serial.println(message);
  }
}

/*
 * function:    debug
 * description: displays a debug message to the serial monitor 
 * input(s):    const char * message, the message to display
 * output(s):   void 
 * author:      Samuel Hild
 */
void debug(const __FlashStringHelper * message)
{
  // display debug message
  if (RADIO_ENABLE)
  {
    Serial.print(millis());
    Serial.print(F(" - [@@] debug: "));
    Serial.println(message);
  }
}

/*
 * function:    blink
 * description: blinks an led
 * input(s):    the led to blink
 * output(s):   void
 * author:      Samuel Hild
 */ 
void blink(int led)
{
  digitalWrite(led, HIGH);
  delay(100);
  digitalWrite(led, LOW);
  delay(100);
}


/*
 * function:    freeRam()
 * description: returns the amount of free ram
 *              Copied from arduino wiki
 * inputs:      void
 * outputs:     int ram, the amount of free ram
 * author:      Samuel Hild
 */
int freeRam()
{
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

/*
 * function:    clearEEPROM
 * description: clears all data from the EEPROM
 * input(s):    void
 * output(s):   void
 * author:      Samuel Hild
 */
void clearEEPROM()
{
  for (unsigned int i = 0; i < EEPROM.length(); i++)
    EEPROM.write(i, 0);
}

/*
 * function:    dataCopy
 * description: copy data from two data structures
 * input(s):    collectedData * to, the data structure to copy to
 *              collectedData * from, the data structure to copy from
 * output(s):   void
 * author:      Samuel Hild
 */
void dataCopy(collectedData * to, collectedData * from)
{
  to->timestamp = from->timestamp;
  to->number = from->number;

  to->altitude = from->altitude;
  to->pressure = from->pressure;
  to->tempurature = from->tempurature;

  to->velocity = from->velocity;
  to->acceleration = from->acceleration;

  to->ram = from->ram;
  to->mainVoltage = from->mainVoltage;
  to->drogueVoltage = from->drogueVoltage;

  to->lipoVoltage = from->lipoVoltage;
  to->lipoCurrent = from->lipoCurrent;
  to->lipoCapacity = from->lipoCapacity;
}