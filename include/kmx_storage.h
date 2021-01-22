/* 
* kmx_storage.h
*
* Created: 31.12.2016 14:57:45
* Author: Ivan
*/
// ################################################
// ### KMX Configuration
// ################################################

#include "Arduino.h"
//#include <Wire.h>	// external I2C EEPROM.

//#define EEPROM_EMULATION_SIZE 16348
//#include <FileAsEEPROM.h>
#include <kmx_flash.h>

#define KMX_MAX_LIGHTS 8
#define KMX_MAX_GROUPS 15	//15
#define KMX_MAX_SCENES 10	//12
#define KMX_MAX_TEXT 12

//#define EE_MAX_SIZE 0x7FFF //

#define KMX_MSG_WRITE_ccc_Lights 0b000      // schreibe LeuchtenWerte ohne diese zu dimmen
#define KMX_MSG_WRITE_ccc_LightsFade 0b001  // schreibe LeuchtenWerte und dimme das Licht auf diese Werte
#define KMX_MSG_WRITE_ccc_SaveScene 0b010   // speichere alle LeuchtenWerte ins EEprom
#define KMX_MSG_WRITE_ccc_ScenePara 0b100   // schreibe die SzenenParameter
#define KMX_MSG_WRITE_ccc_GroupPara 0b101   // schreibe die GruppenParameter

struct ee_light
{
	uint8_t R = 0;	// Helligkeitswert der Leuchten
	uint8_t G = 0;
	uint8_t B = 0;
	uint8_t W = 0;
};

struct ee_scene {
	ee_light lightValues[KMX_MAX_LIGHTS+1];	// Liste der Leuchten-Helligkeitswerte
	uint8_t activeLighs;					// aktive Leuchten der Szenen z.B. 0b00000011 Leuchte 87654321
	uint8_t dimmableLights;					// dimmbare Leuchten der Szenen z.B. 0b00000011 Leuchte 87654321
	uint8_t switchScene;					// verschiedene Flags der Szene bit0 = Szene kann mit +- Taster ausgewählt werden
	uint16_t offDelay;						// Nachlaufzeit in Sec
	//char sceneName[KMX_MAX_TEXT];			// Namen der Szene
} ;

struct ee_group {
	//uint8_t actual_scene=0x0;				// Aktuelle Szene - ACHTUNG dieser Wert muss an erster Stelle stehen, da die Szene direkt an die 1Speicherstelle gespeichert wird
	uint8_t chksum = 0;						// Checksumme des Buffers (ACHTUNG das 1.+2. Byte actual_scene und chksum wird nicht dazugerechnet!!)
	uint8_t used_scenes = 5;				// Anzahl der benutzten Szenen
	ee_scene scenes[KMX_MAX_SCENES+1];		// SzenenListe der Gruppe
	uint8_t scenePM = 3;					// PM aktiviert diese Szene
	uint8_t scenePMnight = 2;				// PM aktiviert diese Szene bei Nacht
	uint8_t sceneMinusLong = 5;				// -Taste Lang gedrückt aktiviert diese Szene
	uint8_t scenePlusLong = 4;				// +Taste lang gedrückt aktiviert diese Szene
	uint8_t scenePanic = 5;					// Panik-Taste aktiviert diese Szene
} ;
	
typedef struct {
	//uint8_t actual_scenes[KMX_MAX_GROUPS];
	ee_group ee_groups[KMX_MAX_GROUPS+1];
	
	} EEPROMSTORAGE;

#ifndef __KMX_STORAGE_H__
#define __KMX_STORAGE_H__


class kmx_storage
{
public:
	
	unsigned int start_index = 0;	// Index ab welcher Adresse die Parameter gespeichert werden (nach den konnekting-Parameter)
	EEPROMSTORAGE * store;
	
	kmx_flash storage_EEprom = kmx_flash("store.bin", ((sizeof(EEPROMSTORAGE)/1023)+1)*1023);	// hier werden die Szenen der Gruppen gespeichert
	kmx_flash storage_actualScene = kmx_flash("actualscene.bin", KMX_MAX_GROUPS);				// hier wird die aktuelle SzenenNr. der Gruppe gespeichert

	kmx_storage();
	bool readGroup(uint8_t group_nr);	// reads the eeprom
	void writeGroup(uint8_t group_nr);  //a function to write to the eeprom
	void updateStorage();
	void clearStorageLightValues(uint8_t group_nr, uint8_t szene);
	uint8_t read_actualScene(uint8_t group_nr);
	void update_actualScene(uint8_t group_nr, uint8_t actualScene);

	int readMemory(int index);
	void writeMemory(int index, uint8_t val);
	void updateMemory(int index, uint8_t val);
	void commitMemory();
	
	uint8_t chksum8(const byte *buff, size_t len);
	uint8_t getChksum(uint8_t group_nr);
	void setChksum(uint8_t group_nr);
	
			
private:
	//byte _buffer[ sizeof(EEPROMSTORAGE)] = {0xFF};
	byte *p_buffer;
	uint8_t *p_actualScene;
};

#endif //__KMX_STORAGE_H__
