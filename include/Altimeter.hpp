/*
 * header: 	Altimeter.h
 * description:	The header file for Altimeter.c++, Includes interfaces, helper functions  and
 * 		subclasses
 * author: 	Samuel Hild
 */



/*
 * interface:	Altimeter
 * description:	an interface for all altimeter objects. By using subclasses, it becomes easier
 * 		to simulate a launch in testing.
 * author:	Samuel Hild
 */
class Altimeter
{
private:
protected:
  float initialAltitude;
  float maxAltitude;
  float seaLevelPressure;

public:
  virtual bool startup(){}
  virtual float readAltitude(){}
  virtual float readTempurature(){}
  virtual float readPressure(){}

  virtual float getMaxAltitude(){}
  virtual float getInitialAltitude(){}
};

/*
 * function: 	buildAltimeter()
 * description:	A factory function for creating an instance of Altimeter.
 * input(s):	int, altimeter type
 * output(s):	Altimeter, concreate altimeter instance
 * author:	Samuel Hild
 */
Altimeter * buildAltimeter(int altimeterType);
