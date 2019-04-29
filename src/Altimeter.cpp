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
    return bmp->begin();
  }

  float readAltitude() override
  {
    return bmp->readAltitude(seaLevelPressure);
  }
  float readTemperature() override
  {
    return bmp->readTemperature();
  }
  float readPressure() override
  {
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
    debug ("Starting Simulated Altimeter");
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
    debug("free ram");
    Serial.println(freeRam());
  }

  Altimeter * newAltimeter = new BmpAltimeter();

  while (!newAltimeter->startup())
  {
    error("failed to start bmp altimeter");
    if(digitalRead(LOOP_STOP) == HIGH)
    {
      alert("interupt detected");

      delete newAltimeter;

      Altimeter * newAltimeter = new SimulatedAltimeter();
      newAltimeter->startup();

      blink(LOOP_LED);

      delay (500);

      break;
    }

    delay(STARTUP_DELAY);
  }

  delay (STARTUP_DELAY);
  debug("altimeter startup complete");

  if (RADIO_ENABLE)
  {
    Serial.print(newAltimeter->readPressure());
    Serial.println(" Pa");

    Serial.print(newAltimeter->readTemperature());
    Serial.println(" C");

    Serial.print(newAltimeter->readAltitude());
    Serial.println(" m");
  }
  return newAltimeter;
}
