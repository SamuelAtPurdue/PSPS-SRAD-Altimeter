/*
 * src:		        main.cpp
 * description:	  main file for execution
 * Author:	      Samuel Hild
 */
#include <Utility.hpp>
#include <SparkFunBQ27441.h>
#include <EEPROM.h>

#include <Altimeter.hpp>
#include <Storage.hpp>

bool healthCheck1();
bool healthCheck2();

void setupGpio();
bool setupBattery();
bool setupPrimaryStorage();
bool setupSecondaryStorage();
bool setupRadio();

void collectRaw(collectedData *);

void writeData(collectedData *);
void transmitTelemetry(collectedData *);
void writeDataEEPROM(collectedData *);

Altimeter * altimeter;
Storage * primaryStorage;
Storage * secondaryStorage;
Runmode current;

bool programmingMode = false;
collectedData previousData;
collectedData currentData;
collectedData predictedData;  //  Unused for now
uint32_t liftoffTime = 0;

/*
 * function:	     setup()
 * description:	   simulated setup() function
 * input(s):	     void
 * output(s):	     void
 * author:	       Samuel Hild
 */
void setup()
{
  //Initial Setup
  setupGpio();
  Serial.begin(9600);

  if (!healthCheck1()) 
    fatal (F("health check 1 failed"));

  // setup battery
  while (!setupBattery())
  {
    error (F("failure to start battery"));
    if (digitalRead(LOOP_STOP) == HIGH)
    {
      alert (F("interupt detected"));
      debug (F("programming mode active"));

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

  // make initial reading
  collectRaw(&previousData);
  collectRaw(&currentData);

  // Set Default Values
  previousData.velocity = 0;
  previousData.acceleration = 0;

  currentData.velocity = 0;
  currentData.acceleration = 0;

  current = Runmode::PREFLIGHT;
}

/*
 * function:	  loop()
 * description: main loop for all inflight actions
 * input(s):	  void
 * output(s):	  void
 * author:	    Samuel Hild
 */
void loop()
{
  digitalWrite(LOOP_LED, HIGH);
  
  debug(F("loop begin"));

  collectRaw(&currentData);
  writeData(&currentData);
  transmitTelemetry(&currentData);
  
  switch (current)
  {
    case Runmode::PREFLIGHT :
      if ((currentData.altitude >= (altimeter->getInitialAltitude() + LAUNCH_ALTITUDE_THRESHOLD)) || 
        (currentData.velocity >= LAUNCH_VELOCITY_THRESHOLD) || 
        (currentData.acceleration >= LAUNCH_ACCELERATION_THRESHOLD))
        {
          liftoffTime = currentData.timestamp;
          current = Runmode::BURN;  
          break;    // Skips over the delay
        }

      delay (REFRESH_RATE*2);
      break;
    
    case Runmode::BURN :
      if (currentData.acceleration <= 0 || currentData.timestamp > (liftoffTime + BURN_TIME_THRESHOLD))
      {
        current = Runmode::ASCENT;
      }

      //TODO: Add FRAM Loop
      break;

    case Runmode::ASCENT :
      if (currentData.velocity <= APOGEE_VELOCITY_THRESHOLD)
      {
        current = Runmode::APOGEE;
      }
      // TODO: Add FRAM loop
      break;

    case Runmode::APOGEE :
      if (currentData.velocity <= 0)
      {
        // Deploy DROUGE
        digitalWrite(DROGUE_DEPLOY, HIGH);
        delay(DEPLOY_DELAY);
        digitalWrite(DROGUE_DEPLOY, LOW);

        if (analogRead(DROGUE_READ) > 500) 
        {
          error(F("deploy error detected drogue"));
          digitalWrite(DROGUE_DEPLOY, HIGH);
        }
        
        current = Runmode::DROGUE;
      }

      //TODO: FRAM
      
      break;

    case Runmode::DROGUE :
      if (currentData.altitude <= (MAIN_ALTITUDE_THRESHOLD + altimeter->getInitialAltitude()))
      {
        digitalWrite(DROGUE_LED, LOW);
        digitalWrite(MAIN_DEPLOY, HIGH);
        delay(DEPLOY_DELAY);
        digitalWrite(MAIN_DEPLOY, LOW);

        if (analogRead(MAIN_READ) > 500)
        {
          error(F("deploy error detected main"));
          digitalWrite(MAIN_DEPLOY, HIGH);
        }

        current = Runmode::MAIN;
      }

      //TODO: FRAM

      break;

    case Runmode::MAIN :
      if ((currentData.velocity >= ZERO_THRESHOLD) || currentData.altitude <= altimeter->getInitialAltitude())
        current = Runmode::POSTFLIGHT;

      break;

    case Runmode::POSTFLIGHT :

      // Safety Checks
      digitalWrite(DROGUE_DEPLOY, LOW);
      digitalWrite(MAIN_DEPLOY, LOW);

      digitalWrite(DROGUE_LED, (analogRead(DROGUE_READ) < 500) ? HIGH : LOW);
      digitalWrite(MAIN_LED, (analogRead(MAIN_READ) < 500) ? HIGH : LOW);

      // final Reading
      collectRaw(&currentData);
      writeData(&currentData);
      transmitTelemetry(&currentData);

      // TODO: final fram reading

      alert (F("end of flight detected"));
      digitalWrite(LOOP_LED, LOW);

      primaryStorage->close();

      alert (F("operations terminated"));

      blink(LOOP_LED);
      blink(LOOP_LED);

      for (;;);

    default:
      fatal(F("unknown runmode"));
      break;
  }

  // Interupt Detection
  if (digitalRead(LOOP_STOP) == HIGH)
  {
    alert (F("interupt detected"));
    digitalWrite(LOOP_LED, LOW);

    primaryStorage->close();

    alert (F("operations terminated"));

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

  debug(F("begin startup"));

  debug(F("begin EEPROM dump"));

  for (unsigned int i = 0; i < EEPROM.length(); i++)
  {
    Serial.print(F("0x"));
    Serial.print(EEPROM.read(i), HEX);
    Serial.print(F(" "));
  }
  Serial.println();

  // Check Leds
  for (int i = 0; i < 4; i++)
  {
    blink(ERROR_LED + i);
  }

  float mainRead = analogRead(MAIN_READ);

  if (RADIO_ENABLE)
  {
    debug(F("main read: "));
    Serial.println(mainRead);
  }

  if (mainRead < 500)
  {
    digitalWrite(MAIN_LED, HIGH);
    error(F("main charge discontinuous"));
  }

  float drogueRead = analogRead(DROGUE_READ);

  if (RADIO_ENABLE)
  {
    debug(F("drogue read: "));
    Serial.println(drogueRead);
  }

  if (drogueRead < 500)
  {
    digitalWrite(DROGUE_LED, HIGH);
    error(F("drogue charge discontinuous"));
  }

  debug(F("Health Check 1 complete"));

  return true;
}

bool setupBattery()
{
  if(lipo.begin())
  {
    lipo.setCapacity(BATTERY_CAPACITY);

    debug(F("battery startup successful"));

    if (RADIO_ENABLE)
    {
      debug(F("dump initial battery info"));
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

  return primaryStorage->isActive();
}

bool setupSecondaryStorage()
{
  secondaryStorage = buildStorage();
  return secondaryStorage->open();
}

/*
 * function:    collectRaw
 * description: Collects raw data from the sensors
 * input(s):    collectedData * data, the struct to store the data
 * output(s):   void
 * author:      Samuel Hild
 */
void collectRaw(collectedData * data)
{
  debug(F("start collecting data"));

  data->timestamp = millis();
  (data->number)++;

  if (RADIO_ENABLE) Serial.println(data->number);

  data->altitude = altimeter->readAltitude();
  data->pressure = altimeter->readPressure();
  data->tempurature = altimeter->readTemperature();

  data->ram = freeRam();
  data->mainVoltage = analogRead(MAIN_READ);
  data->drogueVoltage = analogRead(DROGUE_READ);

  data->lipoVoltage = (programmingMode) ? 3.6 : lipo.voltage();
  data->lipoCurrent = (programmingMode) ? 0.040 : lipo.current(AVG);
  data->lipoCapacity = (programmingMode) ? 1900 : lipo.capacity(REMAIN);

  return;
}

void writeData(collectedData * data)
{

  debug(F("writing data to sd card"));

  writeDataEEPROM(data);

  const char * message = dataToCStr(data);

  if (primaryStorage->isActive())
    primaryStorage->write(message);
    
  else if (RADIO_ENABLE)
    Serial.print(message);

  debug(F("writing data to sd card"));

  return;
}

/*
 * function:    transmitTelemetry
 * description: Transmits data over the radio
 * input(s):    collectedData * data, the data to transmit
 * output(s):   void
 * author:      Samuel Hild
 */
void transmitTelemetry(collectedData * data)
{
  if (RADIO_ENABLE)
    debug(F("DATA PACKET: "));

  Serial.print(data->timestamp);
  Serial.print(",");
  Serial.print((uint32_t) data->pressure);
  Serial.print(",");
  Serial.print((uint16_t) data->ram);
  Serial.print(",");
  Serial.print((uint8_t) data->lipoVoltage);
  Serial.print(",");
  Serial.print((uint8_t) (data->mainVoltage / 100));
  Serial.print(",");
  Serial.print((uint8_t) (data->drogueVoltage / 100));

  Serial.println();

  if (RADIO_ENABLE)
    Serial.print("\n\n");
  
  return;
}

/*
 * function:    healthCheck2
 * description: checks the status of the device after setup
 * input(s):    void
 * output(s):   bool success, shows whether the device is operational
 * author:      Samuel Hild
 */
bool healthCheck2()
{
  // Check continuity
  debug(F("verify drogue"));
  digitalWrite(DROGUE_LED, (analogRead(DROGUE_READ) < 500) ? HIGH : LOW);

  debug(F("verify main"));
  digitalWrite(MAIN_LED, (analogRead(MAIN_READ) < 500) ? HIGH : LOW);

  // take test Reading
  collectRaw(&currentData);

  if (RADIO_ENABLE)
  {
    debug(F("dump initial data"));

    Serial.println(currentData.timestamp);
    Serial.println(currentData.number);

    Serial.println(currentData.altitude);
    Serial.println(currentData.pressure);
    Serial.println(currentData.tempurature);

    Serial.println(currentData.ram);
    Serial.println(currentData.mainVoltage);
    Serial.println(currentData.drogueVoltage);

    Serial.println(currentData.lipoVoltage);
    Serial.println(currentData.lipoCurrent);
    Serial.println(currentData.lipoCapacity);
  }  

  if (currentData.pressure < 0 || currentData.pressure == ERRANT_PRESSUE)
    error(F("pressure error"));

  return true;
}

void writeDataEEPROM(collectedData * data)
{
  debug(F("storing in EEPROM"));

  EEPROM.put(0, altimeter->getMaxAltitude());
  EEPROM.put(sizeof(float), * data);

  debug(F("end collecting data"));
}