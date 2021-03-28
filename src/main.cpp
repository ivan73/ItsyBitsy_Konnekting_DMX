#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <Adafruit_DotStar.h>
#include <Adafruit_NeoPixel_ZeroDMA.h>
 
#include <Bounce2.h>
#include <MenuBackend.h>

#include <KonnektingDevice.h>
#include <kdevice_KONNEKTING_DMX2.h>

#include <kmx_flash.h>

#include <kmx_group.h>
#include <kmx_menu.h>
#include <kmx_defaultSzenes.h>

// ################################################
// ### DEBUG CONFIGURATION
// ################################################
// #define KDEBUG // comment this line to disable DEBUG mode

#ifdef KDEBUG
#include <DebugUtil.h>
#define DEBUGSERIAL Serial
#endif

// ################################################
// ### KONNEKTING Configuration
// ################################################
#define KNX_SERIAL Serial1 // Leonardo/Micro/Zero etc. use Serial1

// ################################################
// ### IO Configuration
// ################################################
#define PROG_LED_PIN 11		//PB9 Pin8 //16
#define PROG_BUTTON_PIN A0	//PB8 Pin7 //15
#define ERROR_LED 10 //25 //or change it to another pin

#define OLED_RESET_PIN 12 // Reset pin # (or -1 if sharing Arduino reset pin)

#define NEOPIXEL_PIN 5

#define UP_BUTTON_PIN A3
#define DOWN_BUTTON_PIN A2
#define SAVEB_PIN 7

#define DMX_TX_PIN A4
#define DMX_RX_PIN A1

Bounce buttonUp = Bounce(); 
Bounce buttonDown = Bounce(); 


// ################################################
// ### Global variables, sketch related
// ################################################
	// für FreeRam ()
unsigned long lastmillis = millis();
unsigned long currentmillis = millis();
unsigned long currentmicros = micros();
unsigned int seconds = 0;

uint8_t grFade = 0;
uint16_t FreeEepromOffset = 0xFFFF;

byte msgBuffer[14];

uint8_t kmx_para_groups = KMX_MAX_GROUPS; 	// Anzahl der Beleuchtungs-Gruppen
uint8_t kmx_para_offset_CommObjects = 20; 	// Startindex der Gruppen-CommObjekte
uint8_t kmx_para_lengh_CommObjects = 15; 	// Anzahl der Gruppen-CommObjekte
uint8_t kmx_para_offset_Parameter = 10; 	// Startindex der Gruppen-Parameter
uint8_t kmx_para_lengh_Parameter = 10; 		// Anzahl der Gruppen-Parameter

bool knx_device_ok = false;
int knx_device_init_attempts = 0;
bool prgState = false;

kmx_flash kon_EEprom = kmx_flash("konnekting.bin", 2048);

kmx_storage kmx_st;

kmx_group kmx_gr[KMX_MAX_GROUPS];
kmx_group *p_group = NULL;

uint8_t twoButtons = 0;
bool workingState = true;
uint8_t manGroup = 1;
uint8_t manScene = 1;

Adafruit_NeoPixel_ZeroDMA strip(1, NEOPIXEL_PIN, NEO_GRB);


#define NUMPIXELS 	1   // There is only one pixel on the board
#define DATAPIN   	8	// Use these pin definitions for the ItsyBitsy M4
#define CLOCKPIN   	6

uint32_t color_RED = 0xFF0000;
uint32_t color_GREEN = 0x00FF00;
uint32_t color_BLUE = 0x0000FF;
uint32_t color_ORANGE = 0xFFA500;
uint32_t color_VIOLET = 0xBC13FE;
uint32_t color_WHITE = 0xFFFFFF;

//Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &Wire);
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 64, &Wire, OLED_RESET_PIN);
Adafruit_DotStar stripDotstar(NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BGR);

bool colorToggle = false;

extern "C" char *sbrk(int i);
int FreeRam () {
	char stack_dummy = 0;
	return &stack_dummy - sbrk(0);
}

void knxEvents(byte index);
void writeSceneToKNX(int group_index, uint8_t scene);
void writeSleepToKNX(int group_index, bool sleep);
void writeMsgToKNX(byte msg[]);
void writeChannelValueToKNX(int group_index, uint8_t channel, uint8_t val);
void manualScene(ManualSceneEvent sceneEvent);
void LoopTiming();

kmx_menu kmxMenu = kmx_menu(UP_BUTTON_PIN, DOWN_BUTTON_PIN, display, manualScene);

int readMemory(int index)
{
    return kon_EEprom.read(index);
}
void writeMemory(int index, uint8_t val)
{
    kon_EEprom.write(index, val);
}
void updateMemory(int index, uint8_t val)
{
    if (readMemory(index) != val) {
        writeMemory(index, val);
    }
}
void commitMemory()
{
    kon_EEprom.commit();
}
// ################################################
// ### SETUP
// ################################################

void setup() {
	#ifdef KDEBUG
		DEBUGSERIAL.begin(115200);	    // Start debug serial with 9600 bauds
	while (!DEBUGSERIAL)	delay(100);
		Debug.setPrintStream(&DEBUGSERIAL);	// make debug serial port known to debug class Means: KONNEKTING will sue the same serial port for console debugging
	#endif

	Debug.print(F("KONNEKTING_DMX\n"));

	//pinMode(ERROR_LED, OUTPUT);
	pinMode(SAVEB_PIN, INPUT_PULLUP);

	stripDotstar.begin();
	stripDotstar.setPixelColor(0, 0);
	stripDotstar.show();

	strip.begin(); // Initialize pins for output
	strip.setBrightness(80);

	strip.setPixelColor(0, color_GREEN);
	strip.show();
	delay(1000);
	
	strip.setPixelColor(0, 0);
	strip.show();
	
	//display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Address 0x3C for 128x32
	display.begin(SSD1306_SWITCHCAPVCC, 0x3d); // Address 0x3C for 128x64
	display.display();
	display.clearDisplay();	// Clear the buffer.
	display.display();

	display.setTextSize(1);	  // text display
	display.setTextColor(SSD1306_WHITE);
	display.setCursor(0,0);
	display.print("Konnekting_DMX\n");
	display.print("setup");
	display.setCursor(0,0);
	display.display();
	delay(1000);

	// Konnekting.setMemoryReadFunc(&kmx_storage::readMemory);
	// Konnekting.setMemoryWriteFunc(&kmx_storage::writeMemory);
	// Konnekting.setMemoryUpdateFunc(&kmx_storage::updateMemory);
	// Konnekting.setMemoryCommitFunc(&kmx_storage::commitMemory);

	
	Konnekting.setMemoryReadFunc(readMemory);
	Konnekting.setMemoryWriteFunc(writeMemory);
	Konnekting.setMemoryUpdateFunc(updateMemory);
	Konnekting.setMemoryCommitFunc(commitMemory);

	//// Initialize KNX enabled Arduino Board
	do 
	{
		Debug.println(F("init device - (%d) Attempt \n"), knx_device_init_attempts);
		

		if (digitalRead(SAVEB_PIN) == true)	// Ausfall KNX-Busspannung
		{
			if (knx_device_init_attempts == 0)
				Konnekting.init(KNX_SERIAL, PROG_BUTTON_PIN, PROG_LED_PIN, MANUFACTURER_ID, DEVICE_ID, REVISION);
			knx_device_ok = false;
			display.setCursor(0,0);
			display.clearDisplay();
			display.print("KNX keine Spannung!");
			display.display();
		}
		else
			knx_device_ok = Konnekting.init(KNX_SERIAL, PROG_BUTTON_PIN, PROG_LED_PIN, MANUFACTURER_ID, DEVICE_ID, REVISION);

		knx_device_init_attempts++;

		if (!knx_device_ok)
		{
			strip.setPixelColor(0, color_RED); strip.show();
			Debug.print(F("next device init attempt - wait 2s\n"));
			delay(1500);
			strip.setPixelColor(0, 0); strip.show();
			delay(500);
		}
		else
		{
			strip.setPixelColor(0, color_GREEN); strip.show();
		}

	} while (!knx_device_ok && knx_device_init_attempts<5);
	
	if (Konnekting.isFactorySetting() && !Konnekting.isProgState()) // ProgState wenn FactrorySetting
		Konnekting.toggleProgState();

    // If device has been parametrized with KONNEKTING Suite, read params from EEPROM
    // Otherwise continue with global default values from sketch
  	if (!Konnekting.isFactorySetting()) {
		Debug.print(F("!isFactorySetting\n"));
		kmx_para_groups = Konnekting.getUINT8Param(PARAM_kmx_groups);
		kmx_para_offset_CommObjects = Konnekting.getUINT8Param(PARAM_kmx_offset_CommObjects);
		kmx_para_lengh_CommObjects = Konnekting.getUINT8Param(PARAM_kmx_lengh_CommObjects);
		kmx_para_offset_Parameter = Konnekting.getUINT8Param(PARAM_kmx_offset_Parameter);
		kmx_para_lengh_Parameter = Konnekting.getUINT8Param(PARAM_kmx_lengh_Parameters);
		if (Konnekting.isProgState()) // kein ProgState wenn kein FactrorySetting
			Konnekting.toggleProgState();
 	}

	if (Konnekting.isProgState()) {
		Debug.print(F("device in ProgState\n"));
		display.setCursor(0,0);
		display.clearDisplay();
		display.print("device in ProgState");
		display.display();
		strip.setBrightness(80);
  		strip.setPixelColor(0, color_BLUE);
  		strip.show();
	}

  	FreeEepromOffset = Konnekting.getFreeEepromOffset();

  	if (kmx_para_groups > KMX_MAX_GROUPS) {
	    Debug.println(F("kmx_para_groups(%d) greater KMX_MAX_GROUPS(%d)"), kmx_para_groups, KMX_MAX_GROUPS);
	    kmx_para_groups = KMX_MAX_GROUPS;
    }
	
	uint16_t eeOffset = ((FreeEepromOffset/1023)+1)*1023;	// eeOffset - Ab hier steht das EEprom zur Verfügung  (gerundet auf 1023)
	Debug.println(F("FreeEepromOffset(%d)"), FreeEepromOffset);
	Debug.println(F("kmx_storage EepromOffset(%d)"), eeOffset);
	Debug.println(F("kmx EEPROMSTORAGE Storage Size: %d"), sizeof(EEPROMSTORAGE));
	Debug.println(F("kmx ee_group Storage Size: %d"), sizeof(ee_group));

	bool storageOK = false;			// Daten erfolgreich aus EEprom gelesen
	uint8_t writeAttemps = 0;	

	ee_group *p_ee_group = NULL;	// Pointer auf Parameter einer Gruppe

	//kmx_st.start_index = eeOffset;	// EEprom Adresse ab welcher die Szenen-Werte dieser Gruppe gespeichert werden
	//kmx_st.start_index = 0;

	// lesen der Parameter jeder Gruppe
	for (uint8_t grIndex = 0; grIndex < kmx_para_groups ; grIndex++)
	{
		p_ee_group = &kmx_st.store->ee_groups[grIndex];	// Setze Pointer auf Parameter der Gruppe
		p_group = &kmx_gr[grIndex];						// Gruppe-Objekt
		p_group->setStorage(&kmx_st);					// Speicher-Objekt der Gruppe übergeben
		p_group->setIndex(grIndex);						// Gruppenindex setzen

		Debug.println(F("*******************************************************************************"));
		Debug.println(F("Setup Group: (%d)"), grIndex);

		//int ee_gr_index = (int)(&kmx_st.store->ee_groups[grIndex]) - (int)(kmx_st.store);	// Index der Parameter der Gruppe im Speicher
		//int ee_index = eeOffset + ee_gr_index;												// Offset der konnekting-Parameter dazu
		//kmx_st.start_index = ee_index;	// EEprom Adresse ab welcher die Szenen-Werte dieser Gruppe gespeichert werden
	
		storageOK = false;
		writeAttemps = 0;
		
		//fillDefaultSceneParameter(p_ee_group, grIndex);	// zuerst mit default Parametern füllen
		//kmx_st.store->actual_scenes[grIndex] = 1;	// default Szene
		//kmx_st.storage_actualScene.write(grIndex, 1); // default Szene

		while(!storageOK && writeAttemps < 3)
		{
			storageOK = kmx_st.readGroup(grIndex);			// Lesen der Szenen-Parameter aus dem EEprom
			kmx_st.clearStorageLightValues(grIndex, 0);	// Löschen LeuchtenWerte der Szene 0
			kmx_st.clearStorageLightValues(grIndex, 1);	// Löschen LeuchtenWerte der Szene 1
			
			if (storageOK && kmx_st.store->ee_groups[grIndex].chksum != kmx_st.getChksum(grIndex)){
				Debug.println(F("### clearStorageLightValues ######################################################"));
				Debug.println(F("kmx EEprom Storage Chksum not ok - setChksum"));
				kmx_st.setChksum(grIndex);					// Prüfsumme neu berechnen und in die vorgesehene Speicherzelle schreiben
			}
			
			if (!storageOK)	// Eprom Daten konnten nicht exakt gelesen werden Prüfsummen-Fehler!
			{
				if (writeAttemps > 0)
					Debug.println(F("############################################################################"));
				Debug.println(F("kmx EEprom Storage Chksum not ok - fill EEprom default values"));
				fillDefaultSceneParameter(p_ee_group, grIndex);
				fillDefaultSceneValues(p_ee_group, grIndex);
				kmx_st.storage_actualScene.write(grIndex, 1); // default Szene

				strip.setPixelColor(0, color_RED); strip.show();
				delay(500);
				strip.setPixelColor(0, 0); strip.show();
				delay(500);
				
				kmx_st.clearStorageLightValues(grIndex, 0);	// Löschen LeuchtenWerte Szene 0
				kmx_st.clearStorageLightValues(grIndex, 1);	// Löschen LeuchtenWerte Szene 0
				kmx_st.setChksum(grIndex);					// Prüfsumme neu berechnen und in die vorgesehene Speicherzelle schreiben		
				kmx_st.writeGroup(grIndex);						// default - Parameter in EEprom sichern
				writeAttemps++;
			}
		}

		uint8_t lights = 0;			// Anzahl der Leuchten dieser Gruppe
		uint16_t konIndex = 0;		// Konnekting - Index der Gruppen-Parameter

		konIndex = kmx_para_offset_Parameter + grIndex * kmx_para_lengh_Parameter;
		char name[12] = {0};
		Konnekting.getSTRING11Param(konIndex).toCharArray(name, 11, 0); 
		p_group->setName(name);
		konIndex++;

		// Anzahl der Leuchten dieser Gruppe
		lights = Konnekting.getUINT8Param(konIndex); 
		if (lights > KMX_MAX_LIGHTS)
			lights = KMX_MAX_LIGHTS;
		p_group->setLightsCount(lights);
		
		konIndex++;

		// Type der Leuchten none=0, switching=1, dimming=2, rgb=3
		char type[12] = {0};
		Konnekting.getSTRING11Param(konIndex).toCharArray(type, 11, 0);
		konIndex++;

		// DMX-Kanal Nummer der Leuchten
		char dmx_ch[12]= {0};
		Konnekting.getSTRING11Param(konIndex).toCharArray(dmx_ch, 11, 0);
		konIndex++;

		for (int i_Light=0; i_Light < lights; i_Light++)
		{
			char ch = dmx_ch[i_Light];
			
			kmx_light *kl = p_group->getLight(i_Light);
			kl->setType((light_type)type[i_Light]);
			kl->groupIndex = grIndex;
			kl->setDMXChannel(ch);
			
			if (type[i_Light] == switching) {
				kl->setWriteChannelValueToKNXFunc(&writeChannelValueToKNX);
			}

		}
		p_group->sceneEmergemcy = Konnekting.getUINT8Param(konIndex);
		konIndex++;

		// Funktionen zum Senden auf dem KNX-Bus Zuweisen
		Debug.println(F("kmx assign func: %d"), grIndex);
		p_group->setWriteSceneToKNXFunc(&writeSceneToKNX);	// Szene senden
		p_group->setWriteSleepToKNXFunc(&writeSleepToKNX);	// Schlaf-Modus-Senden
		p_group->setWriteMsgToKNXFunc(&writeMsgToKNX);		// Msg-Modus-Senden
		//p_group->setWriteChannelValueToKNXFunc(&writeChannelValueToKNX);	// Channel Senden

		// letzte gespeicherte Szene aktivieren
		if (storageOK)
			p_group->setSceneInit(kmx_st.read_actualScene(grIndex));
	}

	SAMD21DMX.startOutput();
	Debug.println(F("DMX Output is started..."));

	lastmillis = millis();
	Debug.println(F("Setup is ready. go to loop..."));

	display.setCursor(0,0);
	display.clearDisplay();
	display.print("Setup is ready. go to loop...");
	display.display();
	if (!knx_device_ok)
	{
		kmxMenu.setEmergency(true);
		workingState = false;
	}
}

void loop() {
  	// LoopTiming();
	  
	currentmillis = millis();
	currentmicros = micros();
	

	if (workingState || Konnekting.isProgState())
		Knx.task();	// sending/receiving KNX telegrams WARNING: this function shall be called periodically (400us max period
	if (!Konnekting.isProgState())
		kmxMenu.task();

	if (currentmillis - lastmillis >= 1000)
	{
		if (knx_device_ok) {
			if ( digitalRead(SAVEB_PIN) == true) {	// Ausfall KNX-Busspannung
		   		knx_device_ok = false;
				//kmxMenu.getMenu().select(miEmergencyLightingSelect);
		   		kmxMenu.setEmergency(true);
			}
		}
		if (Konnekting.isProgState())
		{
			if (!prgState)
			{
				Debug.print(F("device in ProgState\n"));
				strip.setBrightness(80);
				strip.setPixelColor(0, color_BLUE);
				strip.show();
				prgState = true;
			}
		}
		else
		{	
			prgState = false;
			
			if (!knx_device_ok) {
				if (colorToggle)
					strip.setPixelColor(0, 0);
				else
					strip.setPixelColor(0, color_RED);
				strip.setBrightness(10);
			}
			else if (workingState) {
				if (colorToggle)
					strip.setPixelColor(0, 0);
				else
					strip.setPixelColor(0, color_GREEN);
				strip.setBrightness(5);
			}
			else {
				if (colorToggle)
					strip.setPixelColor(0, 0);
				else
					strip.setPixelColor(0, color_VIOLET);
				strip.setBrightness(10);
			}
		
			
			strip.show();
		}
		colorToggle = !colorToggle;

		seconds++;
		if (!prgState && seconds >= 60)
		{
			seconds = 0;
			int mem =  FreeRam();
			Debug.print(F("DEBUG! free ram: %d\n"), mem);

			if (!knx_device_ok){
				Debug.println(F("init device - (%d) Attempt \n"), knx_device_init_attempts);
				if (digitalRead(SAVEB_PIN) == true)	// Ausfall KNX-Busspannung
					knx_device_ok = false;
				else
					knx_device_ok = Konnekting.init(KNX_SERIAL, PROG_BUTTON_PIN, PROG_LED_PIN, MANUFACTURER_ID, DEVICE_ID, REVISION);

				if (knx_device_ok)
				{
				 	kmxMenu.setEmergency(false);
					//kmxMenu.getMenu().select(miWorking);
					if (Konnekting.isFactorySetting() && !Konnekting.isProgState()) // ProgState wenn FactrorySetting
						Konnekting.toggleProgState();
					else if (!Konnekting.isFactorySetting() && Konnekting.isProgState())
						Konnekting.toggleProgState();
				}
				
				knx_device_init_attempts++;
			}
			else
				knx_device_init_attempts = 0;
		}

		if (!Konnekting.isActive())
			Debug.print(F("Konnekting is not active: \n"));


		lastmillis = currentmillis;
	}

	/*
     * only do measurements and other sketch related stuff if not in programming mode
     * means: only when konnekting is ready for appliction
     */
    if (Konnekting.isReadyForApplication()) 
	{
	
    }

	for (unsigned int ifade=0; ifade<kmx_para_groups; ifade++)
	{
	  	kmx_gr[ifade].verify();	// Gruppen-Szene und Bedingungen jede Sekunde auf Aenderung pruefen
	  	kmx_gr[ifade].update();	// update bewirkt Senden der DMX-Werte
	}

	// if (grFade < kmx_para_groups)	// bei jeden Zyklus eine andere Gruppe Senden
	// {
	// 	p_group = &kmx_gr[grFade];
	// 	p_group->verify();	//Gruppen-Szene und Bedingungen jede Sekunde auf Aenderung pruefen
	// 	p_group->update();	// update bewirkt Senden der DMX-Werte
	// 	grFade++;
	// 	if (grFade >= kmx_para_groups)
	// 		grFade = 0;
	// }
	// LoopTiming();

}

void knxEvents(byte index) {
	int comObjIndex;
	int groupId;
	//kmx_group *p_group;

	if (!Konnekting.isReadyForApplication())
		return;
	// Event einer DMX-Gruppe?
	if (index >= kmx_para_offset_CommObjects && index < (kmx_para_offset_CommObjects+kmx_para_groups*kmx_para_lengh_CommObjects))
	{
		// Ermittle die Gruppen-Nummer
		groupId = (index-kmx_para_offset_CommObjects)/kmx_para_lengh_CommObjects;
		
		// Index des Gruppen-Objektes
		comObjIndex = index - kmx_para_offset_CommObjects - (groupId * kmx_para_lengh_CommObjects);

		p_group = &kmx_gr[groupId];
		//bool writeBit = false;
		int switchIndex = comObjIndex + kmx_para_offset_CommObjects;
		bool tmp = false;
		byte tmpScene = 0;

		switch(switchIndex)
		{
			case COMOBJ_kmx_gr0_scene:
				tmpScene = Knx.read(index);
				p_group->setSceneFromKNX(tmpScene);
				break;
			case COMOBJ_kmx_gr0_switchPM:
				tmp = Knx.read(index);
				p_group->setSwitchPM1(tmp);
				break;
			case COMOBJ_kmx_gr0_switch:
				tmp = Knx.read(index);
				p_group->setKeyMinus(tmp);
				break;
			case COMOBJ_kmx_gr0_szene_plus:
				tmp = Knx.read(index);
				p_group->setKeyPlus(tmp);
				break;
			case COMOBJ_kmx_gr0_switchPM2:
				tmp = Knx.read(index);
				p_group->setSwitchPM2(tmp);
			break;
			case COMOBJ_kmx_gr0_dimming:
				break;
			case COMOBJ_kmx_gr0_nacht:
				tmp = Knx.read(index);
				p_group->setNight(tmp);
				break;
			case COMOBJ_kmx_gr0_panik:
				tmp = Knx.read(index);
				p_group->setKeyPanik(tmp);
				break;
			default:
				Debug.println(F("->Event groupId:(%d %s) id:(%d) value(%d)"), groupId, p_group->getName(), comObjIndex, Knx.read(index));
			break;
		}
		//Debug.println(F("->Event groupId:(%d %s) id:(%d) value(%d)"), groupId, p_group->getName(), comObjIndex, Knx.read(index));
	}
	else
	{
		switch(index)
		{
			case COMOBJ_kmx_msg_rx:	//14char
				Knx.read(index, msgBuffer);
				groupId = (msgBuffer[0] & 0b00001111);	//WcccGGGG	W: 0-Read 1Write; ccc Command:0-LightValues 2-SaveScene 3-ReadRequest; GGGG: GroupNr0-15
				if (groupId >= KMX_MAX_GROUPS)
				{
					Debug.println(F("COMOBJ_kmx_transmission GroupNr out of index: %d KMX_MAX_GROUPS: %d"), groupId, KMX_MAX_GROUPS);
					break;
				}
				p_group = &kmx_gr[groupId];
				p_group->handleMsg(msgBuffer);
				display.setCursor(0,0);
				display.clearDisplay();
				display.println( kmxMenu.getCurrentMenu().getName());
				display.print(kmx_gr[groupId].getName());
				display.print("(");
				display.print(groupId);
				display.print(")\n");
				display.print("handleMsg");
				display.display();
				break;

			default:
				break;
		}
	}
};


void writeSceneToKNX(int group_index, uint8_t scene)
{
	if (knx_device_ok)
	{
		byte comobj_index = COMOBJ_kmx_gr0_scene + kmx_para_lengh_CommObjects* group_index;
		Debug.println(F("write to KNX: Scene(%d) from group(%d %s) "), scene, group_index, kmx_gr[group_index].getName());
		Knx.write(comobj_index, (int)scene);
		display.setCursor(0,0);
		display.clearDisplay();
		display.println( kmxMenu.getCurrentMenu().getName());
		display.print(kmx_gr[group_index].getName());
		display.print("(");
		display.print(group_index);
		display.print(")\n");
		display.print("writeKnx-Szene(");
		display.print(scene);
		display.print(")\n");
		// display.println(("%s(%d)", kmx_gr[group_index].getName(), group_index));
		//display.println(("writeKnx-Szene(%d)", scene));
		//display.getTextBounds()
		display.setCursor(0,4);
		display.display();
	}
}



void writeSleepToKNX(int group_index, bool sleep)
{
	if (knx_device_ok)
	{
		byte comobj_index = COMOBJ_kmx_gr0_schlafen + kmx_para_lengh_CommObjects* group_index;
		Debug.println(F("write to KNX: Sleep(%d) from group(%d %s) "), sleep, group_index, kmx_gr[group_index].getName());
		Knx.write(comobj_index, sleep);
	}
}

void writeMsgToKNX(byte msg[])
{
	if (knx_device_ok)
	{
		//Debug.println(F("write msg to KNX(%s)"), msg);
		Debug.println(F("write msg to KNX"));
		Knx.write(COMOBJ_kmx_msg_tx, msg);
	}
}

void writeChannelValueToKNX(int group_index, uint8_t channel, byte val)
{
	if (knx_device_ok && channel > 0 && channel <= 3)
	{
		byte comobj_index = COMOBJ_kmx_gr0_knx1 + kmx_para_lengh_CommObjects* group_index+(channel-1);
		Debug.println(F("----> switch knx (%d) to (%d) group(%d %s) "), comobj_index, val, group_index, kmx_gr[group_index].getName());
		Knx.write(comobj_index, (bool)val);
	}
}

void manualScene(ManualSceneEvent sceneEvent)
{
	Debug.print(F("ManualSceneEvent gr(%d) sz(%d) (%s)\n"), sceneEvent.groupIncDec, sceneEvent.sceneIncDec, sceneEvent.current->getName());

	display.clearDisplay();
	display.setCursor(0,0);
	display.println(sceneEvent.current->getName());

	if (strcmp(sceneEvent.current->getName(), miWorking.getName()) == 0) {
		Debug.println(F("miWorking"));
		workingState = true;
		for (unsigned int iGr=0; iGr<kmx_para_groups; iGr++) {
			kmx_gr[iGr].setContinous(false);
			kmx_gr[iGr].setEmergencyScene(false);
			//kmx_gr[iGr].setSceneInit(kmx_st.read_actualScene(iGr));
		}
	}
	else {
		workingState = false;
	}

	if (strcmp(sceneEvent.current->getName(), miManualGroupSelect.getName()) == 0) {
		// Debug.println(F("miManualGroupSelect"));
		
		if (sceneEvent.groupIncDec != 0) {
			manGroup += sceneEvent.groupIncDec;
			if (manGroup >= kmx_para_groups)
				manGroup = 0;

		}
		display.print(kmx_gr[manGroup].getName()); display.print("("); display.print(manGroup); display.print(")\n");
	}

	else if (strcmp(sceneEvent.current->getName(), miManualSceneSelect.getName()) == 0){
		// Debug.println(F("miManualScene"));
		if (sceneEvent.sceneIncDec != 0) {
			manScene += sceneEvent.sceneIncDec;
			if (manScene > kmx_st.store->ee_groups[manGroup].used_scenes)
				manScene = 0;
		}
		display.print(kmx_gr[manGroup].getName()); display.print("("); display.print(manGroup); display.print(")\n");
		display.print("Szene("); display.print(manScene); display.print(")\n");
	}
	else if (strcmp(sceneEvent.current->getName(), miManualSceneActivate.getName()) == 0){
		// Debug.println(F("miManualSceneActivate"));
		kmx_gr[manGroup].setContinous(true);
		kmx_gr[manGroup].setScene(manScene);
	}

	else if (strcmp(sceneEvent.current->getName(), miEmergencyLightingSelect.getName()) == 0){
		Debug.println(F("miEmergencyLightingSelect"));
		for (unsigned int iGr=0; iGr<kmx_para_groups; iGr++){
			kmx_gr[iGr].setEmergencyScene(true);
		}
	}
	else if (strcmp(sceneEvent.current->getName(), miConfigProgStateSelect.getName()) == 0){
		Debug.println(F("miConfigProgStateSelect"));
		Konnekting.toggleProgState();
		kmxMenu.getMenu().moveLeft();
	}

	display.display();
}

/*********************************************************************************
** LoopTiming() v06 by GuntherB & Doc_Arduino @ german Arduino Forum        	**
**********************************************************************************
** Funktion um die Dauer der Loop-Zeiten zu ermitteln							**
** wird in der Loop am Anfang und Ende aufgerufen								**
** benötigt ca (AL * 4 + 16) Byte RAM											**
*********************************************************************************/

void LoopTiming()
{
  const int AL = 1000;        // Arraylänge, NUR GERADE Zahlen verwenden!
  static unsigned long LoopTime[AL+2];
  static unsigned int Index=0, Messung=0, Min=0xFFFF, Max, Avg;
  
  if (Messung % 2 == 0)     // wenn Messung X gerade (0,2,4,6 usw.), entspricht immer Anfang der Loop
    {
     LoopTime[Index] = micros();
     Messung++;
     Index++;    
     return;	            // Funktion sofort beenden, spart bestimmt Zeit
    }

  if (Messung % 2 == 1)     // wenn Messung X ungerade (1,3,5,7 usw.), entspricht immer Ende der Loop
    {
     LoopTime[Index] = micros();
     LoopTime[Index-1] = LoopTime[Index] - LoopTime[Index-1];  	// Loopdauer einen Index niedriger einspeichern wie aktuell
     Messung++;
    }	
	    
	if (Index >= AL) 	// Array voll, Daten auswerten
	{  
		for (int i = 0; i<AL; i++)
		{
		Min = min(Min, LoopTime[i]);
		Max = max(Max, LoopTime[i]);
		Avg += LoopTime[i];
		}

		Avg = Avg / AL;
		if (Max >= 400)
		{
			Debug.println(F("Minimal:      %d us"), Min);
			Debug.println(F("Durchschnitt: %d us"), Avg);
			Debug.println(F("Maximal:      %d us"), Max);
		}
		Min = 0xFFFF;
		Max = 0;
		Avg = 0;
		Messung = 0;
		Index = 0;
    }
}



/*
     This is an important function
     Here all use events are handled
     
     This is where you define a behaviour for a menu item
*/
// void menuUseEvent(MenuUseEvent used)
// {
// 	Debug.println(F("Menu use "));
// 	Debug.println(F(used.item.getName()));
// 	if (used.item.isEqual(miManualGroup))
// 	{
// 		Debug.println(F("Raum wählen"));
// 	}
// 	if (used.item.isEqual(miManualScene))
// 	{
// 		Debug.println(F("Szene wählen"));
// 	}
// }

/*
     This is an important function
     Here we get a notification whenever the user changes the menu
     That is, when the menu is navigated
*/
// void menuChangeEvent(MenuChangeEvent changed)
// {

// 	Debug.print(F("Menu: "));
// 	Debug.println(F(changed.to.getName()));
// 	//Debug.println(F(changed.from.getName()));
// 	//Debug.println(F(" "));
	
// }