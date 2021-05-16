#ifndef LED_H
#define LED_h

#define LED0_bm  (1<<16)
#define LED1_bm  (1<<17)
#define LED2_bm  (1<<18)
#define LED3_bm  (1<<19)
#define LED4_bm  (1<<20)

enum LED_DIRECTION {LEFT,RIGHT};
void LedInit(void);
void LedStepLeft(void);
void LedStepRight(void);
void LedOn(int iLedNumber);
void LedClr(int iLedNumber);
void LedToggle(int iLedNumber);

#endif
