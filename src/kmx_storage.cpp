/* 
* kmx_storage.cpp
*
* Created: 31.12.2016 14:57:45
* Author: Ivan
*/


#include "kmx_storage.h"
#include "DebugUtil.h"

// default constructor
kmx_storage::kmx_storage(){
	//uint16_t sz = ((sizeof(EEPROMSTORAGE)/1023)+1)*1023;
	//storage_EEprom = kmx_flash("store.bin", sz);

		
	p_buffer = storage_EEprom.getBuffer();
	store = (EEPROMSTORAGE *) p_buffer;	//overlay the pointer to the storage onto the buffer
	//store = (EEPROMSTORAGE *) _buffer;

	//storage_actualScene = kmx_flash("actualscene.bin", KMX_MAX_GROUPS);
	p_actualScene = storage_actualScene.getBuffer();
} 

bool kmx_storage::readGroup(uint8_t group_nr)
{
	bool chksumOK;
	int group_index = (int)(&store->ee_groups[group_nr]) - (int)(store);	// Differenz der beiden Pointer-Adressen
	int ee_index = start_index + group_index;
	Debug.println(F("read: index(%d) size(%d)"), group_index, sizeof(ee_group));

	byte cks = 0;
	storage_EEprom.read(0);	// beim Lesen eines Bytes wird fals init() noch nicht ausgeführt zuerst die Flash-Datei gelesen.

	// for( unsigned int i = 0; i < sizeof(ee_group); ++i ) {
	// 	//_buffer[group_index+i] = readMemory(ee_index+i);
	// 	storage_EEprom.read(i);
	// }
	
	cks = getChksum(group_nr);
	if (store->ee_groups[group_nr].chksum == cks) {
		chksumOK = true;
	}
	else {
		chksumOK = false;
		DEBUG_PRINTLN(F("memRead: Chksum not ok Store-chksum(0x%02x) chksum(0x%02x)"),store->ee_groups[group_nr].chksum, cks);
		//clearStorageLightValues(group_nr);
	}

	DEBUG_PRINTLN(F("memRead: index(%d) bytes(%d) chksum(0x%02x) chksumOK(%d)"), ee_index, sizeof(ee_group),cks, chksumOK);
	return chksumOK;
}

void kmx_storage::writeGroup(uint8_t group_nr) {
	int group_index = (int)(&store->ee_groups[group_nr]) - (int)(store);	// Differenz der beiden Pointer-Adressen
	int ee_index = start_index + group_index;
	Debug.println(F("write: index(%d) size(%d)"), group_index, sizeof(ee_group));
	storage_EEprom.setDirty();

	// for( uint16_t i = 0; i < sizeof(ee_group); ++i ) {
	//  	//writeMemory(ee_index+i, _buffer[group_index+i]);
	// }
	commitMemory();
	DEBUG_PRINTLN(F("memWrite: index(%d) bytes(%d) chksum(0x%02x)"), ee_index, sizeof(ee_group),store->ee_groups[group_nr].chksum);
}

void kmx_storage::updateStorage()
{
	// byte tmp;
	// for(unsigned int i = 0; i < sizeof(EEPROMSTORAGE); ++i ) {
	// 	updateMemory(i+start_index, _buffer[i]);
	// 	tmp = readMemory(i+start_index);
	// 	if (tmp != _buffer[i])
	// 	{
	// 		Debug.println(F("memUpdate: VerifyError index=0x%02x value=0x%02x read=0x%02x"), i, _buffer[i], tmp);
	// 	}
	// }
	storage_EEprom.setDirty();
	commitMemory();
	Debug.println(F("memUpdate: index(%d) bytes(%d)"), start_index, sizeof(EEPROMSTORAGE));
}

void kmx_storage::clearStorageLightValues(uint8_t group_nr, uint8_t szene)
{
	for (uint8_t l = 0; l<KMX_MAX_LIGHTS; l++)
	{
		store->ee_groups[group_nr].scenes[szene].lightValues[l].R = 0;
		store->ee_groups[group_nr].scenes[szene].lightValues[l].G = 0;
		store->ee_groups[group_nr].scenes[szene].lightValues[l].B = 0;
		store->ee_groups[group_nr].scenes[szene].lightValues[l].W = 0;
	}
}

uint8_t kmx_storage::read_actualScene(uint8_t group_nr)
{
	// int actualSceneindex = (int)(&store->actual_scenes[group_nr]) - (int)(store);
	// int ee_index = start_index + actualSceneindex;
	// Debug.println(F("read_actualScene startindex: %d"), actualSceneindex);
	//return readMemory(ee_index);
	Debug.println(F("(%d) read_actualScene"), group_nr);
	return storage_actualScene.read(group_nr);
}

void kmx_storage::update_actualScene(uint8_t group_nr, uint8_t actualScene)
{
	// int actualSceneindex = (int)(&store->actual_scenes[group_nr]) - (int)(store);
	// int ee_index = start_index + actualSceneindex;
	// Debug.println(F("update_actualScene startindex: %d "), actualSceneindex);
	//updateMemory(ee_index, actualScene);
	//commitMemory();
	Debug.println(F("(%d) update_actualScene(%d)"), group_nr, actualScene);
	storage_actualScene.update(group_nr, actualScene);
	storage_actualScene.commit();
}

uint8_t kmx_storage::getChksum(uint8_t group_nr)
{
	int startindex = (int)(&store->ee_groups[group_nr]) - (int)(store) +1;	// Differenz der beiden Pointer-Adressen - 1.Byte nicht mitberechnen
	//int startindex = (int)(&store->ee_groups[group_nr].chksum) - (int)(store);	// Differenz der beiden Pointer-Adressen - 1.Byte nicht mitberechnen
	int size = sizeof(ee_group)-1;

	uint8_t cks = chksum8(&p_buffer[startindex], size);

	Debug.println(F("getChksum: index(%d) size(%d) chksum(0x%02x)"), startindex, size, cks);
	return cks;
}

void kmx_storage::setChksum(uint8_t group_nr)
{
	uint8_t cks = getChksum(group_nr);
	store->ee_groups[group_nr].chksum = cks;
	Debug.println(F("setChksum: group(%d) chksum(0x%02x) "), group_nr, cks);
}

uint8_t kmx_storage::chksum8(const byte *buff, size_t len)
{
	unsigned int sum;       // nothing gained in using smaller types!
	for ( sum = 0 ; len != 0 ; len-- )
	sum += *(buff++);   // parenthesis not required!
	return (uint8_t)sum;
}

int kmx_storage::readMemory(int index) {//Example for 24AA02E64 eeprom chip
	// when using external storage, implement READ command here
	//return EEPROM.read(index);
	return storage_EEprom.read(index);
}

void kmx_storage::writeMemory(int index, uint8_t val) {//Example for 24AA02E64 eeprom chip
	// when using external storage, implement WRITE command here
	//return EEPROM.write(index, val);
	storage_EEprom.write(index, val);
	
}

void kmx_storage::updateMemory(int index, uint8_t val) {
	// when using external storage, implement UPDATE command here
	//return EEPROM.update(index, val);
	int old = readMemory(index);
	if(old != val){
		Debug.println(F("writeMemory: index(%d) oldVal(0x%02x) newVal(0x%02x)"), index, old, val);
		writeMemory(index,val);
	}
}

void kmx_storage::commitMemory() {
	// this is useful if using flash memory to reduce write cycles
	// EEPROM needs no commit, so this function is intentionally left blank
	storage_EEprom.commit();
}