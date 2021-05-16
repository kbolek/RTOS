#include "FreeRTOS.h"
#include "task.h"
#include "led.h"
#include "uart.h"


void UartRx( void *pvParameters ){
	
	while(1){
		LedToggle(cUart_GetChar() -'0');
	}
}

int main( void ){
	
	
	LedInit();
	UART_InitWithInt(9600);
	
	
	xTaskCreate(UartRx,NULL,128,NULL,1,NULL);
	vTaskStartScheduler();
	
	while(1);
}
