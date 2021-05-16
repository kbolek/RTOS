#include "watch.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "led.h"

QueueHandle_t WatchQueue;

void Watch_Thread(void *pvParameters){
	struct WatchEvent asWatchEventSeconds = {SECONDS,0};
	struct WatchEvent asWatchEeventMinutes = {MINUTES,0};

	while(1){
		if(60 == asWatchEventSeconds.TimeValue){
			asWatchEventSeconds.TimeValue = 0;
			asWatchEeventMinutes.TimeValue = (asWatchEeventMinutes.TimeValue+1)%60;
			xQueueSendToBack(WatchQueue,&asWatchEeventMinutes,portMAX_DELAY);
			xQueueSendToBack(WatchQueue,&asWatchEventSeconds,portMAX_DELAY);
		}
		else{
			asWatchEventSeconds.TimeValue = 1 + asWatchEventSeconds.TimeValue;
			xQueueSendToBack(WatchQueue,&asWatchEventSeconds,portMAX_DELAY);
		}
		vTaskDelay(1000);
	}
}

struct WatchEvent sWatch_Read(void){
	
	struct WatchEvent eReturnWatch;
	xQueueReceive(WatchQueue, &eReturnWatch,portMAX_DELAY);
	return eReturnWatch;	
	
}

void Watch_Init(void){
	WatchQueue = xQueueCreate(1,sizeof(struct WatchEvent));
}
