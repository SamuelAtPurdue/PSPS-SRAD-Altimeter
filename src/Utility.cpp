#include <Utility.hpp>

/*
 * function:    fatal
 * description: display fatal error and halt the system
 * input(s):    const char * message, the error message to display
 * output(s):   void
 * author:      Samuel Hild
 */
void fatal (const char * message)
{
  if (RADIO_ENABLE)
  {
    unsigned long time = millis();    // time for the messages

    // display error message
    Serial.print(time);
    Serial.print(" - [!!] FATAL: ");
    Serial.println(message);

    // display the serial output
    Serial.print(time);
    Serial.println(" - [!!] HALTING PROGRAM OPPERATION");
  }

  // display fatal error on the led display
  digitalWrite(ERROR_LED, HIGH);

  for (;;);   // halt the system

  return;
}

/*
 * function:    fatal
 * description: display fatal error and halt the system
 * input(s):    const char * message, the error message to display
 * output(s):   void
 * author:      Samuel Hild
 */
void error (const char * message)
{
  // display error message
  if (RADIO_ENABLE)
  {
    Serial.print(millis());
    Serial.print(" - [!!] err: ");
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
void alert(const char * message)
{
  // display alert message
  if (RADIO_ENABLE)
  {
    Serial.print(millis());
    Serial.print(" - [**] alert: ");
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
void debug(const char * message)
{
  // display debug message
  if (RADIO_ENABLE)
  {
    Serial.print(millis());
    Serial.print(" - [@@] debug: ");
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
