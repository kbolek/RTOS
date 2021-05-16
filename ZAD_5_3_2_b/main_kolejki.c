#include "FreeRTOS.h"
#include "task.h"
#include "led.h"
#include "semphr.h"
#include "uart.h"
#include "string.h"
#include "keyboard.h"
#include "queue.h"

QueueHandle_t xQueue;

char ucTxString[TRANSMITER_SIZE];

void Rtos_Transmiter_SendString(void *pvParameters){
	char cStringReceived[20];
	TickType_t StartCountingTicks, ExecutionTicks=0;
	while(1){
		if(xQueueReceive(xQueue,cStringReceived,portMAX_DELAY) == pdPASS){
			CopyString(cStringReceived, ucTxString);
			AppendUIntToString(ExecutionTicks,ucTxString);
			AppendString("\n", ucTxString);	
			
			StartCountingTicks = xTaskGetTickCount(); //the count of ticks since vTaskStartScheduler was called. 
			Transmiter_SendString(ucTxString);
			while (Transmiter_GetStatus()!=FREE){};
			ExecutionTicks = xTaskGetTickCount()-StartCountingTicks;
		}

	}
	
	
}

void LettersTx (void *pvParameters){
	char* cStringToSend = ((char *)pvParameters);
	
	while(1){			
		if(xQueueSendToBack(xQueue,cStringToSend,0) != pdPASS){
			LedToggle(0);
		}	
		vTaskDelay(100);
	}
}

void KeyboardTx (void *pvParameters){
	char* cStringToSend = ((char *)pvParameters);
	while(1){	
		if(eKeyboardRead() != RELEASED){
			xQueueSendToBack(xQueue,cStringToSend,0);	
		}
		vTaskDelay(100);
  }
		
}

int main( void ){

	UART_InitWithInt(300);
	KeyboardInit();
	LedInit();

	xQueue = xQueueCreate(5,sizeof(char[20]));
	xTaskCreate(LettersTx, NULL, 128,"-ABCDEEFGH-:", 1, NULL );
	xTaskCreate(KeyboardTx, NULL, 128,"-Keyboard-:", 1, NULL );
	xTaskCreate(Rtos_Transmiter_SendString, NULL, 128, NULL, 1, NULL );
	vTaskStartScheduler();
	while(1);
}
