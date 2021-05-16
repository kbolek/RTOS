#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "keyboard.h"
#include "uart.h"
#include "string.h"
#include "command_decoder.h"
#include "servo.h"
#include "led.h"

#define QUEUE_EVENT_SIZE 20

QueueHandle_t xQueueEvent;

void UART_Event_Handler(void *pvParameters){
	char cQueueBuffer[QUEUE_EVENT_SIZE];
	
	while(1){
		Uart_GetString(cQueueBuffer);
		xQueueSend(xQueueEvent, &cQueueBuffer, portMAX_DELAY);
	}
	
}

void Keyboard_Event_Handler(void *pvParameters){
	enum KeyboardState ePreviousState, eCurrentState = RELEASED;
	char cQueueBuffer[QUEUE_EVENT_SIZE];
	
	while(1){
		ePreviousState = eCurrentState;
		eCurrentState = eKeyboardRead();
		if(RELEASED != eCurrentState){
			CopyString("button ", cQueueBuffer);
			AppendUIntToString((unsigned int)eCurrentState,cQueueBuffer);
			xQueueSend(xQueueEvent, &cQueueBuffer, portMAX_DELAY);
		}
	}
}

void Executor(void *pvParameters){
	char cQueueBuffer[QUEUE_EVENT_SIZE];
	
	while(1){
		xQueueReceive(xQueueEvent, cQueueBuffer, portMAX_DELAY);
		DecodeMsg(cQueueBuffer);
		if((ucTokenNr > 0) & (asToken[0].eType == KEYWORD)){
			switch(asToken[0].uValue.eKeyword){
				case CALLIB:
					ServoCallib();
					Uart_PutString("ok");
					break;
				case GOTO:
					ServoGoTo(asToken[1].uValue.uiNumber);
					Uart_PutString("ok");
					break;
				case ID:
					Uart_PutString("id servo");
					break;
				case BUTTON:
					switch(asToken[1].uValue.uiNumber){
						case 1:
							ServoCallib();
							break;
						case 2:
							ServoGoTo(12);
							break;
						case 3: 
							ServoGoTo(24);
							break;
						case 4:
							ServoGoTo(36);
							break;
						default:
							Uart_PutString("unknowncommand");
							break;
					}
					Uart_PutString("ok");
					break;
				default:
					Uart_PutString("unknowncommand");
					break;			
			}		
		}	
		else{
			Uart_PutString("unknowncommand");
		}
	}
}



int main( void ){
	TickType_t xAutomatFrequency = 100;
	
	xQueueEvent = xQueueCreate(1 ,sizeof(char[QUEUE_EVENT_SIZE]));
	UART_InitWithInt(9600);
	ServoInit();
	KeyboardInit();
	LedInit();
	
	xTaskCreate(Keyboard_Event_Handler, NULL , 100 , NULL, 1 , NULL );
	xTaskCreate(Automat, NULL , 100 , &xAutomatFrequency, 1 , NULL );
	xTaskCreate(UART_Event_Handler, NULL , 100 , NULL, 1 , NULL );
	xTaskCreate(Executor, NULL , 100 , NULL, 1 , NULL );
	vTaskStartScheduler();
	
	while(1);
}
