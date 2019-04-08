#include <Utility.hpp>
#include <Arduino.h>

#define ERROR_LED 5

void fatal(String message)
{
  Serial.println(String(millis()) + " - [!!] FATAL: " + message);
  Serial.println(String(millis()) + " - [!!] HALTING PROGRAM OPPERATION");

  digitalWrite(ERROR_LED, HIGH);

  for (;;);

  return;
}

void error (String message)
{
  Serial.print(String(millis()));
  Serial.println(" - [!!] err: " + message);

  blink(ERROR_LED);
  blink(ERROR_LED);

  return;
}

void blink(int led)
{
  digitalWrite(led, HIGH);
  delay(150);
  digitalWrite(led, LOW);
  delay(150);

  return;
}

void debug(String message)
{
  Serial.println(String(millis()) + " - [@@] debug: " + message);
  delay(STARTUP_DELAY);
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
