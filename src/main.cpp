/*
 * src:		        main.cpp
 * description:	  main file for execution
 * Author:	      Samuel Hild
 */
#include <Utility.hpp>
#include <SparkFunBQ27441.h>

#include <Altimeter.hpp>
#include <Filter.hpp>
#include <Storage.hpp>


bool healthCheck1();
bool healthCheck2();

void setupGpio();
bool setupBattery();
bool setupAltimeter();
bool setupPrimaryStorage();
bool setupSecondaryStorage();
bool setupRadio();

void writeData();

Altimeter * altimeter;
Storage * primaryStorage;
Storage * secondaryStorage;
Runmode current;

bool programmingMode = false;

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
  setupGpio();

  if (!healthCheck1()) fatal("health check 1 failed");

  // setup battery
  while (!setupBattery())
  {
    error("failure to start battery");
    if (digitalRead(LOOP_STOP) == HIGH)
    {
      alert ("interupt detected");
      debug("programming mode active");

      blink(LOOP_LED);
      blink(LOOP_LED);

      programmingMode = true;

      delay(1000);
      break;
    }
    delay (STARTUP_DELAY);
  }

  // Setup Altimeter
  altimeter = buildAltimeter();

  setupPrimaryStorage();

  setupSecondaryStorage();

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
  writeData();
  digitalWrite(LOOP_LED, LOW);

  if (digitalRead(LOOP_STOP) == HIGH)
  {
    alert ("interupt detected");
    digitalWrite(LOOP_LED, LOW);

    primaryStorage->close();
    secondaryStorage->close();

    alert ("operations terminated");

    blink(LOOP_LED);
    blink(LOOP_LED);

    for (;;);
  }

  delay (REFRESH_RATE);
}

/*
 * function:	  setupGpio()
 * description:	sets up gpio display
 * input(s):	  void
 * output(s):	  void
 * author:	    Samuel Hild
 */
void setupGpio()
{
  pinMode(LOOP_STOP, INPUT);

  pinMode(ERROR_LED, OUTPUT);
  pinMode(LOOP_LED, OUTPUT);
  pinMode(MAIN_LED, OUTPUT);
  pinMode(DROGUE_LED, OUTPUT);

  pinMode(MAIN_DEPLOY, OUTPUT);
  pinMode(DROGUE_DEPLOY, OUTPUT);

  pinMode(SDCS, OUTPUT);
  pinMode(BMPCS, OUTPUT);

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

  if (RADIO_ENABLE)
  {
    debug("main read: ");
    Serial.println(mainRead);
  }

  if (mainRead < 500)
  {
    digitalWrite(MAIN_LED, HIGH);
    error("main charge discontinuous");
  }

  float drogueRead = analogRead(DROGUE_READ);

  if (RADIO_ENABLE)
  {
    debug("drogue read: ");
    Serial.println(drogueRead);
  }

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

    if (RADIO_ENABLE)
    {
      debug("dump initial battery info");
      Serial.print(lipo.voltage());
      Serial.println("mV");

      Serial.print(lipo.current(AVG));
      Serial.println("mA");

      Serial.print(lipo.capacity(REMAIN));
      Serial.println("mAh");
    }

    return true;
  }
  return false;
}


bool setupPrimaryStorage()
{
  // Setup Storage
  primaryStorage = buildStorage(1);
  delay (STARTUP_DELAY);

  if (!primaryStorage->isActive())
  {
    error("failure to start primary storage");
    return false;
  }

  return true;
}

bool setupSecondaryStorage()
{
  secondaryStorage = buildStorage(2);
  return true;
}

void writeData()
{
  String message =  (String(millis()) + ", ") + 
                    (String(freeRam()) + ", ") + 
                    (String(current) + ", ") + 
                    (String(altimeter->readAltitude()) + ", ") +
                    (String(altimeter->readPressure()) + ", ") + 
                    (String(altimeter->readTemperature()) + ", ");
  
  if (!programmingMode)
    message += (String(lipo.voltage()) + ", ")+(String(lipo.current(AVG)) + ", ") + (String(lipo.capacity(REMAIN)) + ", ");
  message += (String(analogRead(MAIN_READ)) + ", ") + (String(analogRead(DROGUE_READ)) + ", \n");

  if (primaryStorage->isActive())
    primaryStorage->write(message.c_str());
  else if (RADIO_ENABLE)
    Serial.print(message);

  uint32_t timestamp = millis();  
  uint32_t altitude = altimeter->readAltitude();
  uint16_t ram = freeRam();
  uint8_t voltage = lipo.voltage();
  uint8_t cont1 = analogRead(MAIN_READ) / 100;
  uint8_t cont2 = analogRead(DROGUE_READ) / 100;

  if (!RADIO_ENABLE)
    debug("DATA PACKET");

  Serial.print(timestamp);
  Serial.print(altitude);
  Serial.print(ram);
  Serial.print(voltage);
  Serial.print(cont1);
  Serial.print(cont2);

  if (!RADIO_ENABLE)
    Serial.println();

  return;
}