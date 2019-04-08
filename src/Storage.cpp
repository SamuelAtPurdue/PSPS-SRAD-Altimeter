/*
 * src:	        Storage.cpp
 * description:	Source file for all storage related functionality
 * author:	    Samuel Hild
 */

#include <Storage.hpp>
#include <SPI.h>
#include <SD.h>
#include <Utility.hpp>
#include <Adafruit_FRAM_SPI.h>

#define FILENAME "data.csv"

/*
 * interface:	  SdStorage
 * description: class for SD card storage
 * Author: 	    Samuel Hild
 */
class SdStorage : public Storage
{
private:
  File datafile;
  bool active;

protected:
public:
  SdStorage(){}

  bool open() override
  {
    bool success = SD.begin(SDCS);

    if (success)
      datafile = SD.open(FILENAME, FILE_WRITE);

    active = success && datafile;

    return active;
  }

  void write(String data) override
  {
    datafile.print(data);
  }

  void close() override
  {
    datafile.close();
  }
  bool isActive() override
  {
    return active;
  }
};

/*
 * class:       FramStorage
 * description: class for FRAM Storage
 * author:      Samuel Hild
 */
class FramStorage : public Storage
{
private:
  Adafruit_FRAM_SPI fram = Adafruit_FRAM_SPI(FRAMCS);
protected:
public:
  FramStorage() {}

  bool open()
  {
    return fram.begin();
  }
  void write(String data)
  {
    char * cdata = data.c_str();


  }
};

/*
 * function:	  buildStorage()
 * description:	Factory function for creating an instance of Storage
 * input(s):	  int, selection, the type of Storage to create
 * output(s):	  Storage, new instance of Storage
 * Author: 	    Samuel Hild
 */
Storage buildStorage(int selection)
{
  switch (selection)
  {
    case 1:
      Storage * primaryStorage = new SdStorage();

      if (primaryStorage->open())
        Serial.println("sd startup successful");
      else
        error("failed to start sd storage");

      return * primaryStorage;

    case 2:
      return *(new Storage());
    default:
      fatal("unknown storage device");
  }
}
