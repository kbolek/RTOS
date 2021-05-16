#ifndef TIMER_INTERRUPTS_H
#define TIMER_INTERRUPTS_H
#include "FreeRTOS.h"
#include "semphr.h"

void Timer1Interrupts_Init(unsigned int uiPeriod,SemaphoreHandle_t *xSemaphore);
#endif
