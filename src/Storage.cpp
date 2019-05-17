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

  void write(uint8_t data) override
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
  uint32_t previousAddress = 0x00;
  bool active = false;

protected:
public:
  FramStorage() {}

  bool open()
  {
    active = fram.begin();
    debug(F("fram dump:"));
    if (active)
    {
      uint8_t read;
      for (int i = 0; i < FRAM_SIZE; i++)
      {
        read = fram.read8(i);

        if (!((i + 1) % 19))
        {
          Serial.println();
        }
        else
        {
          Serial.print("0x");
        
          if (read < 0x10)
            Serial.print(F("0"));

          Serial.print(read, HEX);
          Serial.print(F(" "));
        }
      }
      if(RADIO_ENABLE) Serial.println();
    }
    return active;
  }
  
  void write(const char * data)
  {

    fram.writeEnable(true);
    for (unsigned int i = 0; i < sizeof(*data) / sizeof(char); i++)
    {
      if (!RADIO_ENABLE)
      {
        debug(F("writing data to FRAM"));
        Serial.println(*data + i);
      }
      previousAddress += sizeof(uint8_t);
      fram.write8(previousAddress, *data+i);
    }
    fram.writeEnable(false);
  }

  void write (uint8_t data)
  {
    fram.writeEnable(true);
    fram.write8(previousAddress, data);
    previousAddress += sizeof(uint8_t);
    fram.writeEnable(false);
  }

  void close()
  {
    /* Not needed */
    Serial.println(F("[??] warn: FramStorage::close() has no implementation"));
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
  Storage * newStorage;
  if (selection == 1)
  {
      newStorage = new SdStorage();

      if (newStorage->open())
        debug(F("sd startup successful"));
      else
        error(F("failed to start sd storage"));
  }
  else
    newStorage = new FramStorage();
  
  return newStorage;
}