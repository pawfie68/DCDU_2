#pragma once
#include <wiringPi.h>

#define	BR_UP	13 ///GPIO9
#define	BR_DWN	12 ///GPIO10
#define	MSG_UP	2 ///GPIO27
#define	MSG_DWN	0 ///GPIO17
#define	L_UP	4 ///GPIO23
#define	L_DWN	5 ///GPIO24

#define	PRINT	27 ///GPIO16
#define	PGE_UP	26 ///GPIO12
#define	PGE_DWN	23 ///GPIO13
#define	R_UP	24 ///GPIO19
#define	R_DWN	25 ///GPIO26

#define	ALL_PULLUP	30

bool flag_BR_UP = false;
bool flag_BR_DWN = false;
bool flag_MSG_UP = false;
bool flag_MSG_DWN = false;
bool flag_L_UP = false;
bool flag_L_DWN = false;

bool flag_PRINT = false;
bool flag_PGE_UP = false;
bool flag_PGE_DWN = false;
bool flag_R_UP = false;
bool flag_R_DWN = false;


void BrightUp()
{
	flag_BR_UP = true;
}

void BrightDwn()
{
	flag_BR_DWN = true;
}

void MsgUp()
{
	flag_MSG_UP = true;
}

void MsgDwn()
{
	flag_MSG_DWN = true;
}

void LeftUp()
{
	flag_L_UP = true;
}

void LeftDwn()
{
	flag_L_DWN = true;
}

void Print()
{
	flag_PRINT = true;
}

void PoeUp()
{
	flag_PGE_UP = true;
}

void PoeDwn()
{
	flag_PGE_DWN = true;
}

void RigthUp()
{
	flag_R_UP = true;
}

void RightDwn()
{
	flag_R_DWN = true;
}

void ConfigurePins()
{
	wiringPiSetup();

	pinMode(L_UP, INPUT); //L_UP
	pinMode(L_DWN, INPUT); //L_DOWN
	pinMode(MSG_UP, INPUT);
	pinMode(MSG_DWN, INPUT); //MSG_DOWN
	pinMode(BR_UP, INPUT);
	pinMode(BR_DWN, INPUT);

	pinMode(ALL_PULLUP, OUTPUT); //PULLUP
	pinMode(PRINT, INPUT); //PRINT
	pinMode(PGE_UP, INPUT); //PGE UP
	pinMode(PGE_DWN, INPUT); //PGE DOWN
	pinMode(R_UP, INPUT); //RIGHT UP
	pinMode(R_DWN, INPUT); // RIGHT DOWN

	pullUpDnControl(L_UP, PUD_DOWN);
	pullUpDnControl(L_DWN, PUD_DOWN);
	pullUpDnControl(MSG_UP, PUD_DOWN);
	pullUpDnControl(MSG_DWN, PUD_DOWN);
	pullUpDnControl(BR_UP, PUD_DOWN);
	pullUpDnControl(BR_DWN, PUD_DOWN);

	pullUpDnControl(PRINT, PUD_DOWN);
	pullUpDnControl(PGE_UP, PUD_DOWN);
	pullUpDnControl(PGE_DWN, PUD_DOWN);
	pullUpDnControl(R_UP, PUD_DOWN);
	pullUpDnControl(R_DWN, PUD_DOWN);


	wiringPiISR(L_UP, INT_EDGE_RISING, &LeftUp);
	wiringPiISR(L_DWN, INT_EDGE_RISING, &LeftDwn);
	wiringPiISR(MSG_UP, INT_EDGE_RISING, &MsgUp);
	wiringPiISR(MSG_DWN, INT_EDGE_RISING, &MsgDwn);
	wiringPiISR(BR_UP, INT_EDGE_RISING, &BrightUp);
	wiringPiISR(BR_DWN, INT_EDGE_RISING, &BrightDwn);

	wiringPiISR(R_UP, INT_EDGE_RISING, &RigthUp);
	wiringPiISR(R_DWN, INT_EDGE_RISING, &RightDwn);
	wiringPiISR(PGE_UP, INT_EDGE_RISING, &PoeUp);
	wiringPiISR(PGE_DWN, INT_EDGE_RISING, &PoeDwn);
	wiringPiISR(PRINT, INT_EDGE_RISING, &Print);

	digitalWrite(ALL_PULLUP, HIGH);
}