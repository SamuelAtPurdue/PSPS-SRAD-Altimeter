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
  virtual bool open() = 0;
  virtual void write(const char *) = 0;

  virtual void close() = 0;
  virtual bool isActive() = 0;

  virtual ~Storage() {} 
};

/*
 * function:	  buildStorage()
 * description: Factory function for creating an instance of Storage
 * input(s):	  int selection, the type of Storage to create
 * output(s):	  Storage, new instance of Storage
 * Author: 	    Samuel Hild
 */
Storage * buildStorage(int selection);
