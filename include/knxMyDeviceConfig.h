/*
 * knxMyDeviceConfig.h
 *
 * Created: 18.02.2017 23:13:26
 *  Author: Ivan
 */ 


#ifndef KNXMYDEVICECONFIG_H_
#define KNXMYDEVICECONFIG_H_

#include "Arduino.h"
#include <KnxDevice.h>

//uint8_t kmx_para_groups = 15; // Anzahl der Beleuchtungs-Gruppen
//uint8_t kmx_para_offset_CommObjects = 20; // Startindex der Gruppen-CommObjekte
//uint8_t kmx_para_lengh_CommObjects = 15; // Anzahl der Gruppen-CommObjekte
//uint8_t kmx_para_offset_Parameter = 10; // Startindex der Gruppen-Parameter
//uint8_t kmx_para_lengh_Parameter = 10; // Anzahl der Gruppen-Parameter

typedef struct para_group {
	char name[12] = {0};
	uint8_t lightsCount = 0;
	light_type lt[KMX_MAX_LIGHTS] = {none};
	uint8_t channels[KMX_MAX_LIGHTS] = {0};
};
//para_group para_groups[KMX_MAX_GROUPS] =
//{
	//{"Eltern", 4, {dimming, dimming, dimming, dimming}, {0x1C, 0x1D, 0x2F, 0x30}}
//};

// Definition of the Communication Objects attached to the device
//KnxComObject KnxDevice::_comObjectsList[] =
//{
	//KnxComObject(G_ADDR(2,2,255), KNX_DPT_16_000, 0x2a) ,
	//KnxComObject(G_ADDR(2,2,10),KNX_DPT_17_001, 0x2a),	//scene
	//KnxComObject(G_ADDR(2,2,13),KNX_DPT_1_001, 0x2a),	//switchPM
	//KnxComObject(G_ADDR(2,2,14),KNX_DPT_1_001, 0x2a),	//switchPM2
	//KnxComObject(G_ADDR(6,2,13),KNX_DPT_1_001, 0x2a),	//switchMinus
	//KnxComObject(G_ADDR(6,2,14),KNX_DPT_1_001, 0x2a),	//switchPlus
	//KnxComObject(G_ADDR(2,2,11),KNX_DPT_1_001, 0x2a),	//nacht
	//KnxComObject(G_ADDR(2,2,12),KNX_DPT_1_001, 0x2a),	//schlafen
//};


#endif /* KNXMYDEVICECONFIG_H_ */