#include "led.h"
#include <LPC21xx.H>


void LedOn(int iLedNumber){
	IO1CLR = LED0_bm|LED1_bm|LED2_bm|LED3_bm|LED4_bm;
	switch(iLedNumber){
		case 0:
			IO1SET = LED0_bm;
			break;
		case 1:
			IO1SET = LED1_bm;
			break;
		case 2:
			IO1SET = LED2_bm;
			break;
		case 3:
			IO1SET = LED3_bm;
			break;
		case 4:
			IO1CLR = LED0_bm|LED1_bm|LED2_bm|LED3_bm|LED4_bm;
			break;
		case 5:
			IO1SET = LED4_bm;
	}	
}

void LedClr(int iLedNumber){
	IO1CLR = (LED0_bm << iLedNumber);
}


void LedStep(enum LED_DIRECTION eWhere){
	static unsigned int uiActualLedPoint = 0; 
	if(eWhere == RIGHT){
		uiActualLedPoint = uiActualLedPoint + 1;
	}
	else if (eWhere == LEFT){
		--uiActualLedPoint;
	}
	LedOn(uiActualLedPoint%4);
}


void LedStepLeft(void){

  LedStep(LEFT);
}


void LedStepRight(void){

  LedStep(RIGHT);
}

void LedInit(void){
		IO1DIR = IO1DIR|LED0_bm|LED1_bm|LED2_bm|LED3_bm|LED4_bm;
}

void LedToggle(int iLedNumber){
	if(IO1PIN & (LED0_bm << iLedNumber)){
		IO1CLR = (LED0_bm << iLedNumber);
	}
	else{
		IO1SET = (LED0_bm << iLedNumber);
	}
}
