#include "kmx_menu.h"
#include "DebugUtil.h"

#define MENU_EEPROM_DEBUG

kmx_menu::kmx_menu(const u_int8_t pinUp, const u_int8_t pinDown, Adafruit_SSD1306 display, cb_scene manualScene)
{
    cb_manualScene = manualScene;

	_display = display;
	
	pinMode(pinUp,INPUT_PULLUP);
	buttonUp.attach(pinUp);
	buttonUp.interval(20); // interval in ms
	
	pinMode(pinDown,INPUT_PULLUP);
	buttonDown.attach(pinDown);
	buttonDown.interval(20); // interval in ms

    menuSetup();
}



void kmx_menu::menuSetup()
{
	menu.getRoot().add(miWorking);
  	miWorking.addAfter(miManualGroup).addAfter(miEmergencyLighting).addAfter(miConfig);
  	miManualGroup.addRight(miManualGroupSelect).addRight(miManualSceneSelect).addRight(miManualSceneActivate);
	miEmergencyLighting.addRight(miEmergencyLightingSelect);
	miConfig.addRight(miConfigProgState).addRight(miConfigProgStateSelect);
}

void kmx_menu::task()
{
    buttonUp.update();
    buttonDown.update();

	if (MenuSet) {	// Menu auserhalb der Klasse gewä
		if (Emergency) 	
			menu.select(miEmergencyLightingSelect);
		else
			menu.select(miWorking);
		
		if (cb_manualScene) {
			(*cb_manualScene)({0,0,&menu.getCurrent()});
		}
		MenuSet = false;
	}
	


	

	if (strcmp(menu.getCurrent().getName(), menu.getRoot().getName()) == 0) {
		menu.select(miWorking);
		if (cb_manualScene) 		{
			(*cb_manualScene)({0,0,&menu.getCurrent()});
		}
	}
	else if (strcmp(menu.getCurrent().getName(), miManualSceneActivate.getName()) == 0) {
		menu.select(miManualGroupSelect);
		if (cb_manualScene)
		{
			(*cb_manualScene)({0,0,&menu.getCurrent()});
		}
	}


    if (buttonUp.fell() && !buttonDown.read())
    	twoButtons = 1;

    if (buttonDown.fell() && !buttonUp.read())
    	twoButtons = 1;

    if (buttonUp.rose())	// UP Taste losgelassen
    {
    	//Debug.print(F("buttonUp.Losgelassen\n"));
    	if (twoButtons == 1 && !buttonDown.read())
    		twoButtons = 2;
    	else if (twoButtons == 2)	// zwei Tasten
    	{
    		// Debug.print(F("zwei Tasten %d \n" ),buttonUp.previousDuration());
    		if (buttonUp.previousDuration() > 750 )	// Zwei Tasten lange
    		{
    			// Debug.print(F("doppel Lang\n"));
				menu.moveRight();
				if (cb_manualScene) {
					(*cb_manualScene)({0,0, &menu.getCurrent()});
				}
    		}
    		else									// Zwei Tasten kurz
    		{
    			// Debug.print(F("doppel Kurz\n"));
				menu.moveLeft();
				if (cb_manualScene) {
					(*cb_manualScene)({0,0, &menu.getCurrent()});
				}
    		}
    		twoButtons = 0;
    	}
    	else	// nur UP-Taste losgelassen
        {
			if (strcmp(menu.getCurrent().getName(), miWorking.getName()) != 0)	//miWorking
				menu.moveUp();
			
			if (strcmp(menu.getCurrent().getName(), miManualGroupSelect.getName()) == 0) {	//(menu.getCurrent().isEqual(miManualGroupSelect)) {
				if (cb_manualScene) {
					(*cb_manualScene)({-1,0, &menu.getCurrent()});
				}
			}
			else if (strcmp(menu.getCurrent().getName(), miManualSceneSelect.getName()) == 0) {	//miManualSceneSelect
				if (cb_manualScene) { 
					(*cb_manualScene)({0,-1, &menu.getCurrent()}); 
				}
			}
			else if (cb_manualScene) {
				(*cb_manualScene)({0,0, &menu.getCurrent()}); 
			}

        }
    }

    if (buttonDown.rose())		// DOWN-Taste losgelassen
    {
    	//Debug.print(F("buttonDown.Losgelassen\n"));
    	if (twoButtons == 1 && !buttonUp.read())
    		twoButtons = 2;
    	else if (twoButtons == 2)
    	{
    		// Debug.print(F("zwei Tasten %d \n" ),buttonDown.previousDuration());
    		if (buttonDown.previousDuration() > 750 )	// Zwei Tasten lange
    		{
				// Debug.print(F("doppel Lang\n"));
				menu.moveRight();
				if (cb_manualScene) 
				{ 
					(*cb_manualScene)({0,0, &menu.getCurrent()});
				}
    		}
    		else										// Zwei Tasten kurz
    		{
    			// Debug.print(F("doppel Kurz\n"));
				menu.moveLeft();
				if (cb_manualScene) { 
					(*cb_manualScene)({0,0, &menu.getCurrent()});
				}
    		}
    		twoButtons = 0;
    	}
    	else	//nur DOWN-Taste losgelassen
        {    
			menu.moveDown();
			
			if (strcmp(menu.getCurrent().getName(), miManualGroupSelect.getName()) == 0) {	//(menu.getCurrent().isEqual(miManualGroupSelect)) {
				if (cb_manualScene) {
					(*cb_manualScene)({1,0, &menu.getCurrent()});
				}
			}
			else if (strcmp(menu.getCurrent().getName(), miManualSceneSelect.getName()) == 0) {	//miManualSceneSelect
				if (cb_manualScene) { 
					(*cb_manualScene)({0,1, &menu.getCurrent()}); 
				}
			}
			else if (cb_manualScene) {
				(*cb_manualScene)({0,0, &menu.getCurrent()});
			}
        }
    }	
}

MenuItem kmx_menu::getCurrentMenu()
{
	return menu.getCurrent();
}

MenuBackend kmx_menu::getMenu()
{
	return menu;
}

void kmx_menu::setEmergency(bool to)
{
	Emergency = to;
	MenuSet = true;
}



/*
     This is an important function
     Here all use events are handled
     
     This is where you define a behaviour for a menu item
*/
void kmx_menu::menuUseEvent(MenuUseEvent used)
{
	Debug.println(F("Menu use "));
	Debug.println(F(used.item.getName()));

}

/*
     This is an important function
     Here we get a notification whenever the user changes the menu
     That is, when the menu is navigated
*/
void kmx_menu::menuChangeEvent(MenuChangeEvent changed)
{
	//Debug.print(F("Menu: "));
	//Debug.println(F(changed.to.getName()));
}


// void kmx_menu::writeLine(uint8_t line, char *str)
// {
// 	//strcpy(&lcdBuf[0][line], str);

// 	//char buf2[32];
// 	snprintf(&lcdBuf[0][line], 20, str);

// }

// void kmx_menu::writeLCD()
// {
// 	// _display.setCursor(0,0);
// 	// _display.clearDisplay();
// 	// for (int i = 0; i<=20; i++)
// 	// {
// 	// 	_display.println(lcdBuf[i][0]);
// 	// }
// 	// _display.display();
// }