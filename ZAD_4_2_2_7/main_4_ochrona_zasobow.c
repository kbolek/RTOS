#include "FreeRTOS.h"
#include "task.h"
#include "led.h"
#include "semphr.h"
#include "uart.h"
#include "string.h"
#include "keyboard.h"


xSemaphoreHandle xSemaphore;
TickType_t StartCountingTicks, ExecutionTicks=0;

void Rtos_Transmiter_SendString(char *cString){
	char cTxString[20];
 
	StartCountingTicks = xTaskGetTickCount(); //the count of ticks since vTaskStartScheduler was called. 
	CopyString(cString,cTxString);
	AppendUIntToString(ExecutionTicks,cTxString);
	AppendString("\n", cTxString);
	
	
	xSemaphoreTake(xSemaphore,portMAX_DELAY);
	Transmiter_SendString(cTxString);
	while (Transmiter_GetStatus()!=FREE){};
	xSemaphoreGive(xSemaphore);
	ExecutionTicks = xTaskGetTickCount()-StartCountingTicks;
	
}

void LettersTx (void *pvParameters){

	while(1){	
	
		Rtos_Transmiter_SendString("-ABCDEEFGH-:");
		vTaskDelay(300);
	}
}

void KeyboardTx (void *pvParameters){
	
	while(1){
	
	while(eKeyboardRead() == RELEASED){}
	Rtos_Transmiter_SendString("-Keyboard-:");
	vTaskDelay(300);
	
  }
		
}

int main( void ){

	UART_InitWithInt(300);
	KeyboardInit();
  vSemaphoreCreateBinary(xSemaphore); 
	xTaskCreate(LettersTx, NULL, 128, NULL, 1, NULL );
	xTaskCreate(KeyboardTx, NULL, 128, NULL, 1, NULL );
	vTaskStartScheduler();
	while(1);
}
