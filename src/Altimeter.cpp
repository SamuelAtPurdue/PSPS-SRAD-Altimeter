#include "../include/Altimeter.hpp"
#include <iostream>

#define ERRANT_PRSSURE 60373.94
#define STD_PRESSURE 1013.25

#define BMP_CS 10

using namespace std;

class BmpAltimeter : public Altimeter
{
private:
public:
  BmpAltimeter (float seaLevelPressure = STD_PRESSURE)
  {
    Altimeter::seaLevelPressure = seaLevelPressure;
    Altimeter::initialAltitude = 0;
    Altimeter::maxAltitude = 0;
  
    //TODO: BMP280 code
  }

  float readAltitude()
  {
    return 0;
  }
  float readTempurature()
  {
    return 0;
  }
  float readPressure()
  {
    return 0;
  }
  float getMaxAltitude()
  {
    return Altimeter::maxAltitude;
  }
  float getInitialAltitude()
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

  float readAltitude()
  {
    return 0;
  }
  float readTempurature()
  {
    return 0;
  }
  float readPressure()
  {
    return 0;
  }
  float getMaxAltitude()
  {
    return Altimeter::maxAltitude;
  }
  float getInitialAltitude()
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
      throw; 
  }

  return newAltimeter;
}
