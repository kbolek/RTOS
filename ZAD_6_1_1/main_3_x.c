#include "FreeRTOS.h"
#include "task.h"
#include "led.h"

typedef struct{
	unsigned char ucLedNumber;
	unsigned char ucBlinkingFreq;
}sLedCtrl;


void LedCtrl(void *pvParameters){

	while(1){
		vTaskSuspend(*(xTaskHandle *)pvParameters);
		vTaskDelay(1000);
		vTaskResume(*(xTaskHandle *)pvParameters);
		vTaskDelay(1000);
	}

}

void LedBlink( void *pvParameters ){
	
	while(1){
		LedToggle(((sLedCtrl*)pvParameters)->ucLedNumber);
		vTaskDelay(1000/(((sLedCtrl*)pvParameters)->ucBlinkingFreq)/2);
	}	
}


int main(void){
	sLedCtrl asLedCtr = {0,10};
	xTaskHandle TaskHandle_LedBlinking;
	LedInit();
	xTaskCreate(LedBlink,NULL,100,&asLedCtr,2,&TaskHandle_LedBlinking);
	xTaskCreate(LedCtrl,NULL, 100, &TaskHandle_Blinking, 2, NULL);
	vTaskStartScheduler();
	while(1);

}
