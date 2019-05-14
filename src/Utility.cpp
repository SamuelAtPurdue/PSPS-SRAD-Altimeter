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

  return;
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

  return;
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

  return;
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
  return;
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

  return;
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
 * function:    dataToCStr
 * description: Converts the data struct to a c string
 *              I like to think this function doesn't exist
 * input(s):    collectedData * data, the data to convert to string
 * output(s):   const char * message, the data converted to a string
 * author:      Samuel Hild
 */
const char * dataToCStr(collectedData * data)
{
  // Given more time I would perfer not to use arduino strings for this task
  // but I decided the potential performance risks were worth the time saved
  // by using them.
  String temp = (String(data->timestamp) + ",") + 
                (String(data->altitude) + ",") + 
                (String(data->pressure) + ",") + 
                (String(data->tempurature) + ",") +
                (String(data->ram) + ",") +
                (String(data->mainVoltage) + ",") +
                (String(data->drogueVoltage) + ",") +
                (String(data->lipoVoltage) + ",") +
                (String(data->lipoCurrent) + ",") +
                (String(data->lipoCapacity) + "\n");

  const char * message = temp.c_str();

  return message;
}

void clearEEPROM()
{
  for (unsigned int i = 0; i < EEPROM.length(); i++)
  {
    EEPROM.write(i, 0);
  }
}