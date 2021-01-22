#ifndef KMX_MENU_h
#define KMX_MENU_h

#include "Arduino.h"
#include <Bounce2.h>
#include <MenuBackend.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// enum status : uint8_t
// {
// 	working = 0,
// 	setGroup = 1,
// 	setScene = 2,
//     activateScene = 3,
//     emergencyLighting= 4,
// };

//beneath is list of menu items needed to build the menu
static MenuItem miWorking = MenuItem("Normalbetrieb\n");
    static MenuItem miManualGroup = MenuItem("Handbetrieb\nRaum&Szene waehlen");
        static MenuItem miManualGroupSelect = MenuItem("Handbetrieb\nRaum waehlen");
            static MenuItem miManualSceneSelect = MenuItem("Handbetrieb\nSzene waehlen");
                static MenuItem miManualSceneActivate = MenuItem("Handbetrieb\nSzene aktivieren");
static MenuItem miEmergencyLighting = MenuItem("Notbeleuchtung Ein");
    static MenuItem miEmergencyLightingSelect = MenuItem("Notbeleuchtung aktiv");
static MenuItem miConfig = MenuItem("Einstellungen");
    static MenuItem miConfigProgState = MenuItem("Programmiermodus waehlen");
        static MenuItem miConfigProgStateSelect = MenuItem("Programmiermodus");

//static status currentStatusMode;

struct ManualSceneEvent
{
  const int8_t groupIncDec;
  const int8_t sceneIncDec;
  MenuItem *current;
  //bool use = false;
};

typedef void (*cb_scene)(ManualSceneEvent);


class kmx_menu {

    public:
        kmx_menu(const u_int8_t pinUp, const u_int8_t pinDown, Adafruit_SSD1306 display, cb_scene manualScene = 0);
        void task();
        MenuItem getCurrentMenu();
        MenuBackend getMenu();
        void setEmergency(bool to);
        void writeLine(uint8_t line, char *str);
        void writeLCD();

    private:
        Bounce buttonUp = Bounce(); 
        Bounce buttonDown = Bounce(); 
        uint8_t twoButtons = 0;
        uint8_t gruppe = 0;
        uint8_t szene = 0;

        bool Emergency = false;
        bool MenuSet = false;

        char lcdBuf [20][8];
        Adafruit_SSD1306 _display;

        void menuSetup();
        //this controls the menu backend and the event generation

        static void menuUseEvent(MenuUseEvent used);
        static void menuChangeEvent(MenuChangeEvent changed);

        MenuBackend menu = MenuBackend(menuUseEvent, menuChangeEvent);

        cb_scene cb_manualScene;

        

        
};

#endif