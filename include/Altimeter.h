/*
 * header: 	Altimeter.h
 * description: 	The header file for Altimeter.c++, Includes interfaces, helper functions  and
 * 		subclasses
 * author: 	Samuel Hild
 */

#define STANDARD_PRESSURE 1013.25

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
  static double initialAltitude = 0;
  static double initialAltitude = 0;
  static double seaLevelPressure = STANDARD_PRESSURE;

public:
  virtual double readAltitude();
  virtual double readTempurature();
  virtual double readPressure();

  virtual double getMaxAltitude();
  virtual double getInitialAltitude();
};

/*
 * function: 	buildAltimeter()
 * description:	A factory function for creating an instance of Altimeter.
 * input(s):	int, altimeter type
 * output(s):	Altimeter, concreate altimeter instance
 * author:	Samuel Hild
 */
Altimeter buildAltimeter(int altimeterType);
