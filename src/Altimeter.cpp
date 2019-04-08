#include <Altimeter.hpp>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <SPI.h>
#include <Utility.hpp>

#define ERRANT_PRSSURE 60373.94
#define STD_PRESSURE 1013.25

using namespace std;

class BmpAltimeter : public Altimeter
{
private:
  Adafruit_BMP280 * bmp;

public:
  BmpAltimeter (float seaLevelPressure = STD_PRESSURE)
  {
    Altimeter::seaLevelPressure = seaLevelPressure;
    Altimeter::initialAltitude = 0;
    Altimeter::maxAltitude = 0;

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
    return 0;
  }
  float readTemperature() override
  {
    return 1;
  }
  float readPressure() override
  {
    return 2;
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

Altimeter buildAltimeter()
{
  debug("free ram");
  Serial.println(freeRam());

  Altimeter * newAltimeter = new BmpAltimeter();

  while (!newAltimeter->startup())
  {
    error("failed to start bmp altimeter");
    if(digitalRead(LOOP_STOP) == HIGH)
    {
      Serial.println("[**] alert: interupt detected");
      newAltimeter = new SimulatedAltimeter();
      newAltimeter->startup();

      blink(LOOP_LED);

      delay (1000);

      break;
    }
    delay(STARTUP_DELAY);
  }

  delay (STARTUP_DELAY);
  debug("altimeter startup complete");

  Serial.println(String (newAltimeter->readPressure()) + " Pa");
  Serial.println(String (newAltimeter->readTemperature()) + " C");
  Serial.println(String (newAltimeter->readAltitude()) + " m");

  return *newAltimeter;
}
