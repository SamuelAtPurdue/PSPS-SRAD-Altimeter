/*
 * header:	Storage.h
 * description:	header file providing Storage classes, helper functions and interfaces.
 * author:	Samuel Hild
 */

#include <Arduino.h>

/*
 * interface:	  Storage
 * description:	interface for all the Storage classes
 * Author: 	    Samuel Hild
 */
class Storage{
private:
protected:
public:
  virtual bool open(){}
  virtual void write(String){}

  virtual void close(){}
  virtual bool isActive(){}
};

/*
 * function:	  buildStorage()
 * description: Factory function for creating an instance of Storage
 * input(s):	  int selection, the type of Storage to create
 * output(s):	  Storage, new instance of Storage
 * Author: 	    Samuel Hild
 */
Storage buildStorage(int selection);
