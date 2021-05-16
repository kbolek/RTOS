#include "FreeRTOS.h"
#include "task.h"
#include "led.h"
#include "uart.h"
#include "string.h"


void UartRx( void *pvParameters ){
	char acBuffer[RECIEVER_SIZE];
	char cZero[RECIEVER_SIZE] =  "zero"; 
	char cOne[RECIEVER_SIZE] =  "jeden"; 
	while(1){
		Uart_GetString(acBuffer);
		if(eCompareString(acBuffer,cZero) == EQUAL){
			LedToggle(0);
		}
		if(eCompareString(acBuffer,cOne) == EQUAL){
			LedToggle(1);
		}
		LedToggle(acBuffer[0] -'0');
	}
}

int main( void ){
	
	
	LedInit();
	UART_InitWithInt(9600);
	
	
	xTaskCreate(UartRx,NULL,128,NULL,1,NULL);
	vTaskStartScheduler();
	
	while(1);
}
