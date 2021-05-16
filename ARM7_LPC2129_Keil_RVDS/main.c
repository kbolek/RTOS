#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "keyboard.h"
#include "uart.h"
#include "string.h"
#include "command_decoder.h"
#include "servo.h"

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
	enum KeyboardState eKeyboardState = RELEASED;
	char cQueueBuffer[QUEUE_EVENT_SIZE];
	
	while(1){
		eKeyboardState  = eKeyboard_Read();
		if(RELEASED != eKeyboardState){
			CopyString("button ", cQueueBuffer);
			AppendUIntToString((unsigned int)eKeyboardState,cQueueBuffer);
			AppendString("\n", cQueueBuffer);
			xQueueSend(xQueueEvent, &cQueueBuffer, portMAX_DELAY);
		}
	}
}

void Executor(void *pvParameters){
	char cQueueBuffer[QUEUE_EVENT_SIZE];
	char cString[QUEUE_EVENT_SIZE];
	struct ServoParam sServoStatus;
	
	while(1){
		
		xQueueReceive(xQueueEvent, cQueueBuffer, portMAX_DELAY);
		DecodeMsg(cQueueBuffer);
		if((ucTokenNr > 0) & (asToken[0].eType == KEYWORD)){
			switch(asToken[0].uValue.eKeyword){
				case CALLIB:
					ServoCallib();
					Uart_PutString("ok\n");
					break;
				case GOTO:
					ServoGoTo(asToken[1].uValue.uiNumber);
					Uart_PutString("ok\n");
					break;
				case ID:
					Uart_PutString("id servo\n");
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
							Uart_PutString("unknowncommand\n");
							break;
					}
					Uart_PutString("ok\n");
					break;
				case STATE:
					sServoStatus = Servo_State();
					switch(sServoStatus.eState){
						case CALLIBRATION: 
							CopyString("state callib ", cString); 
							break;
						case IDLE: 
							CopyString("state idle ", cString); 
							break;
						case IN_PROGRESS: 
							CopyString("state in_progress ", cString); 
							break;
						default:
							break;
					}
					AppendUIntToString(sServoStatus.uiCurrentPosition ,cString);
					AppendString("\n", cString);
					Uart_PutString(cString);
					break;
				default:
					Uart_PutString("unknowncommand\n");
					break;
				
			}		
		}	
		else{
			Uart_PutString("unknowncommand");
		}
	}
}



int main( void ){
	
	xQueueEvent = xQueueCreate(10 ,sizeof(char[QUEUE_EVENT_SIZE]));
	UART_InitWithInt(9600);
	ServoInit();
	KeyboardInit();

	xTaskCreate(Keyboard_Event_Handler, NULL , 100 , NULL, 1 , NULL );
	xTaskCreate(UART_Event_Handler, NULL , 100 , NULL, 1 , NULL );
	xTaskCreate(Executor, NULL , 100 , NULL, 1 , NULL );
	vTaskStartScheduler();
	
	while(1);
}
