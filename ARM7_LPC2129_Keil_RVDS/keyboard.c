#include <LPC21xx.H>
#include "keyboard.h"
#include "FreeRTOS.h"
#include "queue.h"

#define BUT1_bm (1<<4)
#define BUT2_bm (1<<6)
#define BUT3_bm (1<<5)
#define BUT4_bm (1<<7)


QueueHandle_t KeyboardQueue;

enum KeyboardState eKeyboardRead(void){
	enum KeyboardState eReturnNumber = RELEASED;
	if((IO0PIN & BUT1_bm) == 0){ 
		eReturnNumber = BUTTON_1;
	}
	else if((IO0PIN&BUT2_bm) == 0){
		eReturnNumber = BUTTON_2;
	}
	else if((IO0PIN&BUT3_bm) == 0){
		eReturnNumber = BUTTON_3;
	}
	else if ((IO0PIN&BUT4_bm) == 0){
		eReturnNumber = BUTTON_4;
	}
	
	return eReturnNumber;
}


void KeyboardInit(void){
	xTaskCreate(Keyboard_Thread, NULL , 100 , NULL, 1 , NULL );
	KeyboardQueue = xQueueCreate(1,sizeof(enum KeyboardState));
	IO0DIR = IO0DIR&(~(BUT1_bm|BUT2_bm|BUT3_bm|BUT4_bm));
}

enum KeyboardState eKeyboard_Read(void){
	
	enum KeyboardState eReturnButton;
	xQueueReceive(KeyboardQueue, &eReturnButton,portMAX_DELAY);
	return eReturnButton;	

}

void Keyboard_Thread(void *pvParameters){
	enum KeyboardState ePreviousState = RELEASED, eCurrentState = RELEASED;
	while(1){
		ePreviousState = eCurrentState;
		eCurrentState = eKeyboardRead();
		if(eCurrentState != ePreviousState){
			xQueueSendToBack(KeyboardQueue,&eCurrentState,portMAX_DELAY);
	  }
	}
}

