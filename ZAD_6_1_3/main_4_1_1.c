#include "FreeRTOS.h"
#include "task.h"
#include "led.h"
#include "semphr.h"


void PulseTrigger(void * pvParameters){
	//should every second drive to semaphore to free state 
	while(1){
		vTaskDelay(1000);
		xSemaphoreGive((*(xSemaphoreHandle *)pvParameters));
	}

}

void PulseTriggerDelayed(void *pvParameters){
	vTaskDelay(333);
	while(1){
		xSemaphoreGive((*(xSemaphoreHandle *)pvParameters));
		vTaskDelay(333);
	}
}


void Pulse_LED0(void * pvParameters){
	//if there semaphore is free, blink the LED 0.1s
	while(1){
		xSemaphoreTake((*(xSemaphoreHandle *)pvParameters),portMAX_DELAY);
		LedOn(0);
		vTaskDelay(100);
		LedClr(0);
	}
}

void Pulse_LED1(void * pvParameters){
	while(1){
		xSemaphoreTake((*(xSemaphoreHandle *)pvParameters),portMAX_DELAY);
		LedOn(1);
		vTaskDelay(100);
		LedClr(1);
	}
}



int main(void){
	xSemaphoreHandle xSemaphore;
	vSemaphoreCreateBinary(xSemaphore); //Binary semaphore is used when there is only one shared resource. 
	LedInit();
	xTaskCreate(PulseTrigger,NULL,100,&xSemaphore,1,NULL);
	xTaskCreate(PulseTriggerDelayed,NULL,100,&xSemaphore,1,NULL);
	xTaskCreate(Pulse_LED0,NULL,100,&xSemaphore,1,NULL);
	xTaskCreate(Pulse_LED1,NULL,100,&xSemaphore,1,NULL);
	vTaskStartScheduler();
	while(1);

}
