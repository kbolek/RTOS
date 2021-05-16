#ifndef SERVO_H
#define SERVO_H

void ServoInit(void);
void Automat(void *pvParameters);

void ServoGoTo(unsigned int uiPosition);
void ServoCallib(void);

void ServoWait(unsigned int uiTicksToWait);
void ServoSpeed(unsigned int uiServoSpeed);

#endif
