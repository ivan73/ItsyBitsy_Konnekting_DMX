/* 
* kmx_scene.h
*
* Created: 25.12.2016 09:32:54
* Author: Ivan
*/


#ifndef __KMX_SCENE_H__
#define __KMX_SCENE_H__

#include <Arduino.h>

class kmx_scene
{
//variables
public:
protected:
private:

//functions
public:
	//kmx_scene();
	//~kmx_scene();
	bool update();
protected:
private:
	//kmx_scene( const kmx_scene &c );
	//kmx_scene& operator=( const kmx_scene &c );

	uint16_t _activeLights = 0;		// welche Leuchten werden durch die Szene geändert xxxxxxxxxxxx4321
	uint16_t _dimableLights = 0;	// welche Leuchten können in dieser Szene gedimmt werden xxxxxxxxxxxx4321
	uint16_t _uebergangszeit = 2;	// Zeit (s) nach welcher alle aktiven Leuchten der Szene den Endwert erreichen

}; //kmx_scene

#endif //__KMX_SCENE_H__
