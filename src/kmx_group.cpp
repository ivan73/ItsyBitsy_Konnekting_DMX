/*
* kmx_group.cpp
*
* Created: 20.12.2016 15:47:28
*  Author: Ivan
*/ 

#include "kmx_group.h"
#include "DebugUtil.h"

kmx_group::kmx_group() {
}

void kmx_group::setEmergencyScene(bool state) {
	_isEmergency = state;
	if (_isEmergency)
	{
		if (sceneEmergemcy > 1)
			setScene(sceneEmergemcy);
		else if (_scene > 1)
			setScene(1);
	}
}

void kmx_group::setSceneInit(uint8_t scene) {
	_isManual = false;
	_lastmillisPM = millis();
	//_lastmillisManual = millis();
	setScene(scene);
}

void kmx_group::setSceneFromKNX(uint8_t scene) {
	_isManual = true;
	_lastmillisManual = millis();
	setScene(scene);
}

void kmx_group::setScene(uint8_t scene) {
	uint8_t used_scenes = p_ee_gr->used_scenes;

	if (scene > used_scenes) {
		Debug.println(F("(%d %s) scene not set(%d) - group has only (%d) scenes"), _groupIndex, _name, scene, used_scenes);
		return;
	}
	 
	//_scene = scene;
	Debug.println(F("(%d %s) set scene(%d) manu(%d) group max scenes (%d)"), _groupIndex, _name, scene, _isManual, used_scenes);
	int i = 0;
	
	if (scene == 0) {
		sendSleep(true);
		for (i = 0; i < _lightsCount; i++) {
			lights[i].setRGBW(lights->rgbw_off);
			//lights[i].setRGBW(&p_ee_gr->scenes[scene].lightValues[i].R);
		}
	}
	else if (scene == 1) {
		sendSleep(false);
		for (i = 0; i < _lightsCount; i++) {
			lights[i].setRGBW(lights->rgbw_off);
			//lights[i].setRGBW(&p_ee_gr->scenes[scene].lightValues[i].R);
		}
	}
	else {
		_offDelay = p_ee_gr->scenes[scene].offDelay;
		Debug.println(F("(%d %s) set offDelay: (%d)"), _groupIndex, _name, _offDelay);
		
		sendSleep(false);
		for (i = 0; i < _lightsCount; i++) {	
			bool active = bit_test(p_ee_gr->scenes[scene].activeLighs, i);
			if (init) {
				if (active) {
					lights[i].initRGBW(&p_ee_gr->scenes[scene].lightValues[i].R);
				}
				else {
					Debug.println(F("(%d %s) light(%d) from scene (%d) not active "), _groupIndex, _name, i, scene);
				}
			}
			else {
				Debug.println(F("(%d %s) is active: (%d) , scene(%d) activeLighs(%d)"), _groupIndex, _name, active, scene, p_ee_gr->scenes[scene].activeLighs);
				if (active) {
					lights[i].setRGBW(&p_ee_gr->scenes[scene].lightValues[i].R);
				}
				else {
					Debug.println(F("(%d %s) light(%d) from scene (%d) not active "), _groupIndex, _name, i, scene);
				}
			}
			init = false;
			Debug.println(F("(%d %s) set light: (%d) from scene (%d)"), _groupIndex, _name, i, scene);
		}
	}
	 
	// Aktuelle Szene im EEprom speichern - damit bei Neustart die gleiche Szene aktiviert werden kann
	if (_scene != scene) {
		_scene = scene;
		if (!_isEmergency)
			p_kmx_st->update_actualScene(_groupIndex, _scene);
		writeSceneToKNXFunc(_groupIndex, _scene);
		//writeActualLightsToKNX();
		if (_scene > 0)
		_status = true;
	else
		_status = false;
	}
	_lastmillisPM = millis();
}

void kmx_group::setSwitchPM(bool value) {
	unsigned long differenzManu = millis() -_lastmillisManual;
	_switchPM = value;
	_lastmillisPM = millis();
	Debug.println(F("(%d %s) PM - getriggert - Reset Nachlaufzeit(%d) PM1:(%d) PM2:(%d) PM:(%d) man(%d) t:(%d)"), 
		_groupIndex, _name, _offDelay, _switchPM1, _switchPM2, _switchPM, _isManual, differenzManu/1000);

	if (_isManual) {											                  // Manueller Modus aktiv (Taster)
		if (!_switchPM && differenzManu > (_offDelay*1000)) {	// Manueller Modus Wartzeit schon um?
			_isManual = false;							                    // beenden des Manuellen Modus
			Debug.println(F("(%d %s) PM - Manueller Modus AUS"), _groupIndex, _name);
		}
		else {
			_lastmillisManual = millis();
			Debug.println(F("(%d %s) PM - retrigger Manueller Modus"), _groupIndex, _name);
		}

	}
	else if (value && _scene < 2 && !_sleep) {	// Manueller Modus NICHT aktiv und keine Szene aktiv
		if (_night)										// Nachtmodus?
				setScene(_scenePMnight);
			else
				setScene(_scenePM);
	}
}

void kmx_group::setSwitchPM1(bool value) {
	_switchPM1 = value;
	setSwitchPM(_switchPM1 || _switchPM2);
}

void kmx_group::setSwitchPM2(bool value) {
	_switchPM2 = value;
	setSwitchPM(_switchPM1 || _switchPM2);
}

void kmx_group::setKeyPlus(bool value) {
	unsigned long currentmillis = 0;
	//unsigned long offDelayMillis = 0;
	unsigned long differenz = 0;
	
	currentmillis = millis();

	// Taste gedrückt
	if (value) 	{
		//differenz = currentmillis-_lastmillisKeyPlus1;
		_lastmillisKeyPlus1 = millis();
	}
	// Taste losgelassen
	else 	{
		_isManual = true;
		_lastmillisManual = millis();
		differenz = currentmillis-_lastmillisKeyPlus1;
		// Taste kurz gedrückt
		if (differenz < KMX_KEY_MILLIS_LONG) 		{
			if (_scene == 1)		
				setScene(3);
			else if (_scene > 1)
				setScene(_scene+1);
		}
		//Taste lang gedrückt, aber nicht ganz lange
		else {
			setScene(p_ee_gr->scenePlusLong);
		}
		_lastmillisKeyPlus0 = millis();
		
	}
	_stateKeyPlus = value;
	//verify();
}

void kmx_group::setKeyMinus(bool value) {
	unsigned long currentmillis = 0;
	//unsigned long offDelayMillis = 0;
	unsigned long differenz = 0;
	
	currentmillis = millis();
	
	
	if (value) {  // Taste gedrückt
		//differenz = currentmillis-_lastmillisKeyPlus1;
		_lastmillisKeyMinus1 = millis();
	}
	else {       // Taste losgelassen
		_isManual = true;
		_lastmillisManual = millis();
		differenz = currentmillis-_lastmillisKeyMinus1;
		
		if (differenz < KMX_KEY_MILLIS_LONG) {  // Taste kurz gedrückt
			//if (_scene > 10)		
			//	setScene(_scene-1);	// Nächstkleinere Szene
			if (_scene > 1)	// Wenn kleinste Szene aktiv dann Aus
				setScene(1);		// Aus
			else if (_scene == 1)	// Wenn Aus dann wieder Ein
				setScene(2);		// Einschalten der kleinsten Szene
			else if (_scene == 0)	// Wenn Schlafen dann Aus
			{
				setScene(1);		// Aus
				_isManual = false;
			}
		}
		//Taste lang gedrückt, aber nicht ganz lange
		else { //if (differenz < KMX_KEY_MILLIS_PRESSED)
			if (p_ee_gr->sceneMinusLong == 0 && _scene == 1)
				setScene(0);
			else if (p_ee_gr->sceneMinusLong == 0 && _scene > 1)
				setScene(1);
			else if (p_ee_gr->sceneMinusLong != 0)
				setScene(p_ee_gr->sceneMinusLong);
		}
		_lastmillisKeyMinus0 = millis();
	}
	_stateKeyPlus = value;
}

void kmx_group::setKeyPanik(bool value) {
	if (value && _scene >= 1) {
		Debug.println(F("(%d %s) KeyPanik set scene %d"), _groupIndex, _name, p_ee_gr->scenePanic);
		setScene(p_ee_gr->scenePanic);
	}
}

void kmx_group::setContinous(bool state) {
	_isContinous = state;
	Debug.println(F("(%d %s) setContinous to %d"), _groupIndex, _name, _isContinous);
}

void kmx_group::update() {
	for (int index = 0; index < _lightsCount; index++)
		lights[index].update();
}

// Prüfen der Bedingungen und setzen der Szene
bool kmx_group::verify() {
	unsigned long offDelayMillis = 0;
	unsigned long differenzPM = 0;
	unsigned long differenzManu = 0;

	if ((millis() - _lastmillisVerify) < VERIFY_MILLIS)
		return false;
	_lastmillisVerify = millis();
		
	offDelayMillis = _offDelay*1000;
	differenzPM = millis() -_lastmillisPM;
	differenzManu = millis() -_lastmillisManual;

//	if (_isManual && differenzManu > offDelayMillis) 	{
  if (_isManual && !_switchPM && differenzManu > offDelayMillis) {	// Manueller Modus Wartzeit schon um?
    _isManual = false;							                    // beenden des Manuellen Modus
    Debug.println(F("(%d %s) verify - Manueller Modus AUS"), _groupIndex, _name);
    _lastmillisPM = millis();
  }

	// Nachlauf PM und manual beendet
	if (_scene > 1 && !_isManual&& !_switchPM && !_sleep && differenzManu > offDelayMillis && differenzPM > offDelayMillis && !_isContinous && !_isEmergency) 
		setScene(1);

	return true;
}

void kmx_group::sendSleep(bool value) {
	if (_sleep != value) {
		_sleep = value;
		
		if (*writeSleepToKNXFunc != NULL) {
			//Debug.println(F("write to knx: using fctptr"));
			writeSleepToKNXFunc(_groupIndex, _sleep);
		} 
		else {
			Debug.println(F("sendSleep: No fctptr defined!!"));
		}
	}
}

void kmx_group::setWriteMsgToKNXFunc(void (*func)(byte*)) {
	writeMsgToKNXFunc = func;
}

void kmx_group::setIndex(uint8_t index) {
	_groupIndex = index;
	if (p_kmx_st != NULL)
		p_ee_gr = &p_kmx_st->store->ee_groups[_groupIndex];
}

void kmx_group::setName(char *name) {
	_name[0] = 0;
	strcat(_name, name);	
}

void kmx_group::setStorage(kmx_storage *kmx_st) {
	p_kmx_st = kmx_st;
}

void kmx_group::setLightsCount(uint8_t lCount) {
	_lightsCount = lCount;
}

void kmx_group::setNight(bool value) {
	_night = value;
}

uint8_t kmx_group::getScene() {
	return _scene;
}


char *kmx_group::getName(void) {
	return _name;
}

// Parameter: Index der Leuchte
// Rückgabe: des LichtObjektes 
kmx_light *kmx_group::getLight(uint8_t index) {
	return &lights[index];
}


void kmx_group::handleMsg(byte msg[]) {
	bool writeBit = bit_test(msg[0], 7);;
	uint8_t command = (msg[0] & 0b01110000) >> 4;	//WcccGGGG
	uint8_t groupIndex = (msg[0] & 0b0001111);		//WcccGGGG

	if (groupIndex != _groupIndex || groupIndex >= KMX_MAX_GROUPS) 	{
		Debug.println(F("handleMsg: groupindex (%d) false _group_index (%d) MaxGroups(&d)"), groupIndex, _groupIndex, KMX_MAX_GROUPS);
		return;
	}

	

	if (command < KMX_MSG_WRITE_ccc_ScenePara) {
		uint8_t sceneIndex  = (msg[1] & 0b0001111);		//xlllSSSS
		uint8_t lightsCnt = (msg[1] & 0b01110000) >> 4;	//xlllSSSS

		bool fadeBit = bit_test(command, 0);
		bool saveBit = bit_test(command, 1);

		DEBUG_PRINTLN(F("handleMsg: group (%d) scene (%d) writeBit (%d) command (%d)"), groupIndex, sceneIndex, writeBit, command);

		// 88 61 23 32 45 14 14 14 14 22 14 00 00 00
		uint8_t lightIndex = 0;
		uint8_t rgbw[4];
		uint8_t byteCnt = 2;
		uint8_t bytesPerLight = 0;
		light_type type = none;

		for (int l=0; l<lightsCnt; l++) 		{
			lightIndex = (msg[byteCnt] & 0b0001111);	//xtttxLLL
			type = (light_type)((msg[byteCnt] & 0b01110000) >> 4);
			bytesPerLight = pgm_read_byte(&light_typesBytes[type]);
			byteCnt++;
			for (uint8_t i=0; i<bytesPerLight; i++) 			{
				if (type == dimming)
					i = 3;	// W
				if (i < sizeof(rgbw))
					rgbw[i] = msg[byteCnt];
				byteCnt++;
				Debug.println(F("handleMsg: lightValue (%d) "), rgbw[i]);
			}
			writeLight(rgbw, sceneIndex, lightIndex, fadeBit);	//schreibeSzenenWerte
		}
		p_kmx_st->setChksum(_groupIndex);
		if (saveBit) 	{
			Debug.println(F("handleMsg: save values to EEprom (update)"));
			//p_kmx_st->setChksum(_groupIndex);
			p_kmx_st->updateStorage();
		}
	}

	if (command == KMX_MSG_WRITE_ccc_SaveScene) {
		Debug.println(F("handleMsg: save values to EEprom (update)"));
		p_kmx_st->setChksum(_groupIndex);
		p_kmx_st->updateStorage();
		//p_kmx_st->update(_groupIndex);
		return;
	}

	if (command == KMX_MSG_WRITE_ccc_ScenePara) {
		uint8_t sceneIndex  = (msg[1] & 0b0001111);		//xlllSSSS

		if (sceneIndex <= KMX_MAX_SCENES) {
			ee_scene *eeSc = &p_ee_gr->scenes[sceneIndex];
			// WcG SS AL DL oDL oDH     SS-SceneNr AL-activeLights DL-dimmableLights oD-offDelay(LowByte) oD-offDelay(HighByte)
			eeSc->activeLighs = msg[2];
			eeSc->dimmableLights = msg[3];
			eeSc->offDelay = 0;
			eeSc->offDelay |= (uint16_t)msg[4];
			eeSc->offDelay |= (((uint16_t)msg[5])<<8);

			Debug.println(F("handleMsg: activeLighs (%d) dimmableLights(%d) offDelay(%d)"),  eeSc->activeLighs,  eeSc->dimmableLights,eeSc->offDelay);
			p_kmx_st->setChksum(_groupIndex);
		}
		else {
			Debug.println(F("handleMsg: sceneIndex (%d) to high. max (%d)"), sceneIndex, KMX_MAX_SCENES);
		}
	}

	if (command == KMX_MSG_WRITE_ccc_GroupPara) {
		// WcG sPM sPMn sPL sML sP uS  sPM-scenePM sPMn-scenePMnight sPL-scenePlusLong sML-sceneMinusLong sP-scenePanic uS-usedScenes
		p_ee_gr->scenePM = msg[1];
		p_ee_gr->scenePMnight = msg[2];
		p_ee_gr->scenePlusLong = msg[3];
		p_ee_gr->sceneMinusLong = msg[4];
		p_ee_gr->scenePanic = msg[5];
		p_ee_gr->used_scenes = msg[6];

		Debug.println(F("handleMsg: GroupPara(%d) scenePM(%d) scenePMnight(%d) scenePlusLong(%d) sceneMinusLong(%d) scenePanic(%d) usedScenes(%d)"),
			_groupIndex ,p_ee_gr->scenePM, p_ee_gr->scenePMnight, p_ee_gr->scenePlusLong, p_ee_gr->sceneMinusLong, p_ee_gr->scenePanic, p_ee_gr->used_scenes );
		p_kmx_st->setChksum(_groupIndex);

	}

}

void kmx_group::writeActualLightsToKNX() {
	bool writeBit = false;
	uint8_t command = 0;
	//uint8_t lightIndex = 0;
	//uint8_t sceneIndex = 0;
	uint8_t lightsCnt = 0;
	//uint8_t rgbw[4];
	uint8_t byteCnt = 2;
	uint8_t bytesPerLight = 0;
	light_type type = none;
	byte value = 0;

	memset(&_msgToSend[0], 0, sizeof(_msgToSend));
	_msgToSend[0] =  (writeBit << 7);
	_msgToSend[0] |= (command << 4);
	_msgToSend[0] |= _groupIndex;

	
	//if (_scene > 9)
	//{
		//sceneIndex = _scene-10;
	//}
	//else
	//{
		//DEBUG_PRINTLN(F("writeActualLightsToKNX: grId (%d) scene not send (%d)"), _group_index, _scene);
		//return;
	//}

	for (byte lightNr=0; lightNr < _lightsCount; lightNr++) {
		type = lights[lightNr].getType();
		bytesPerLight = pgm_read_byte(&light_typesBytes[type]);
		
		//wenn keine Platz mehr im Paket dann Paket zuerst senden und neues Paket beginnen
		if ((byteCnt+bytesPerLight+1) > 13) {
			_msgToSend[1] = (lightsCnt<<4) | _scene;
			//Debug.println(F("writeActualLightsToKNX: grId (%d) msg (%s)"), _groupIndex, (char*)_msgToSend);
			Debug.println(F("writeActualLightsToKNX: grId (%d)"), _groupIndex);
			writeMsgToKNXFunc(_msgToSend);	// send
			memset(&_msgToSend[1], 0, sizeof(_msgToSend)-1);
			byteCnt = 2;
		}
		if ((byteCnt+bytesPerLight+1) <= 13) {
			for (byte x=0; x<=bytesPerLight; x++) {
				if (x==0) {
					_msgToSend[byteCnt] = (type << 4) | (lightNr);
					byteCnt++;
				}
				else {
					uint8_t * rgbw = &p_ee_gr->scenes[_scene].lightValues[lightNr].R;
					value = (byte)rgbw[x-1];
					if (bytesPerLight == 1)
						value = (byte)p_ee_gr->scenes[_scene].lightValues[lightNr].W;
					_msgToSend[byteCnt] = value;
					byteCnt++;
				}
			}
			lightsCnt++;
		}
	}
	_msgToSend[1] = (lightsCnt << 4) | _scene;
	//Debug.println(F("writeActualLightsToKNX: grId (%d) msg (%s)"), _groupIndex, (char*)_msgToSend);
	Debug.println(F("writeActualLightsToKNX: grId (%d)"), _groupIndex);
	writeMsgToKNXFunc(_msgToSend);	// send
}

void kmx_group::writeLight(byte rgbw[4], uint8_t sceneIndex, uint8_t lightIndex, bool fade) {
	unsigned int fadeTimeBak;
	if (lightIndex <= _lightsCount && lightIndex < KMX_MAX_LIGHTS) {
		if (sceneIndex >= KMX_MAX_SCENES || sceneIndex > p_ee_gr->used_scenes) {	
			Debug.println(F("handleMsg: sceneIndex (%d) to high. max (%d)"), sceneIndex, KMX_MAX_SCENES);
			return;
		}
		p_ee_gr->scenes[sceneIndex].lightValues[lightIndex].R = rgbw[0];
		p_ee_gr->scenes[sceneIndex].lightValues[lightIndex].G = rgbw[1];
		p_ee_gr->scenes[sceneIndex].lightValues[lightIndex].B = rgbw[2];
		p_ee_gr->scenes[sceneIndex].lightValues[lightIndex].W = rgbw[3];
		
		// if (sceneIndex > p_ee_gr->used_scenes)
		// {
		// 	Debug.println(F("handleMsg: new used_scenes (%d) old (%d)"), sceneIndex, p_ee_gr->used_scenes);
		// 	p_ee_gr->used_scenes = sceneIndex;
		// }
		if (fade) {
			if (bit_test(p_ee_gr->scenes[sceneIndex].activeLighs, lightIndex)) {
				fadeTimeBak = lights[lightIndex].getFadeTime();
				lights[lightIndex].setFadeTime(50);
				lights[lightIndex].setRGBW(rgbw);
				lights[lightIndex].setFadeTime(fadeTimeBak);
			}
		}
	}
	else {
		Debug.println(F("handleMsg: lightIndex (%d) to high. max (%d)"), lightIndex, _lightsCount);
	}
}

//writeSceneToKNX
void kmx_group::setWriteSceneToKNXFunc(void (*func)(int, uint8_t)) {
	writeSceneToKNXFunc = func;
}

void kmx_group::setWriteSleepToKNXFunc(void (*func)(int, bool)) {
	writeSleepToKNXFunc = func;
}

// void kmx_group::setWriteChannelValueToKNXFunc(void (*func)(int, uint8_t, uint8_t)) {
//  	writeChannelValueToKNXFunc = func;
// }