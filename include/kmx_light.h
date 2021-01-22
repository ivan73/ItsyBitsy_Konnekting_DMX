/*
 * kmx_light.h
 *
 * Created: 24.12.2016 08:11:33
 *  Author: Ivan
 */ 


#ifndef KMX_LIGHT_H_
#define KMX_LIGHT_H_

#include "Arduino.h"
#include "kmx_fader.h"

enum light_type : uint8_t
{
	none = 0,
	switching = 1,
	dimming = 2,
	rgb = 3,
	rgbw = 4
	//knx_dp1 = 5
};

const uint8_t light_typesBytes[] PROGMEM = {
	0,	//none
	1,	//switching
	1,	//dimming
	3,	//rgb
	4	//rgbw
	//1	//knx_dp1
};

class kmx_light
{
	public:
		uint8_t rgbw_off[4] = {0};
		int groupIndex;
		
		void update();
		void setType(light_type type);
		light_type getType(void);
		void setDMXChannel(uint8_t channel);
		void initRGBW(uint8_t rgbw_value[4]);
		void setRGBW(uint8_t rgbw_value[4]);
		void setFadeTime(unsigned int fadeTime);
		unsigned int getFadeTime(void);

		void setWriteChannelValueToKNXFunc(void (*func)(int, uint8_t, byte));

	private:
		//char _name[12];
		//kmx_fader _fader;
		kmx_fader _RGBWfader[4];
		u_int8_t _knxChannel= 0;	// knx-Schaltkanal-Nr. (1,2,3)
		light_type _type = none;
		bool _state = false;
		uint8_t *_rgbw = {NULL};
		uint8_t _dmx_channel = 0;
		unsigned int _fadeTime = 2000;

		void (*writeChannelValueToKNXFunc)(int, uint8_t, byte);
		
};

#endif /* KMX_LIGHT_H_ */