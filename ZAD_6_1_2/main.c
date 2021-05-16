#include "FreeRTOS.h"
#include "task.h"
#include "led.h"
#include "watch.h"


void WatchOnLed(void *pvParameters){
	struct WatchEvent asWatchRead;
	while(1){
		asWatchRead = sWatch_Read();
		switch(asWatchRead.eTimeUnit){
			case SECONDS:
				LedToggle(1);
				break;
			case MINUTES:
				LedToggle(2);
				break;
			default:
				break;
			
		}
	}
}

int main( void ){
	
	LedInit();
	Watch_Init();
	
	xTaskCreate(Watch_Thread,NULL,128,NULL,1,NULL);
	xTaskCreate(WatchOnLed,NULL,128,NULL,1,NULL);
	vTaskStartScheduler();
	
	while(1);
}
