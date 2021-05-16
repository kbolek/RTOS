#ifndef SERVO_H
#define SERVO_H


enum ServoState {CALLIBRATION,IDLE,IN_PROGRESS};

struct ServoParam{
	enum ServoState eState;
	unsigned int uiCurrentPosition;
	unsigned int uiDesiredPostion;
};

struct ServoParam Servo_State(void);

void ServoInit(void);
void Automat(void *pvParameters);

void ServoGoTo(unsigned int uiPosition);
void ServoCallib(void);

void ServoWait(unsigned int uiTicksToWait);
void ServoSpeed(unsigned int uiServoSpeed);

#endif
