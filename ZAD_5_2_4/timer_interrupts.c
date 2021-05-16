#include <LPC21xx.H>
#include "led.h"
#include "timer_interrupts.h"

// TIMER
#define mCOUNTER_ENABLE (1<<0)
#define mCOUNTER_RESET  (1<<1)

// CompareMatch
#define mINTERRUPT_ON_MR0 (1<<0)
#define mRESET_ON_MR0    (1<<1)
#define mMR0_INTERRUPT    (1<<0)

// VIC (Vector Interrupt Controller) VICIntEnable
//#define VIC_TIMER0_CHANNEL_NR 4
#define VIC_TIMER1_CHANNEL_NR 5

// VICVectCntlx Vector Control Registers
#define mIRQ_SLOT_ENABLE (1<<5)


//zmienna globalna bedaca wskaznikiem na funkcje
SemaphoreHandle_t *pxSemaphore;

/**********************************************/
//(Interrupt Service Routine) of Timer 1 interrupt
__irq void Timer1IRQHandler(){

	T1IR=mMR0_INTERRUPT; 	// skasowanie flagi przerwania 
	xSemaphoreGiveFromISR(*pxSemaphore,NULL);
	VICVectAddr=0x00; 	// potwierdzenie wykonania procedury obslugi przerwania
}
/**********************************************/
void Timer1Interrupts_Init(unsigned int uiPeriod,SemaphoreHandle_t *xSemaphore){ // microseconds
	pxSemaphore = xSemaphore;

  // interrupts
	VICIntEnable |= (0x1 << VIC_TIMER1_CHANNEL_NR);            // Enable Timer 1 interrupt channel 
	VICVectCntl1  = mIRQ_SLOT_ENABLE | VIC_TIMER1_CHANNEL_NR;  // Enable Slot 0 and assign it to Timer 1 interrupt channel
	VICVectAddr1  =(unsigned long)Timer1IRQHandler; 	   // Set to Slot 0 Address of Interrupt Service Routine 

 // match module
	/*The Match register values are continuously compared to the Timer Counter value. When the two values are equal, actions can 
	be triggered automatically. The action possibilities are to generate an interrupt, reset the Timer Counter, or stop the timer. Actions 
	are controlled by the settings in the MCR register*/

	T1MR0 = 15 * uiPeriod;                 	      // value 
	T1MCR |= (mINTERRUPT_ON_MR0 | mRESET_ON_MR0); // action 
	T1TCR |=  mCOUNTER_ENABLE; // start 

}
/**********************************************/
