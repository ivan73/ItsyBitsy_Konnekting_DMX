/*
 * kmx_defaultSzenes.h
 *
 * Created: 04.02.2017 09:37:57
 *  Author: Ivan
 */ 

 

#ifndef KMX_DEFAULTSZENES_H_
#define KMX_DEFAULTSZENES_H_

enum myGroups {
	Eltern = 0,
	Gaeste = 1,
	Carla = 2,
	Mattia = 3,
	Wohnzimmer = 4,
	Essen = 5,
	Kochen = 6,
	Speiss = 7,
	Bad = 8,
	Flur = 9,
	Buero = 10,
	TWC = 11,
	Umkleide = 12,
	Terasse = 13,
	Treppenhaus  = 14
};

void fillDefaultSceneParameter(ee_group * st, uint8_t grNr)
{
	switch (grNr)
	{
		case Eltern:
			//st->actual_scene = 1;
			st->used_scenes = 7;
			st->scenePMnight = 2;
			st->scenePM = 3;
			st->sceneMinusLong = 0;
			st->scenePlusLong = 4;
			st->scenePanic = 7;
			break;
		case Carla:
		case Mattia:
			// st->actual_scene = 1;
			st->used_scenes = 8;
			st->scenePMnight = 2;
			st->scenePM = 3;
			st->sceneMinusLong = 0;
			st->scenePlusLong = 4;
			st->scenePanic = 8;
			break;
		case Gaeste:
			// st->actual_scene = 1;
			st->used_scenes = 5;	
			st->scenePMnight = 2;
			st->scenePM = 3;
			st->sceneMinusLong = 0;
			st->scenePlusLong = 4;
			st->scenePanic = 5;
		break;
		case Buero:
			// st->actual_scene = 1;
			st->used_scenes = 5;
			st->scenePMnight = 2;
			st->scenePM = 3;
			st->sceneMinusLong = 5;
			st->scenePlusLong = 4;
			st->scenePanic = 5;
			break;
		case Wohnzimmer:
			// st->actual_scene = 1;
			st->used_scenes = 5;
			st->scenePMnight = 2;
			st->scenePM = 3;
			st->sceneMinusLong = 5;
			st->scenePlusLong = 4;
			st->scenePanic = 5;
			break;
		case Essen:
			// st->actual_scene = 1;
			st->used_scenes = 6;
			st->scenePMnight = 2;
			st->scenePM = 3;
			st->sceneMinusLong = 5;
			st->scenePlusLong = 4;
			st->scenePanic = 6;
			break;
		case Flur:
			// st->actual_scene = 1;
			st->used_scenes = 5;
			st->scenePMnight = 2;
			st->scenePM = 3;
			st->sceneMinusLong = 5;
			st->scenePlusLong = 4;
			st->scenePanic = 5;
			break;
		case Bad:	//16 17 15 35 36 PanelBadewanne
			//0 Aus PM gesperrt
			//1 Aus
			// st->actual_scene = 1;
			st->used_scenes = 7;
			st->scenePMnight = 2;
			st->scenePM = 3;
			st->sceneMinusLong = 5;
			st->scenePlusLong = 6;
			st->scenePanic = 7;
			break;
		case Umkleide:
			// st->actual_scene = 1;
			st->used_scenes = 5;
			st->scenePMnight = 2;
			st->scenePM = 3;
			st->sceneMinusLong = 5;
			st->scenePlusLong = 4;
			st->scenePanic = 5;
			break;
		case TWC:
			// st->actual_scene = 1;
			st->used_scenes = 6;
			st->scenePMnight = 2;
			st->scenePM = 3;
			st->sceneMinusLong = 5;
			st->scenePlusLong = 4;
			st->scenePanic = 5;
			break;
		case Treppenhaus:
			// st->actual_scene = 1;
			st->used_scenes = 5;
			st->scenePMnight = 2;
			st->scenePM = 3;
			st->sceneMinusLong = 5;
			st->scenePlusLong = 4;
			st->scenePanic = 5;
		break;
		case Terasse:
			// st->actual_scene = 1;
			st->used_scenes = 5;
			st->scenePMnight = 2;
			st->scenePM = 3;
			st->sceneMinusLong = 5;
			st->scenePlusLong = 4;
			st->scenePanic = 5;
		break;
		case Speiss:
			// st->actual_scene = 1;
			st->used_scenes = 5;
			st->scenePMnight = 2;
			st->scenePM = 3;
			st->sceneMinusLong = 5;
			st->scenePlusLong = 4;
			st->scenePanic = 5;
		break;
		default:
			// st->actual_scene = 1;
			st->used_scenes = 5;
			st->scenePMnight = 2;
			st->scenePM = 3;
			st->sceneMinusLong = 5;
			st->scenePlusLong = 4;
			st->scenePanic = 5;
		break;
	}
}

void fillDefaultSceneValues(ee_group * st, uint8_t grNr)
{
	//kmx_scene sc = new kmx_scene;
	int szene = 0;
	switch (grNr)
	{
		case Eltern:
			szene = 0;	//AUS Schlafen
			st->scenes[szene].activeLighs = 0b11111111; st->scenes[szene].dimmableLights = 0b11111111; st->scenes[szene].offDelay = 0;
			
			szene = 1;	//AUS 
			st->scenes[szene].activeLighs = 0b11111111; st->scenes[szene].dimmableLights = 0b11111111; st->scenes[szene].offDelay = 120;
			szene = 2;	//10 PM_Nacht
			st->scenes[szene].activeLighs = 0b11111111; st->scenes[szene].dimmableLights = 0b11111111; st->scenes[szene].offDelay = 120;
			st->scenes[szene].lightValues[0].W = 2;
			szene = 3;	//11 PM_Tag
			st->scenes[szene].activeLighs = 0b11111111; st->scenes[szene].dimmableLights = 0b11111111; st->scenes[szene].offDelay = 120;
			st->scenes[szene].lightValues[0].W = 10;
			szene = 4;
			st->scenes[szene].activeLighs = 0b11111111; st->scenes[szene].dimmableLights = 0b11111111; st->scenes[szene].offDelay = 120;
			st->scenes[szene].lightValues[0].W = 50;
			szene = 5;
			st->scenes[szene].activeLighs = 0b11111111; st->scenes[szene].dimmableLights = 0b11111111; st->scenes[szene].offDelay = 120;
			st->scenes[szene].lightValues[0].W = 100;
			szene = 6;
			st->scenes[szene].activeLighs = 0b11111111; st->scenes[szene].dimmableLights = 0b11111111; st->scenes[szene].offDelay = 120;
			st->scenes[szene].lightValues[0].W = 50;
			szene = 7;
			st->scenes[szene].activeLighs = 0b11111111; st->scenes[szene].dimmableLights = 0b11111111; st->scenes[szene].offDelay = 120;
			st->scenes[szene].lightValues[0].W = 100;
			break;
		case Carla:
		case Mattia:
			szene = 2;
			st->scenes[szene].activeLighs = 0b11111111; st->scenes[szene].dimmableLights = 0b11111111; st->scenes[szene].offDelay = 600; 
			st->scenes[szene].lightValues[0].W = 5;
			st->scenes[szene].lightValues[1].W = 20;
			st->scenes[szene].lightValues[2].W = 0;
			st->scenes[szene].lightValues[2].R = 0; st->scenes[szene].lightValues[2].G = 0; st->scenes[szene].lightValues[2].B = 20;
			szene = 3;
			st->scenes[szene].activeLighs = 0b11111111; st->scenes[szene].dimmableLights = 0b11111111; st->scenes[szene].offDelay = 600;
			st->scenes[szene].lightValues[0].W = 10;
			st->scenes[szene].lightValues[1].W = 50;
			st->scenes[szene].lightValues[2].W = 25;
			st->scenes[szene].lightValues[2].R = 0; st->scenes[szene].lightValues[2].G = 0; st->scenes[szene].lightValues[2].B = 50;
			szene = 4;
			st->scenes[szene].activeLighs = 0b11111111; st->scenes[szene].dimmableLights = 0b11111111; st->scenes[szene].offDelay = 600;
			st->scenes[szene].lightValues[0].W = 100;
			st->scenes[szene].lightValues[1].W = 100;
			st->scenes[szene].lightValues[2].W = 25;
			st->scenes[szene].lightValues[2].R = 0; st->scenes[szene].lightValues[2].G = 0; st->scenes[szene].lightValues[2].B = 50;
			szene = 5;
			st->scenes[szene].activeLighs = 0b11111111; st->scenes[szene].dimmableLights = 0b11111111; st->scenes[szene].offDelay = 600;
			st->scenes[szene].lightValues[0].W = 150;
			st->scenes[szene].lightValues[1].W = 150;
			st->scenes[szene].lightValues[2].W = 0;
			st->scenes[szene].lightValues[2].R = 0; st->scenes[szene].lightValues[2].G = 0; st->scenes[szene].lightValues[2].B = 75;
			szene = 6;
			st->scenes[szene].activeLighs = 0b11111111; st->scenes[szene].dimmableLights = 0b11111111; st->scenes[szene].offDelay = 600;
			st->scenes[szene].lightValues[0].W = 150;
			st->scenes[szene].lightValues[1].W = 150;
			st->scenes[szene].lightValues[2].W = 0;
			st->scenes[szene].lightValues[2].R = 0; st->scenes[szene].lightValues[2].G = 0; st->scenes[szene].lightValues[2].B = 75;
			szene = 7;
			st->scenes[szene].activeLighs = 0b11111111; st->scenes[szene].dimmableLights = 0b11111111; st->scenes[szene].offDelay = 600;
			st->scenes[szene].lightValues[0].W = 150;
			st->scenes[szene].lightValues[1].W = 150;
			st->scenes[szene].lightValues[2].W = 0;
			st->scenes[szene].lightValues[2].R = 0; st->scenes[szene].lightValues[2].G = 0; st->scenes[szene].lightValues[2].B = 75;
			szene = 8;
			st->scenes[szene].activeLighs = 0b11111111; st->scenes[szene].dimmableLights = 0b11111111; st->scenes[szene].offDelay = 600;
			st->scenes[szene].lightValues[0].W = 150;
			st->scenes[szene].lightValues[1].W = 150;
			st->scenes[szene].lightValues[2].W = 0;
			st->scenes[szene].lightValues[2].R = 0; st->scenes[szene].lightValues[2].G = 0; st->scenes[szene].lightValues[2].B = 75;
			break;
			//szene = 9;
			//st->scenes[szene].activeLighs = 0b11111111; st->scenes[szene].dimmableLights = 0b11111111; st->scenes[szene].offDelay = 600;
			//st->scenes[szene].lightValues[0].W = 150;
			//st->scenes[szene].lightValues[1].W = 150;
			//st->scenes[szene].lightValues[2].W = 0;
			//st->scenes[szene].lightValues[2].R = 0; st->scenes[szene].lightValues[2].G = 0; st->scenes[szene].lightValues[2].B = 75;
			//szene = 10;
			//st->scenes[szene].activeLighs = 0b11111111; st->scenes[szene].dimmableLights = 0b11111111; st->scenes[szene].offDelay = 600;
			//st->scenes[szene].lightValues[0].W = 150;
			//st->scenes[szene].lightValues[1].W = 150;
			//st->scenes[szene].lightValues[2].W = 0;
			//st->scenes[szene].lightValues[2].R = 0; st->scenes[szene].lightValues[2].G = 0; st->scenes[szene].lightValues[2].B = 75;
			//break;
		case Gaeste:
			szene = 2;
			st->scenes[szene].activeLighs = 0b11111111; st->scenes[szene].dimmableLights = 0b11111111; st->scenes[szene].offDelay = 120;
			st->scenes[szene].lightValues[0].W = 50;
			szene = 3;
			st->scenes[szene].activeLighs = 0b11111111; st->scenes[szene].dimmableLights = 0b11111111; st->scenes[szene].offDelay = 120;
			st->scenes[szene].lightValues[0].W = 100;
			szene = 4;
			st->scenes[szene].activeLighs = 0b11111111; st->scenes[szene].dimmableLights = 0b11111111; st->scenes[szene].offDelay = 120;
			st->scenes[szene].lightValues[0].W = 150;
			szene = 5;
			st->scenes[szene].activeLighs = 0b11111111; st->scenes[szene].dimmableLights = 0b11111111; st->scenes[szene].offDelay = 120;
			st->scenes[szene].lightValues[0].W = 150;
		break;
		case Buero:
			szene = 2;
			st->scenes[szene].activeLighs = 0b11111111; st->scenes[szene].dimmableLights = 0b11111111; st->scenes[szene].offDelay = 60; 
			st->scenes[szene].lightValues[0].W = 50;
			szene = 3;
			st->scenes[szene].activeLighs = 0b11111111; st->scenes[szene].dimmableLights = 0b11111111; st->scenes[szene].offDelay = 60;
			st->scenes[szene].lightValues[0].W = 100;
			szene = 4;
			st->scenes[szene].activeLighs = 0b11111111; st->scenes[szene].dimmableLights = 0b11111111; st->scenes[szene].offDelay = 1800;
			st->scenes[szene].lightValues[0].W = 150;
			szene = 5;
			st->scenes[szene].activeLighs = 0b11111111; st->scenes[szene].dimmableLights = 0b11111111; st->scenes[szene].offDelay = 600;
			st->scenes[szene].lightValues[0].W = 255;
			break;
		case Wohnzimmer:
			szene = 2;
			st->scenes[szene].activeLighs = 0b11111111; st->scenes[szene].dimmableLights = 0b11111111; st->scenes[szene].offDelay = 600;
			st->scenes[szene].lightValues[0].W = 25;
			st->scenes[szene].lightValues[0].R = 0; st->scenes[szene].lightValues[0].G = 0; st->scenes[szene].lightValues[0].B = 0;
			szene = 3;
			st->scenes[szene].activeLighs = 0b11111111; st->scenes[szene].dimmableLights = 0b11111111; st->scenes[szene].offDelay = 600;
			st->scenes[szene].lightValues[0].W = 50;
			st->scenes[szene].lightValues[0].R = 0; st->scenes[szene].lightValues[0].G = 0; st->scenes[szene].lightValues[0].B = 0;
			szene = 4;
			st->scenes[szene].activeLighs = 0b11111111; st->scenes[szene].dimmableLights = 0b11111111; st->scenes[szene].offDelay = 600;
			st->scenes[szene].lightValues[0].W = 100;
			st->scenes[szene].lightValues[0].R = 100; st->scenes[szene].lightValues[0].G = 100; st->scenes[szene].lightValues[0].B = 100;
			szene = 5;
			st->scenes[szene].activeLighs = 0b11111111; st->scenes[szene].dimmableLights = 0b11111111; st->scenes[szene].offDelay = 600;
			st->scenes[szene].lightValues[0].W = 150;
			st->scenes[szene].lightValues[0].R = 150; st->scenes[szene].lightValues[0].G = 150; st->scenes[szene].lightValues[0].B = 150;
			break;
		case Essen:
			szene = 2;
			st->scenes[szene].activeLighs = 0b11111111; st->scenes[szene].dimmableLights = 0b11111111; st->scenes[szene].offDelay = 300;
			st->scenes[szene].lightValues[0].W = 50;
			st->scenes[szene].lightValues[1].W = 10;
			st->scenes[szene].lightValues[2].W = 10;
			szene = 3;
			st->scenes[szene].activeLighs = 0b11111111; st->scenes[szene].dimmableLights = 0b11111111; st->scenes[szene].offDelay = 600;
			st->scenes[szene].lightValues[0].W = 50;
			st->scenes[szene].lightValues[1].W = 50;
			st->scenes[szene].lightValues[2].W = 50;
			szene = 4;
			st->scenes[szene].activeLighs = 0b11111111; st->scenes[szene].dimmableLights = 0b11111111; st->scenes[szene].offDelay = 1800;
			st->scenes[szene].lightValues[0].W = 100;
			st->scenes[szene].lightValues[1].W = 80;
			st->scenes[szene].lightValues[2].W = 80;
			szene = 5;
			st->scenes[szene].activeLighs = 0b11111111; st->scenes[szene].dimmableLights = 0b11111111; st->scenes[szene].offDelay = 1800;
			st->scenes[szene].lightValues[0].W = 150;
			st->scenes[szene].lightValues[1].W = 150;
			st->scenes[szene].lightValues[2].W = 150;
			szene = 6;
			st->scenes[szene].activeLighs = 0b11111111; st->scenes[szene].dimmableLights = 0b11111111; st->scenes[szene].offDelay = 1800;
			st->scenes[szene].lightValues[0].W = 200;
			st->scenes[szene].lightValues[1].W = 200;
			st->scenes[szene].lightValues[2].W = 200;
			break;
		case Flur:
			szene = 2;
			st->scenes[szene].activeLighs = 0b11111111; st->scenes[szene].dimmableLights = 0b11111111; st->scenes[szene].offDelay = 120;
			st->scenes[szene].lightValues[0].W = 2;
			st->scenes[szene].lightValues[1].W = 2;
			st->scenes[szene].lightValues[2].W = 2;
			szene = 3;
			st->scenes[szene].activeLighs = 0b11111111; st->scenes[szene].dimmableLights = 0b11111111; st->scenes[szene].offDelay = 120;
			st->scenes[szene].lightValues[0].W = 10;
			st->scenes[szene].lightValues[1].W = 10;
			st->scenes[szene].lightValues[2].W = 10;
			szene = 4;
			st->scenes[szene].activeLighs = 0b11111111; st->scenes[szene].dimmableLights = 0b11111111; st->scenes[szene].offDelay = 120;
			st->scenes[szene].lightValues[0].W = 50;
			st->scenes[szene].lightValues[1].W = 50;
			st->scenes[szene].lightValues[2].W = 50;
			szene = 5;
			st->scenes[szene].activeLighs = 0b11111111; st->scenes[szene].dimmableLights = 0b11111111; st->scenes[szene].offDelay = 120;
			st->scenes[szene].lightValues[0].W = 100;
			st->scenes[szene].lightValues[1].W = 100;
			st->scenes[szene].lightValues[2].W = 100;
			break;
		case Bad:	//16 17 15 35 36 PanelBadewanne
			szene = 2;	//10 PM_Nacht
			st->scenes[szene].activeLighs = 0b11111111; st->scenes[szene].dimmableLights = 0b11111111; st->scenes[szene].offDelay = 60;
			st->scenes[szene].lightValues[0].W = 0;	//16 PanelBadewanne
			st->scenes[szene].lightValues[1].W = 0;	//17 PanelDusche
			st->scenes[szene].lightValues[2].W = 0;	//15 Spiegel
			st->scenes[szene].lightValues[3].W = 25;	//35 W-Stripe Dusche
			st->scenes[szene].lightValues[3].R = 25; st->scenes[szene].lightValues[3].G = 0; st->scenes[szene].lightValues[3].B = 25;	// RGB-Stripe Dusche
			szene = 3;	//11 PM_Tag
			st->scenes[szene].activeLighs = 0b11111111; st->scenes[szene].dimmableLights = 0b11111111; st->scenes[szene].offDelay = 300;
			st->scenes[szene].lightValues[0].W = 5;	//16 PanelBadewanne
			st->scenes[szene].lightValues[1].W = 5;	//17 PanelDusche
			st->scenes[szene].lightValues[2].W = 10;	//15 Spiegel
			st->scenes[szene].lightValues[3].W = 50;	//35 W-Stripe Dusche
			st->scenes[szene].lightValues[3].R = 50; st->scenes[szene].lightValues[3].G = 0; st->scenes[szene].lightValues[3].B = 50;	// RGB-Stripe Dusche
			szene = 4;	//12 Duschen
			st->scenes[szene].activeLighs = 0b11111111; st->scenes[szene].dimmableLights = 0b11111111; st->scenes[szene].offDelay = 600;
			st->scenes[szene].lightValues[0].W = 5;	//16 PanelBadewanne
			st->scenes[szene].lightValues[1].W = 0;	//17 PanelDusche
			st->scenes[szene].lightValues[2].W = 5;	//15 Spiegel
			st->scenes[szene].lightValues[3].W = 150;	//35 W-Stripe Dusche
			st->scenes[szene].lightValues[3].R = 120; st->scenes[szene].lightValues[3].G = 0; st->scenes[szene].lightValues[3].B = 120;	// RGB-Stripe Dusche
			szene = 5;	//13 WC
			st->scenes[szene].activeLighs = 0b11111111; st->scenes[szene].dimmableLights = 0b11111111; st->scenes[szene].offDelay = 600;
			st->scenes[szene].lightValues[0].W = 10;	//16 PanelBadewanne
			st->scenes[szene].lightValues[1].W = 50;	//17 PanelDusche
			st->scenes[szene].lightValues[2].W = 5;	//15 Spiegel
			st->scenes[szene].lightValues[3].W = 10;	//35 W-Stripe Dusche
			st->scenes[szene].lightValues[3].R = 25; st->scenes[szene].lightValues[3].G = 0; st->scenes[szene].lightValues[3].B = 0;	// RGB-Stripe Dusche
			szene = 6;	//14 Spiegel
			st->scenes[szene].activeLighs = 0b11111111; st->scenes[szene].dimmableLights = 0b11111111; st->scenes[szene].offDelay = 600;
			st->scenes[szene].lightValues[0].W = 50;	//16 PanelBadewanne
			st->scenes[szene].lightValues[1].W = 0;	//17 PanelDusche
			st->scenes[szene].lightValues[2].W = 150;	//15 Spiegel
			st->scenes[szene].lightValues[3].W = 5;	//35 W-Stripe Dusche
			st->scenes[szene].lightValues[3].R = 0; st->scenes[szene].lightValues[3].G = 5; st->scenes[szene].lightValues[3].B = 0;	// RGB-Stripe Dusche
			szene = 7;	//15 Baden
			st->scenes[szene].activeLighs = 0b11111111; st->scenes[szene].dimmableLights = 0b11111111; st->scenes[szene].offDelay = 600;
			st->scenes[szene].lightValues[0].W = 50;	//16 PanelBadewanne
			st->scenes[szene].lightValues[1].W = 0;	//17 PanelDusche
			st->scenes[szene].lightValues[2].W = 25;	//15 Spiegel
			st->scenes[szene].lightValues[3].W = 0;	//35 W-Stripe Dusche
			st->scenes[szene].lightValues[3].R = 0; st->scenes[szene].lightValues[3].G = 0; st->scenes[szene].lightValues[3].B = 0;	// RGB-Stripe Dusche
			break;
		case Umkleide:
			szene = 2;
			st->scenes[szene].activeLighs = 0b11111111; st->scenes[szene].dimmableLights = 0b11111111; st->scenes[szene].offDelay = 60;
			st->scenes[szene].lightValues[0].W = 5;
			szene = 3;
			st->scenes[szene].activeLighs = 0b11111111; st->scenes[szene].dimmableLights = 0b11111111; st->scenes[szene].offDelay = 120;
			st->scenes[szene].lightValues[0].W = 10;
			szene = 4;
			st->scenes[szene].activeLighs = 0b11111111; st->scenes[szene].dimmableLights = 0b11111111; st->scenes[szene].offDelay = 600;
			st->scenes[szene].lightValues[0].W = 150;
			szene = 5;
			st->scenes[szene].activeLighs = 0b11111111; st->scenes[szene].dimmableLights = 0b11111111; st->scenes[szene].offDelay = 600;
			st->scenes[szene].lightValues[0].W = 150;
			break;
		case TWC:
			szene = 2;	//10 PM_Nacht
			st->scenes[szene].activeLighs = 0b11111111; st->scenes[szene].dimmableLights = 0b11111111; st->scenes[szene].offDelay = 60;
			st->scenes[szene].lightValues[0].W = 5;
			st->scenes[szene].lightValues[1].W = 5;
			szene = 3;	//11 PM_Tag
			st->scenes[szene].activeLighs = 0b11111111; st->scenes[szene].dimmableLights = 0b11111111; st->scenes[szene].offDelay = 60;
			st->scenes[szene].lightValues[0].W = 20;
			st->scenes[szene].lightValues[1].W = 25;
			szene = 4;	//12 WC+Vent
			st->scenes[szene].activeLighs = 0b11111111; st->scenes[szene].dimmableLights = 0b11111111; st->scenes[szene].offDelay = 300;
			st->scenes[szene].lightValues[0].W = 20;
			st->scenes[szene].lightValues[1].W = 25;
			szene = 5;	//12 Spiegel
			st->scenes[szene].activeLighs = 0b11111111; st->scenes[szene].dimmableLights = 0b11111111; st->scenes[szene].offDelay = 60;
			st->scenes[szene].lightValues[0].W = 150;
			st->scenes[szene].lightValues[1].W = 150;
			szene = 6;	//14 Duschen
			st->scenes[szene].activeLighs = 0b11111111; st->scenes[szene].dimmableLights = 0b11111111; st->scenes[szene].offDelay = 600;
			st->scenes[szene].lightValues[0].W = 100;
			st->scenes[szene].lightValues[1].W = 10;
			break;
		case Treppenhaus:
			szene = 2;
			st->scenes[szene].activeLighs = 0b11111111; st->scenes[szene].dimmableLights = 0b11111111; st->scenes[szene].offDelay = 60;
			st->scenes[szene].lightValues[0].W = 5;
			st->scenes[szene].lightValues[1].W = 5;
			szene = 3;
			st->scenes[szene].activeLighs = 0b11111111; st->scenes[szene].dimmableLights = 0b11111111; st->scenes[szene].offDelay = 60;
			st->scenes[szene].lightValues[0].W = 25;
			st->scenes[szene].lightValues[1].W = 25;
			szene = 4;
			st->scenes[szene].activeLighs = 0b11111111; st->scenes[szene].dimmableLights = 0b11111111; st->scenes[szene].offDelay = 300;
			st->scenes[szene].lightValues[0].W = 50;
			st->scenes[szene].lightValues[1].W = 50;
			szene = 5;
			st->scenes[szene].activeLighs = 0b11111111; st->scenes[szene].dimmableLights = 0b11111111; st->scenes[szene].offDelay = 300;
			st->scenes[szene].lightValues[0].W = 150;
			st->scenes[szene].lightValues[1].W = 150;
		break;
		case Terasse:
			szene = 2;
			st->scenes[szene].activeLighs = 0b11111111; st->scenes[szene].dimmableLights = 0b11111111; st->scenes[szene].offDelay = 60;
			st->scenes[szene].lightValues[0].W = 0;		//
			st->scenes[szene].lightValues[1].W = 0;		// RGBW-Stripe
			st->scenes[szene].lightValues[1].R = 0; st->scenes[szene].lightValues[3].G = 0; st->scenes[szene].lightValues[3].B = 0;	// RGBW-Stripe
			szene = 3;
			st->scenes[szene].activeLighs = 0b11111111; st->scenes[szene].dimmableLights = 0b11111111; st->scenes[szene].offDelay = 60;
			st->scenes[szene].lightValues[0].W = 0;		//
			st->scenes[szene].lightValues[1].W = 0;		// RGBW-Stripe
			st->scenes[szene].lightValues[1].R = 0; st->scenes[szene].lightValues[3].G = 0; st->scenes[szene].lightValues[3].B = 0;	// RGBW-Stripe
			szene = 4;
			st->scenes[szene].activeLighs = 0b11111111; st->scenes[szene].dimmableLights = 0b11111111; st->scenes[szene].offDelay = 60;
			st->scenes[szene].lightValues[0].W = 0;		//
			st->scenes[szene].lightValues[1].W = 0;		// RGBW-Stripe
			st->scenes[szene].lightValues[1].R = 0; st->scenes[szene].lightValues[3].G = 0; st->scenes[szene].lightValues[3].B = 0;	// RGBW-Stripe
			szene = 5;
			st->scenes[szene].activeLighs = 0b11111111; st->scenes[szene].dimmableLights = 0b11111111; st->scenes[szene].offDelay = 60;
			st->scenes[szene].lightValues[0].W = 0;		//
			st->scenes[szene].lightValues[1].W = 0;		// RGBW-Stripe
			st->scenes[szene].lightValues[1].R = 0; st->scenes[szene].lightValues[3].G = 0; st->scenes[szene].lightValues[3].B = 0;	// RGBW-Stripe
		break;
		case Speiss:
			szene = 2;
			st->scenes[szene].activeLighs = 0b11111111; st->scenes[szene].dimmableLights = 0b11111111; st->scenes[szene].offDelay = 60;
			st->scenes[szene].lightValues[0].W = 100;
			szene = 3;
			st->scenes[szene].activeLighs = 0b11111111; st->scenes[szene].dimmableLights = 0b11111111; st->scenes[szene].offDelay = 60;
			st->scenes[szene].lightValues[0].W = 255;
			szene = 4;
			st->scenes[szene].activeLighs = 0b11111111; st->scenes[szene].dimmableLights = 0b11111111; st->scenes[szene].offDelay = 600;
			st->scenes[szene].lightValues[0].W = 255;
			szene = 5;
			st->scenes[szene].activeLighs = 0b11111111; st->scenes[szene].dimmableLights = 0b11111111; st->scenes[szene].offDelay = 600;
			st->scenes[szene].lightValues[0].W = 255;
		break;
		default:
			szene = 2;
			st->scenes[szene].activeLighs = 0b11111111; st->scenes[szene].dimmableLights = 0b11111111; st->scenes[szene].offDelay = 120;
			st->scenes[szene].lightValues[0].W = 50;
			szene = 3;
			st->scenes[szene].activeLighs = 0b11111111; st->scenes[szene].dimmableLights = 0b11111111; st->scenes[szene].offDelay = 120;
			st->scenes[szene].lightValues[0].W = 100;
			szene = 4;
			st->scenes[szene].activeLighs = 0b11111111; st->scenes[szene].dimmableLights = 0b11111111; st->scenes[szene].offDelay = 120;
			st->scenes[szene].lightValues[0].W = 150;
			szene = 5;
			st->scenes[szene].activeLighs = 0b11111111; st->scenes[szene].dimmableLights = 0b11111111; st->scenes[szene].offDelay = 120;
			st->scenes[szene].lightValues[0].W = 150;
			break;
	}
}




#endif /* KMX_DEFAULTSZENES_H_ */