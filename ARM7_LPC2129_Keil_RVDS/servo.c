#include <LPC21xx.H>
#include "led.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "servo.h"

#define DETECTOR_bm (1<<10)

QueueHandle_t ServoQueue;
QueueHandle_t xStatusQueue;

enum ServoCommands {_CALLIB,_GOTO,_WAIT,_SPEED};
struct ServoCtr{
	enum ServoCommands eServoCommand;
	unsigned int uiServoCommandsParam;
};


void DetectorInit(){
	IO0DIR = IO0DIR&(~DETECTOR_bm);
}


enum State {ACTIVE,INACTIVE};
enum State eReadDetector(){
	if((IO0PIN&DETECTOR_bm)==0){
		return ACTIVE;
	}
	else{
		return INACTIVE;
	}
}


void ServoCallib(void){
	struct ServoCtr eServoCtr = {_CALLIB,0};
	xQueueSendToBack(ServoQueue,&eServoCtr ,portMAX_DELAY);
}

void ServoGoTo(unsigned int uiPosition){
		struct ServoCtr eServoCtr = {_GOTO,0};
		eServoCtr.uiServoCommandsParam = uiPosition;
		xQueueSendToBack(ServoQueue,&eServoCtr ,portMAX_DELAY);
}

void ServoWait(unsigned int uiTicksToWait){
	struct ServoCtr eServoCtr = {_WAIT,0};
	eServoCtr.uiServoCommandsParam = uiTicksToWait;
	xQueueSendToBack(ServoQueue,&eServoCtr ,portMAX_DELAY);
}

void ServoSpeed(unsigned int uiServoSpeed){
	struct ServoCtr eServoCtr = {_SPEED,0};
	eServoCtr.uiServoCommandsParam = uiServoSpeed;
	xQueueSendToBack(ServoQueue,&eServoCtr ,portMAX_DELAY);
}

void Automat(void *pvParameters){
	struct ServoParam eServo = {IDLE,0,0};
	struct ServoCtr eServoBuffer;
	TickType_t ServoStepDelay = 10;

	while(1){
		switch(eServo.eState){
			case IDLE:
					  xQueueReceive(ServoQueue,&eServoBuffer,portMAX_DELAY);
						switch(eServoBuffer.eServoCommand){
							case _CALLIB:
								eServo.eState = CALLIBRATION;
								break;
							case _GOTO:
								eServo.eState = IN_PROGRESS;
								eServo.uiDesiredPostion = eServoBuffer.uiServoCommandsParam;
								break;
							case _WAIT:
								eServo.eState = IDLE;
								vTaskDelay((TickType_t)eServoBuffer.uiServoCommandsParam);
								break;
							case _SPEED:
								ServoStepDelay = (TickType_t)eServoBuffer.uiServoCommandsParam;		
								break;		
					 }
					break;	 
					 
			case IN_PROGRESS:
				if(eServo.uiCurrentPosition < eServo.uiDesiredPostion){
					eServo.eState = IN_PROGRESS;
					LedStepRight();
					eServo.uiCurrentPosition++;
				}
				else if(eServo.uiCurrentPosition > eServo.uiDesiredPostion){
					eServo.eState = IN_PROGRESS;
					LedStepLeft();
					eServo.uiCurrentPosition--;
				}
				else {
					eServo.eState = IDLE;
					eServo.uiCurrentPosition = 0;
					eServo.uiDesiredPostion = 0;
				}
				vTaskDelay(ServoStepDelay); //Step delay because controled by ServoSpeed
				break;
				
			case CALLIBRATION: 
				if(eReadDetector()==INACTIVE){
					LedStepRight();
				}
				else{
					eServo.eState = IDLE;
					eServo.uiCurrentPosition = 0;
					eServo.uiDesiredPostion = 0;
				}	
		}	
		xQueueOverwrite(xStatusQueue,&eServo);
		vTaskDelay(1000/(*(TickType_t*)pvParameters));
	}
}

struct ServoParam Servo_State(void){
	struct ServoParam sServoStatus;
	
	xQueuePeek(xStatusQueue,&sServoStatus,portMAX_DELAY);
	return sServoStatus;
}


void ServoInit(void){
	TickType_t xAutomatFrequency = 5;
	
	xTaskCreate(Automat, NULL , 100 , &xAutomatFrequency, 1 , NULL );
	ServoQueue = xQueueCreate(15,sizeof(struct ServoCtr));
	xStatusQueue = xQueueCreate(1, sizeof(struct ServoParam));
	LedInit();
	DetectorInit();
	ServoCallib();
}	

