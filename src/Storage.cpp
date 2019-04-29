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

#define FILENAME "data.txt"

/*
 * interface:	  SdStorage
 * description: class for SD card storage
 * Author: 	    Samuel Hild
 */
class SdStorage : public Storage
{
private:
  File datafile;
  bool active = false;

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

  void write(const char * data) override
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
  ~SdStorage() {}
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
  uint32_t previousAddress = 0x01;
  bool active = false;

protected:
public:
  FramStorage() {}

  bool open()
  {
    return active = fram.begin();
  }
  void write(const char * data)
  {

    fram.writeEnable(true);
    for (int i = 0; i < sizeof(*data) / sizeof(char); i++)
    {
      if (!RADIO_ENABLE)
      {
        debug("writing data to FRAM");
        Serial.println(*data + i);
      }
      previousAddress += sizeof(uint8_t);
      fram.write8(previousAddress, *data+i);
    }
    fram.writeEnable(false);
  }
  void close()
  {
    /* Not needed */
    Serial.println("[??] warn: FramStorage::close() has no implementation");
  }

  bool isActive()
  {
    return active;
  }
};

/*
 * function:	  buildStorage()
 * description:	Factory function for creating an instance of Storage
 * input(s):	  int, selection, the type of Storage to create
 * output(s):	  Storage, new instance of Storage
 * Author: 	    Samuel Hild
 */
Storage * buildStorage(int selection)
{
  switch (selection)
  {
    case 1:
      Storage * primaryStorage = new SdStorage();

      if (primaryStorage->open())
      {
        debug("sd startup successful");
        return primaryStorage;
      }
      else
      {
        error("failed to start sd storage");
        return primaryStorage;
      }
      

    case 2:
      return new FramStorage();
    default:
      fatal("unknown storage device");
  }
}
