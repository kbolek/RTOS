#include <LPC21xx.H>
#include "led.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#define DETECTOR_bm (1<<10)

QueueHandle_t ServoQueue;

enum State {ACTIVE,INACTIVE};


enum ServoState {CALLIB,IDLE,IN_PROGRESS};
struct ServoParam{
	enum ServoState eState;
	unsigned int uiCurrentPosition;
	unsigned int uiDesiredPostion;
};

enum ServoCommands {_CALLIB,_GOTO,_WAIT,_SPEED};
struct ServoCtr{
	enum ServoCommands eServoCommand;
	unsigned int uiServoCommandsParam;
};


void DetectorInit(){
	IO0DIR = IO0DIR&(~DETECTOR_bm);
}

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
	xQueueSendToBack(ServoQueue,&eServoCtr ,100);
}

void ServoGoTo(unsigned int uiPosition){
		struct ServoCtr eServoCtr = {_GOTO,0};
		eServoCtr.uiServoCommandsParam = uiPosition;
		xQueueSendToBack(ServoQueue,&eServoCtr ,0);
}

void ServoWait(unsigned int uiTicksToWait){
	struct ServoCtr eServoCtr = {_WAIT,0};
	eServoCtr.uiServoCommandsParam = uiTicksToWait;
	xQueueSendToBack(ServoQueue,&eServoCtr ,0);
}

void ServoSpeed(unsigned int uiServoSpeed){
	struct ServoCtr eServoCtr = {_SPEED,0};
	eServoCtr.uiServoCommandsParam = uiServoSpeed;
	xQueueSendToBack(ServoQueue,&eServoCtr ,0);
}

void Automat(void *pvParameters){
	struct ServoParam eServo = {IDLE,0,0};
	struct ServoCtr eServoBuffer;
	TickType_t ServoStepDelay = 10;

	while(1){
		switch(eServo.eState){
			case IDLE:
					if(pdTRUE == xQueueReceive(ServoQueue,&eServoBuffer,100)){
						switch(eServoBuffer.eServoCommand){
							case _CALLIB:
								eServo.eState = CALLIB;
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
					 }
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
			case CALLIB: 
				if(eReadDetector()==INACTIVE){
					LedStepRight();
				}
				else{
					eServo.eState = IDLE;
					eServo.uiCurrentPosition = 0;
					eServo.uiDesiredPostion = 0;
				}	
		}	
		vTaskDelay(1000/(*(TickType_t*)pvParameters));
	}
}
void ServoInit(void){
	ServoQueue = xQueueCreate(10,sizeof(struct ServoCtr));
	LedInit();
	DetectorInit();
	ServoCallib();
}	

