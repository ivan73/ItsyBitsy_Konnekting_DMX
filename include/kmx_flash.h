#ifndef KMX_FLASH_h
#define KMX_FLASH_h

#include "Arduino.h"
#include <SPI.h>
#include <Adafruit_SPIFlash.h>
#include <SdFat.h>

typedef struct {
  byte *data = NULL;
  boolean valid;
} EEPROM_EMU;


class kmx_flash {

  public:
    //kmx_flash();
    kmx_flash(const char *fileName, uint16_t bufSize);

    /**
     * Return the buffer Adress
     */
    uint8_t *getBuffer();

    /**
     * 
     * 
     */
    void setDirty();

    /**
     * Read an eeprom cell
     * @param index
     * @return value
     */
    uint8_t read(int);

    /**
     * Write value to an eeprom cell
     * @param index
     * @param value
     */
    void write(int, uint8_t);

    /**
     * Update a eeprom cell
     * @param index
     * @param value
     */
    void update(int, uint8_t);

    /**
     * Check whether the eeprom data is valid
     * @return true, if eeprom data is valid (has been written at least once), false if not
     */
    bool isValid();

    /**
     * Write previously made eeprom changes to the underlying flash storage
     * Use this with care: Each and every commit will harm the flash and reduce it's lifetime (like with every flash memory)
     */
    void commit();

    uint16_t length() { return size; }

  private:
    void init();

    uint16_t size;
    const char *filename;
    bool _initialized = false;
    EEPROM_EMU _eeprom;
    bool _dirty = false;
    File writeFile;
};

#endif
