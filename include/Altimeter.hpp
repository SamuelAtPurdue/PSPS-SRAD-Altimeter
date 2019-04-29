/*
 * header: 	    Altimeter.hpp
 * description: The header file for Altimeter.cpp, Includes interfaces, helper functions  and
 * 		          subclasses
 * author: 	    Samuel Hild
 */



/*
 * interface:	  Altimeter
 * description:	an interface for all altimeter objects. By using subclasses, it becomes easier
 * 		          to simulate a launch in testing.
 * author:	    Samuel Hild
 */
class Altimeter
{
private:

protected:
  float initialAltitude;
  float maxAltitude;
  float seaLevelPressure;

public:
  virtual bool startup() = 0;
  virtual float readAltitude() = 0;
  virtual float readTemperature() = 0;
  virtual float readPressure() = 0;

  virtual float getMaxAltitude() = 0;
  virtual float getInitialAltitude() = 0;
};

/*
 * function: 	buildAltimeter()
 * description:	A factory function for creating an instance of Altimeter.
 * input(s):	void
 * output(s):	Altimeter, concreate altimeter instance
 * author:	Samuel Hild
 */
Altimeter * buildAltimeter();
