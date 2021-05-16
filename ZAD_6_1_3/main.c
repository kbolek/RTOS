#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "led.h"
#include "watch.h"
#include "keyboard.h"
#include "uart.h"
#include "string.h"

enum API_commands {BUTTON, WATCH_SEC, WATCH_MIN};

typedef struct{
	enum API_commands eAPI_command;
	unsigned int uiParam;
}tDataSend;

xSemaphoreHandle xSemaphore;


void Rtos_Transmiter_SendString(tDataSend asDataSend){
	char cTxString[TRANSMITER_SIZE];
	
	switch(asDataSend.eAPI_command){
		case BUTTON:
			CopyString("button ",cTxString);
			break;
		case WATCH_SEC:
			CopyString("seconds ",cTxString);
			break;
		case WATCH_MIN:
			CopyString("minutes ",cTxString);
			break;
		default:
			break;
	}
	
	AppendUIntToString(asDataSend.uiParam,cTxString);
	AppendString("\n", cTxString);
	Uart_PutString(cTxString);
}


void LedOnByButton(void *pvParameters){
	tDataSend asDataSend = {BUTTON,0};
	enum KeyboardState ePreviousState,eCurrentState = RELEASED;

	while(1){
		ePreviousState = eCurrentState;
		eCurrentState = eKeyboard_Read();
		
		if(eCurrentState != ePreviousState){
			switch(eCurrentState){
			case BUTTON_1:
				asDataSend.uiParam = 1;
				break;
			case BUTTON_2:
				asDataSend.uiParam = 2;
				break;
			case BUTTON_3:
				asDataSend.uiParam = 3;
				break;
			case BUTTON_4:
				asDataSend.uiParam = 4;
				break;
			case RELEASED:
				break;
			default:
				break;
			}
		}	
		if(asDataSend.uiParam != 0){
			Rtos_Transmiter_SendString(asDataSend);
		}
		else {}
	}
}

void WatchOnLed(void *pvParameters){
	struct WatchEvent asWatchRead;
	tDataSend asDataSend = {WATCH_SEC,0};
	while(1){
		asWatchRead = sWatch_Read();
		switch(asWatchRead.eTimeUnit){
			case SECONDS:
				asDataSend.eAPI_command = WATCH_SEC;
				asDataSend.uiParam = asWatchRead.TimeValue;
				break;
			case MINUTES:
				asDataSend.eAPI_command = WATCH_MIN;
				asDataSend.uiParam = asWatchRead.TimeValue;
				break;
			default:
				break;
		}
		if(asDataSend.uiParam != 0){
			Rtos_Transmiter_SendString(asDataSend);
		}
		else {}
	}
}

int main( void ){
	
	LedInit();
	Watch_Init();
	KeyboardInit();
	UART_InitWithInt(9600);
	
	xTaskCreate(Keyboard_Thread,NULL,128,NULL,1,NULL);
	xTaskCreate(LedOnByButton,NULL,128,NULL,1,NULL);
	xTaskCreate(Watch_Thread,NULL,128,NULL,1,NULL);
	xTaskCreate(WatchOnLed,NULL,128,NULL,1,NULL);
	vTaskStartScheduler();
	
	while(1);
}
