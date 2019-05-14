#include <Utility.hpp>
#include <Altimeter.hpp>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <SPI.h>

#define ERRANT_PRSSURE 60373.94
#define STD_PRESSURE 1013.25

class BmpAltimeter : public Altimeter
{
private:
  Adafruit_BMP280 * bmp;
  float previousReading;

public:
  BmpAltimeter (float seaLevelPressure = STD_PRESSURE)
  {
    // set initial values
    Altimeter::seaLevelPressure = seaLevelPressure;
    Altimeter::initialAltitude = 0;
    Altimeter::maxAltitude = 0;

    // create new instance of Adafruit_BMP280
    bmp = new Adafruit_BMP280(BMPCS);
  }

  bool startup() override
  {
    bool success = bmp->begin();
    if (success)
      initialAltitude = readAltitude();

    return success;
  }

  float readAltitude() override
  {
    // Flush Previous Reading
    bmp->readAltitude(seaLevelPressure);

    // Assign the maximum reached altitude
    previousReading = bmp->readAltitude(seaLevelPressure);
    if (previousReading > Altimeter::maxAltitude)
      Altimeter::maxAltitude = previousReading;

    return previousReading;
  }
  float readTemperature() override
  {
    bmp->readTemperature();

    return bmp->readTemperature();
  }
  float readPressure() override
  {
    bmp->readPressure();

    return bmp->readPressure();
  }
  float getMaxAltitude() override
  {
    return Altimeter::maxAltitude;
  }
  float getInitialAltitude() override
  {
    return Altimeter::initialAltitude;
  }
};

/*
 * class:       SimulatedAltimeter
 * description: A concreate type of Altimeter for simulated flight
 * author:      Samuel Hild
 */
class SimulatedAltimeter : public Altimeter
{
private:
public:
  SimulatedAltimeter (float seaLevelPressure = STD_PRESSURE)
  {
    Altimeter::seaLevelPressure = seaLevelPressure;
    Altimeter::initialAltitude = 0;
    Altimeter::maxAltitude = 0;
  }

  bool startup()
  {
    debug (F("Starting Simulated Altimeter"));
    return true;
  }

  float readAltitude() override
  {
    return 15;
  }
  float readTemperature() override
  {
    return 16;
  }
  float readPressure() override
  {
    return 17;
  }
  float getMaxAltitude() override
  {
    return Altimeter::maxAltitude;
  }
  float getInitialAltitude() override
  {
    return Altimeter::maxAltitude;
  }
};

/*
 * function:    buildAltimeter
 * description: builds a new Altimeter object 
 * input(s):    void
 * outputs(s):  new Altimeter object
 * author:      Samuel Hild
 */
Altimeter * buildAltimeter()
{
  if (RADIO_ENABLE)
  {
    debug(F("free ram"));
    Serial.println(freeRam());
  }

  Altimeter * newAltimeter = new BmpAltimeter();

  while (!newAltimeter->startup())
  {
    error(F("failed to start bmp altimeter"));
    if(digitalRead(LOOP_STOP) == HIGH)
    {
      alert(F("interupt detected"));


      newAltimeter = new SimulatedAltimeter();
      newAltimeter->startup();

      blink(LOOP_LED);

      delay (500);

      break;
    }
    debug(F("free ram:"));

    if (RADIO_ENABLE)
      Serial.println(freeRam());

    delay(STARTUP_DELAY);
  }

  delay (STARTUP_DELAY);
  debug(F("altimeter startup complete"));

  if (RADIO_ENABLE)
  {
    Serial.print(newAltimeter->readPressure());
    Serial.println(F(" Pa"));

    Serial.print(newAltimeter->readTemperature());
    Serial.println(F(" C"));

    Serial.print(newAltimeter->readAltitude());
    Serial.println(F(" m"));
  }
  return newAltimeter;
}
