/*
 * kmx_group.h
 *
 * Created: 20.12.2016 15:47:06
 *  Author: Ivan
 */ 


#ifndef KMX_GROUP_H_
#define KMX_GROUP_H_

#include "Arduino.h"
#include "kmx_storage.h"
#include "kmx_light.h"

#define VERIFY_MILLIS 1000			// alle xx mm wird der Status überprüft

#define KMX_KEY_MILLIS_LONG 1000	// ab xx ms wird Tastendruck nach loslassen als lange interpretiert
#define KMX_KEY_MILLIS_PRESSED 2000	// ab xx ms wird Tastendruck bei gedrückthalten als gedrückt interpretiert..

#define KMX_MSG_MAX_BYTES	14

#define bit_test(val, bit_no) (((val) >> (bit_no)) & 1)
#define bit_set(var, bit_no) (var = var | (1<<(bit_no)))
#define bit_clear(var, bit_no) (var = var | (0<<(bit_no)))

class kmx_group 
{
	public:
		//uint8_t lightsCount = 0;	// number of lights
		kmx_light lights[KMX_MAX_LIGHTS];

		// diese Szene wird bei Notbeleuchtung aktiviert
		uint8_t sceneEmergemcy = 0;

	private:

		kmx_storage *p_kmx_st = NULL;
		ee_group *p_ee_gr = NULL;

		// Anzahl der Leuchten
		uint8_t _lightsCount = 0;	
		
		// Name der Gruppe
		char _name[12] = "";
		
		// Szene
		uint8_t _scene = 0;

		// Status PM
		bool _switchPM = false;
		
		// Status PM1
		bool _switchPM1 = false;

		// Status PM2
		bool _switchPM2 = false;

		// Status PM incl Nachlaufzeit
		bool _PMdelayed = false;
			
		bool _stateKeyPlus = false;
		bool _stateKeyMinus = false;
		
		// Status Taster		
		bool _switchManual = false;

		// NachtModus
		bool _night = false;

		// SchlafenModus
		bool _sleep = false;

		// Nachlaufzeit PM in Sekunden
		uint16_t _offDelay = 600;	

		// Status der Belechtung Ein oder Aus
		bool _status = false;		

		
		
		// diese Szene wird aktiviert bei PM
		uint8_t _scenePM = 3;	

		// diese Szene wird in der Nacht aktiviert bei PM
		uint8_t _scenePMnight = 2;

		
		
		// diese Szene wird aktiviert TasterPlus lange
		//uint8_t _scenePlusLong = 12;
		//uint8_t * _scenePlusLong = NULL;

		// diese Szene wird aktiviert TasterMinus lange
		//uint8_t _sceneMinusLong = 0;
		//uint8_t *_sceneMinusLong = NULL;
		// Zeitpunkt des letzen PM Trigger
		
		unsigned long  _lastmillisPM = _offDelay * 1000;	// Nachlaufzeit ist erreicht - damit PM sofort einschalten kann	
		unsigned long  _lastmillisKeyPlus1 = 0;	
		unsigned long  _lastmillisKeyPlus0 = 0;	
		unsigned long  _lastmillisKeyMinus1 = 0;
		unsigned long  _lastmillisKeyMinus0 = 0;
		unsigned long  _lastmillisManual = 0;
		unsigned long  _lastmillisVerify = 0;

		// wenn eine Szene über Taster/Visu ausgewählt wurde
		bool _isManual = false;

		// Szene im Dauerbetrieb
		bool _isContinous = false;

		// Notbeleuchtung aktiv
		bool _isEmergency = false;

		// beim erstem mal Setzen einer Szene (Neustart) wird diese ohne Fade aufgerufen
		bool init = true;

		// Index der Gruppe
		uint8_t _groupIndex = 0;

		// Data to send to knx-Addr
		byte _msgToSend[14] {0};	

	public:
		kmx_group  ();
		void setEmergencyScene(bool state);
		void setSceneInit(uint8_t scene);
		void setSceneFromKNX(uint8_t scene);
		void setScene(uint8_t value);
		void setSwitchPM(bool value);	// Setzt den PM - Status
		void setSwitchPM1(bool value);	// Setzt den PM1 - Status
		void setSwitchPM2(bool value);	// Setzt den PM2 - Status
		void setKeyPlus(bool value);	// Setzt den TasterPlus - Status
		void setKeyMinus(bool value);	// Setzt den TasterMinus - Status
		void setKeyPanik(bool value);	// Setzt den Panik-Status
		void setContinous(bool state);	// Setzt oder löscht den Dauerbetrieb-Status
		void update();					// update (fade) the light - values
		bool verify();					// check the current scene-state return TRUE if the state is changed
		void sendSleep(bool value);

		uint8_t getScene();

		void setIndex(uint8_t index);
		void setStorage(kmx_storage *kmx_st);
		void setLightsCount(uint8_t lCount);
		void setNight(bool value);
		void setName(char *name);
		char *getName(void);
		kmx_light *getLight(uint8_t index);

		void handleMsg(byte msg[]);
		void writeActualLightsToKNX();
		void writeLight(byte rgbw[], uint8_t sceneIndex, uint8_t lightIndex, bool fade);
		static void writeChannelValueToKNX(uint8_t channel, uint8_t val);
		void setWriteMsgToKNXFunc(void (*func)(byte*));
		void setWriteSceneToKNXFunc(void (*func)(int, uint8_t));
		void setWriteSleepToKNXFunc(void (*func)(int, bool));
		// void setWriteChannelValueToKNXFunc(void (*func)(int, uint8_t, uint8_t));

	private:
		void (*writeMsgToKNXFunc)(byte*);
		void (*writeSceneToKNXFunc)(int, uint8_t);
		void (*writeSleepToKNXFunc)(int, bool);
		// void (*writeChannelValueToKNXFunc)(int, uint8_t, uint8_t);

};




#endif /* KMX_GROUP_H_ */