/*
 * src:		        main.cpp
 * description:	  main file for execution
 * Author:	      Samuel Hild
 */
#include <Arduino.h>
#include <Altimeter.hpp>
#include <Filter.hpp>
#include <Storage.hpp>
#include <Utility.hpp>

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

using namespace std;

void gpioSetup();
int healthCheck1();
void setupBattery();
Altimeter altimeterStartup();


/*
 * function:	     setup()
 * description:	   simulated setup() function
 * input(s):	     void
 * output(s):	     void
 * author:	       Samuel Hild
 */
void setup()
{
  Serial.begin(9600);
  gpioSetup();

  if (!healthCheck1())
    fatal("health check 1 failed");

  Altimeter altimeter = altimeterStartup();

}

/*
 * function:	loop()
 * description:	simulated loop() function
 * input(s):	void
 * output(s):	void
 * author:	Samuel Hild
 */
void loop()
{


}

/*
 * function:	  gpioSetup()
 * description:	sets up gpio display
 * input(s):	  void
 * output(s):	  void
 * author:	    Samuel Hild
 */
void gpioSetup()
{
  pinMode(LOOP_STOP, INPUT);

  pinMode(ERROR_LED, OUTPUT);
  pinMode(LOOP_LED, OUTPUT);
  pinMode(MAIN_LED, OUTPUT);
  pinMode(DROGUE_LED, OUTPUT);

  pinMode(MAIN_DEPLOY, OUTPUT);
  pinMode(DROGUE_DEPLOY, OUTPUT);
}

/*
 * function:    healthCheck1()
 * description: Performs an initial health check of the system
 *
 */
int healthCheck1()
{
  //Check continuity
  Serial.print("Begin startup at time: ");
  Serial.println(millis());

  // Check Leds
  for (int i = 0; i < 4; i++)
  {
    digitalWrite(ERROR_LED+i, HIGH);
    delay(300);
    digitalWrite(ERROR_LED+i, LOW);
    delay(300);
  }

  float mainRead = analogRead(MAIN_READ);

  Serial.print("Main Read: ");
  Serial.println(mainRead);

  if (mainRead < 200)
  {
    digitalWrite(MAIN_LED, HIGH);
    error("main charge discontinuous");
  }

  float drogueRead = analogRead(DROGUE_READ);

  Serial.print("Drogue Read: ");
  Serial.println(drogueRead);

  if (drogueRead < 200)
  {
    digitalWrite(DROGUE_LED, HIGH);
    error("drogue charge discontinuous");
  }

  Serial.println("Health Check 1 complete");

  return 1;
}

void batteryStartup()
{
  return;
}

Altimeter altimeterStartup()
{

}
