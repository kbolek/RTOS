#include "FreeRTOS.h"
#include "task.h"
#include "led.h"
#include "semphr.h"
#include "string.h"
#include "keyboard.h"
#include "servo.h"

void Keyboard (void *pvParameters){
	
	enum KeyboardState ePreviousState, eCurrentState = RELEASED;
	LedToggle(4);
	while(1){
		ePreviousState = eCurrentState;
		eCurrentState = eKeyboardRead();
		
		if(eCurrentState != ePreviousState){
			switch(eCurrentState){
			case BUTTON_1: 
				ServoCallib();
				break;
			case BUTTON_2: 
				ServoGoTo(14);
				break;
			case BUTTON_3: 
				ServoSpeed(8);
				ServoGoTo(12);
				ServoSpeed(4);
			  ServoGoTo(24);
				ServoSpeed(2);
			  ServoGoTo(36);
				ServoSpeed(1);
			  ServoGoTo(0);
				break;
			case BUTTON_4: 
				ServoGoTo(12);
				ServoWait(100);
			  ServoGoTo(0);
			  ServoGoTo(24);
				ServoWait(200);
			  ServoGoTo(0);
			  ServoGoTo(36);
				ServoWait(300);
			  ServoGoTo(0);
			break;
			case RELEASED:
				break;
			default:{}
		 }
	 }
		vTaskDelay(100);
	}
}

int main( void ){
	TickType_t xServoPeriod = 100;
	
	KeyboardInit();
	ServoInit(100);
	xTaskCreate(Keyboard, NULL, 128, NULL, 1, NULL );
	xTaskCreate(ServoAutomatCall, NULL, 128, &xServoPeriod, 1, NULL);
	vTaskStartScheduler();
	while(1);
}
