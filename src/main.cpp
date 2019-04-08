/*
 * src:		        main.cpp
 * description:	  main file for execution
 * Author:	      Samuel Hild
 */
#include <Arduino.h>
#include <SparkFunBQ27441.h>

#include <Altimeter.hpp>
#include <Filter.hpp>
#include <Storage.hpp>
#include <Utility.hpp>

using namespace std;

void gpioSetup();
bool healthCheck1();
bool setupBattery();
bool setupRadio();

Altimeter altimeter;
Storage primaryStorage;
Storage secondaryStorage;
Runmode current;
bool programmingMode;


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

  // setup battery
  programmingMode = false;
  while (!setupBattery())
  {
    error("failure to start battery");
    if (digitalRead(LOOP_STOP) == HIGH)
    {
      Serial.println("[**] alert: interupt detected");
      debug("programming mode active");

      programmingMode = true;

      blink(LOOP_LED);
      blink(LOOP_LED);

      delay(1000);
      break;
    }
    delay (STARTUP_DELAY);
  }

  // Setup Altimeter
  altimeter = buildAltimeter();

  // Setup Storage
  int i = 0;
  do
  {
    primaryStorage = buildStorage(1);
    delay (STARTUP_DELAY);
  }while(primaryStorage.isActive() && i++ < STARTUP_CONST);

  if (i >= STARTUP_CONST)
    error("failure to start primary storage");

  secondaryStorage = buildStorage(2);


  digitalWrite(LOOP_LED, HIGH);
  current = PREFLIGHT;
}

/*
 * function:	loop()
 * description:	main loop for all inflight actions
 * input(s):	void
 * output(s):	void
 * author:	Samuel Hild
 */
void loop()
{
  digitalWrite(LOOP_LED, HIGH);


  if (digitalRead(LOOP_STOP) == HIGH)
  {
    Serial.println("[**] alert: interupt detected");
    digitalWrite(LOOP_LED, LOW);

    primaryStorage.close();
    secondaryStorage.close();

    Serial.println("[**] alert: operations terminated");

    blink(LOOP_LED);
    blink(LOOP_LED);

    for (;;);
  }

  delay (REFRESH_RATE);
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

  return;
}

/*
 * function:    healthCheck1()
 * description: Performs an initial health check of the system
 *
 */
bool healthCheck1()
{
  //Check continuity
  debug("begin startup");

  // Check Leds
  for (int i = 0; i < 4; i++)
  {
    blink(ERROR_LED + i);
  }

  float mainRead = analogRead(MAIN_READ);

  debug("main read: ");
  Serial.println(mainRead);

  if (mainRead < 500)
  {
    digitalWrite(MAIN_LED, HIGH);
    error("main charge discontinuous");
  }

  float drogueRead = analogRead(DROGUE_READ);

  debug("drogue read: ");
  Serial.println(drogueRead);

  if (drogueRead < 500)
  {
    digitalWrite(DROGUE_LED, HIGH);
    error("drogue charge discontinuous");
  }

  debug("Health Check 1 complete");

  return true;
}

bool setupBattery()
{
  if(lipo.begin())
  {
    lipo.setCapacity(BATTERY_CAPACITY);

    debug("battery startup successful");

    debug("dump initial battery info");
    Serial.print(lipo.voltage());
    Serial.println("mV");

    Serial.print(lipo.current(AVG));
    Serial.println("mA");

    Serial.print(lipo.capacity(REMAIN));
    Serial.println("mAh");

    return true;
  }
  else
    return false;

}
