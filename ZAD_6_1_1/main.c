#include "FreeRTOS.h"
#include "task.h"
#include "keyboard.h"
#include "led.h"


void LedOnByButton(void *pvParameters){
	while(1){
		switch(eKeyboard_Read()){
			case BUTTON_1:
				LedToggle(0);
				break;
			case BUTTON_2:
				LedToggle(1);
				break;
			case BUTTON_3:
				LedToggle(2);
				break;
			case BUTTON_4:
				LedToggle(3);
				break;
			case RELEASED:
				break;
			default:
				break;
		}
	}
}

int main( void ){
	
	LedInit();
	KeyboardInit();
	
	xTaskCreate(Keyboard_Thread,NULL,128,NULL,1,NULL);
	xTaskCreate(LedOnByButton,NULL,128,NULL,1,NULL);
	vTaskStartScheduler();
	
	while(1);
}
