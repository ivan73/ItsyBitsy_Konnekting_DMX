#include "kmx_flash.h"
#include "DebugUtil.h"

//https://github.com/SirSydom/konnekting_FileAsEEPROM/blob/master/FileAsEEPROM.cpp
//#define FILE_AS_EEPROM_DEBUG

#if defined(EXTERNAL_FLASH_USE_QSPI)
  Adafruit_FlashTransport_QSPI flashTransport;
#endif

Adafruit_SPIFlash flash(&flashTransport);

FatFileSystem fatfs;

// kmx_flash::kmx_flash()
// {
//     size = 1023;
//     filename = "eeprom";
//     _eeprom.data = (byte*)malloc(size);  
// }

kmx_flash::kmx_flash(const char *fileName, uint16_t bufSize)
{
    size = bufSize;
    filename = fileName;
    if ( _eeprom.data != NULL)
        free(_eeprom.data);
    _eeprom.data = (byte*)calloc(bufSize, sizeof(byte));
    memset(_eeprom.data, 0xFF, size);

  // Empty
}

byte *kmx_flash::getBuffer()
{
    return _eeprom.data;
}

void kmx_flash::setDirty()
{
    _dirty = true;
}

uint8_t kmx_flash::read(int address)
{
  if(!_initialized)
    init();
  return _eeprom.data[address];
}

void kmx_flash::update(int address, uint8_t value)
{
  if(!_initialized)
    init();
  if(_eeprom.data[address] != value)
  {
    _dirty = true;
    _eeprom.data[address] = value;
  }
}

void kmx_flash::write(int address, uint8_t value)
{
  update(address, value);
}


void kmx_flash::init()
{
  #ifdef FILE_AS_EEPROM_DEBUG
  Debug.println(F("FileAsEEPROM: init()"));
  #endif
  flash.begin();
  fatfs.begin(&flash);
  //fatfs.remove(EEPROM_FILENAME);

  if(fatfs.exists(filename))
  {
    //Debug.println("FileAsEEPROM: init() file correct, read");
    writeFile = fatfs.open(filename, FILE_READ);
    writeFile.read(_eeprom.data, size);
    writeFile.close();
  }
  else
  {
    #ifdef FILE_AS_EEPROM_DEBUG
    Debug.println(F("FileAsEEPROM: init() file not correct"));
    //Debug.println(F(fatfs.exists(EEPROM_FILENAME));
    #endif
    memset(_eeprom.data, 0xFF, size);
    _dirty = true;
  }
  _initialized = true;
}

bool kmx_flash::isValid()
{
  if(!_initialized)
    init();
  return _eeprom.valid;
}

void kmx_flash::commit()
{
  #ifdef FILE_AS_EEPROM_DEBUG
  Debug.println(F("FileAsEEPROM: (%s) commit()"),filename);
  #endif
  if(!_initialized)
    init();
  if(_dirty)
  {
    #ifdef FILE_AS_EEPROM_DEBUG
    Debug.println(F("FileAsEEPROM: commit() _dirty == true"));
    #endif
    _eeprom.valid = true;
    writeFile = fatfs.open(filename, FILE_WRITE);
    #ifdef FILE_AS_EEPROM_DEBUG
    if(!writeFile) Debug.println(F("open failed"));
    #endif
    #ifdef FILE_AS_EEPROM_DEBUG
    if(!writeFile.seek(0)) Debug.println(F("seek failed"));
    #else
    writeFile.seek(0);
    #endif
    
    #ifdef FILE_AS_EEPROM_DEBUG
    Debug.println(F("chars written: (%d) File size now (%d)"), writeFile.write(_eeprom.data, size), writeFile.size());
    #else
    writeFile.write(_eeprom.data, size);
    #endif
    writeFile.close();
  }
}