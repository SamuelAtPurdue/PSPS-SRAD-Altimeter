#include <Altimeter.hpp>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <SPI.h>

#define ERRANT_PRSSURE 60373.94
#define STD_PRESSURE 1013.25

#define BMP_CS 10

using namespace std;

class BmpAltimeter : public Altimeter
{
private:
  Adafruit_BMP280 * bmp = new Adafruit_BMP280(BMP_CS);
public:
  BmpAltimeter (float seaLevelPressure = STD_PRESSURE)
  {
    Altimeter::seaLevelPressure = seaLevelPressure;
    Altimeter::initialAltitude = 0;
    Altimeter::maxAltitude = 0;
  }

  bool startup() override
  {
    if (!bmp->begin())
    {
      Serial.println(bmp->readPressure());
      Serial.println(bmp->readTemperature());
      Serial.println(bmp->readAltitude(STD_PRESSURE));
      return true;
    }
    return false;
  }

  float readAltitude() override
  {
    return 0;
  }
  float readTempurature() override
  {
    return 0;
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

  float readAltitude() override
  {
    return 0;
  }
  float readTempurature() override
  {
    return 0;
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

Altimeter * buildAltimeter(int altimeterType)
{
  Altimeter * newAltimeter;

  switch (altimeterType)
  {
    case 0:
      newAltimeter = new BmpAltimeter();
    case 1:
      newAltimeter = new SimulatedAltimeter();
    default:
      ;// throw;
  }

  return newAltimeter;
}
