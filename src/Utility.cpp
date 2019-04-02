#include <Utility.hpp>
#include <Arduino.h>

#define ERROR_LED 5

void fatal(const char * message)
{
  Serial.print("[!!] FATAL:");
  Serial.println(message);
  Serial.println("[!!] HALTING PROGRAM OPPERATION");

  digitalWrite(ERROR_LED, HIGH);
  //TODO 2 LONG BUZZER

  for (;;);

  return;
}

void error (const char * message)
{
  Serial.print("[!!] err:");
  Serial.println(message);

  digitalWrite(ERROR_LED, HIGH);
  delay (300);
  digitalWrite(ERROR_LED, LOW);
  delay(300);

  digitalWrite(ERROR_LED, HIGH);
  delay (300);
  digitalWrite(ERROR_LED, LOW);
  delay(300);

  return;
}
