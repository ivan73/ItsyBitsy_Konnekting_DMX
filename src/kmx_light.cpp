/*
 * kmx_light.cpp
 *
 * Created: 24.12.2016 08:11:59
 *  Author: Ivan
 */ 

#include "kmx_light.h"
#include "DebugUtil.h"
#include "kmx_fader.h"

void kmx_light::update()
{
	switch(_type)
	{
		case dimming:
			_RGBWfader[0].update();
			break;
		case rgb:
			_RGBWfader[0].update();
			_RGBWfader[1].update();
			_RGBWfader[2].update();
			break;
		case rgbw:
			_RGBWfader[0].update();
			_RGBWfader[1].update();
			_RGBWfader[2].update();
			_RGBWfader[3].update();
			break;
		default:
			break;

	}
}

void kmx_light::setType(light_type type)
{
	_type = type;
}
 
light_type kmx_light::getType(void)
{
	return _type;
}


void kmx_light::setDMXChannel(uint8_t channel)
{
	_dmx_channel = channel;
	switch (_type)
	{
	case dimming:
		_RGBWfader[0].set_channel(_dmx_channel);
		break;
	case rgb:
		_RGBWfader[0].set_channel(_dmx_channel);
		_RGBWfader[1].set_channel(_dmx_channel+1);
		_RGBWfader[2].set_channel(_dmx_channel+2);
		break;
	case rgbw:
		_RGBWfader[0].set_channel(_dmx_channel);
		_RGBWfader[1].set_channel(_dmx_channel+1);
		_RGBWfader[2].set_channel(_dmx_channel+2);
		_RGBWfader[3].set_channel(_dmx_channel+3);
		break;
	case switching:
		_knxChannel = channel;
		break;
	default:
		break;
	}
 }

 void kmx_light::initRGBW(uint8_t rgbw_value[4])
 {
	 _rgbw= rgbw_value;
	 switch (_type)
	 {
		 case dimming:
			_RGBWfader[0].set_value(rgbw_value[3]);
			DEBUG_PRINTLN(F("---->dimming ch (%d) setValue to (%d) "), _dmx_channel, rgbw_value[3]);
			break;
		 case rgb:
			 _RGBWfader[0].set_value(rgbw_value[0]);
			 _RGBWfader[1].set_value(rgbw_value[1]);
			 _RGBWfader[2].set_value(rgbw_value[2]);
			DEBUG_PRINTLN(F("---->rgb setValue ch (%d) to (%d %d %d) "), _dmx_channel, rgbw_value[0], rgbw_value[1], rgbw_value[2]);
			 break;
		case rgbw:
		 	_RGBWfader[0].set_value(rgbw_value[3]);
		 	_RGBWfader[1].set_value(rgbw_value[0]);
		 	_RGBWfader[2].set_value(rgbw_value[1]);
			_RGBWfader[3].set_value(rgbw_value[2]);
		 	DEBUG_PRINTLN(F("---->rgbw setValue ch (%d) to (%d %d %d %d)"), 
				_dmx_channel, rgbw_value[0], rgbw_value[1], rgbw_value[2], rgbw_value[3]);
		 	break;
		 case switching:
		 	writeChannelValueToKNXFunc(groupIndex, _knxChannel, rgbw_value[3]);
			break;
		 
		 default:
		 break;
	 }
 }

 void kmx_light::setRGBW(uint8_t rgbw_value[4])
 {
	 _rgbw= rgbw_value;
	 switch (_type)
	 {
		 case dimming:
			_RGBWfader[0].fade(rgbw_value[3], _fadeTime);
			DEBUG_PRINTLN(F("---->dimming ch (%d) fade to (%d) fadeTime (%d)"), _dmx_channel, rgbw_value[3], _fadeTime);
			break;
		 case rgb:
			 _RGBWfader[0].fade(rgbw_value[0], _fadeTime);
			 _RGBWfader[1].fade(rgbw_value[1], _fadeTime);
			 _RGBWfader[2].fade(rgbw_value[2], _fadeTime);
			DEBUG_PRINTLN(F("---->rgb fade ch (%d) to (%d %d %d) fadeTime (%d)"), _dmx_channel, rgbw_value[0], rgbw_value[1], rgbw_value[2], _fadeTime);
			 break;
		case rgbw:
		 	_RGBWfader[0].fade(rgbw_value[3], _fadeTime);
		 	_RGBWfader[1].fade(rgbw_value[0], _fadeTime);
		 	_RGBWfader[2].fade(rgbw_value[1], _fadeTime);
			_RGBWfader[3].fade(rgbw_value[2], _fadeTime);
		 	DEBUG_PRINTLN(F("---->rgbw fade ch (%d) to (%d %d %d %d) fadeTime (%d)"), 
				_dmx_channel, rgbw_value[0], rgbw_value[1], rgbw_value[2], rgbw_value[3], _fadeTime);
		 	break;
		 case switching:
		 	writeChannelValueToKNXFunc(groupIndex, _knxChannel, rgbw_value[3]);
			break;
		 
		 default:
		 break;
	 }
 }

 void kmx_light::setFadeTime(unsigned int fadeTime)
 {
	 _fadeTime = fadeTime;
 }

 unsigned int kmx_light::getFadeTime(void)
 {
	 return _fadeTime;
 }

void kmx_light::setWriteChannelValueToKNXFunc(void (*func)(int, uint8_t, byte)) {
	writeChannelValueToKNXFunc = func;
}
   