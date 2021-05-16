#include <LPC210X.H>
#include "uart.h"
#include "string.h"
#include "FreeRTOS.h"
#include "queue.h"



/************ UART ************/
//PIN CONTROL REGISTER
#define mP0_1_RX0_PIN_MODE 												0x00000004
#define mP0_0_TX0_PIN_MODE 												0x00000001

// U0LCR Line Control Register
#define mDIVISOR_LATCH_ACCES_BIT                   0x00000080
#define m8BIT_UART_WORD_LENGTH                     0x00000003

// UxIER Interrupt Enable Register
#define mRX_DATA_AVALIABLE_INTERRUPT_ENABLE        0x00000001
#define mTHRE_INTERRUPT_ENABLE                     0x00000002

// UxIIR Pending Interrupt Identification Register
#define mINTERRUPT_PENDING_IDETIFICATION_BITFIELD  0x0000000F
#define mTHRE_INTERRUPT_PENDING                    0x00000002
#define mRX_DATA_AVALIABLE_INTERRUPT_PENDING       0x00000004

/************ Interrupts **********/
// VIC (Vector Interrupt Controller) channels
#define VIC_UART0_CHANNEL_NR  6
#define VIC_UART1_CHANNEL_NR  7

// VICVectCntlx Vector Control Registers
#define mIRQ_SLOT_ENABLE                           0x00000020
///////////////////////////////////////////////////////////////

/*******************TRANSMITER CODE******************/
typedef struct TransmiterBuffer{
char cData[TRANSMITER_SIZE];
enum eTransmiterStatus eStatus;
unsigned char fLastCharacter;
unsigned char cCharCtr;
}TransmiterBuffer;

struct TransmiterBuffer sTransmiterBuffer;

char Transmiter_GetCharacterFromBuffer(void){
	char ucActualChar;
	ucActualChar = sTransmiterBuffer.cData[sTransmiterBuffer.cCharCtr];
	if(ucActualChar == '\0'){
		if(sTransmiterBuffer.fLastCharacter==0){
			sTransmiterBuffer.fLastCharacter=1;
			return(CR);
		}
		else{
			sTransmiterBuffer.fLastCharacter=0;
			sTransmiterBuffer.eStatus=FREE;
			return('\0');
		}
	}
	else{
		sTransmiterBuffer.cCharCtr++;
		return(ucActualChar);
	}	
}

void Transmiter_SendString(char cString[]){
	sTransmiterBuffer.eStatus=BUSY;
	sTransmiterBuffer.cCharCtr=0;
	CopyString(cString,sTransmiterBuffer.cData);
	U0THR = Transmiter_GetCharacterFromBuffer();
}
	
enum eTransmiterStatus Transmiter_GetStatus(void){
	return sTransmiterBuffer.eStatus;
}

/*******************END OF THE TRANSMITER CODE******************/

/*******************RECEIVER CODE******************/
QueueHandle_t xQueue;

char cUart_GetChar(void){
	char cQueueReceivedChar;
	xQueueReceive(xQueue,&cQueueReceivedChar,portMAX_DELAY);
	return cQueueReceivedChar;
}


/*******************END OF THE REVEIVER CODE******************/

///////////////////////////////////////////
__irq void UART0_Interrupt (void) {
   // jesli przerwanie z odbiornika (Rx)
	
   
   unsigned int uiCopyOfU0IIR=U0IIR; // odczyt U0IIR powoduje jego kasowanie wiec lepiej pracowac na kopii

   if((uiCopyOfU0IIR & mINTERRUPT_PENDING_IDETIFICATION_BITFIELD) == mRX_DATA_AVALIABLE_INTERRUPT_PENDING) // odebrano znak
   {
				char cReceivedChar = U0RBR;
				xQueueSendToBackFromISR(xQueue,&cReceivedChar,NULL);
   } 
   
   if ((uiCopyOfU0IIR & mINTERRUPT_PENDING_IDETIFICATION_BITFIELD) == mTHRE_INTERRUPT_PENDING)              // wyslano znak - nadajnik pusty 
   {
      if(sTransmiterBuffer.eStatus == BUSY){
				U0THR = Transmiter_GetCharacterFromBuffer();
			}
		
   }

   VICVectAddr = 0; // Acknowledge Interrupt
}


void UART_InitWithInt(unsigned int uiBaudRate){
	unsigned long ulDivisor, ulWantedClock;
	xQueue = xQueueCreate(RECIEVER_SIZE ,sizeof(char));
	ulWantedClock=uiBaudRate*16;
	ulDivisor=15000000/ulWantedClock; // for 300 baudrate ulDivisor = 3125 => 0x35
	// UART
	PINSEL0 = PINSEL0 | 0x55;                                     // ustawic piny uar0 odbiornik nadajnik
	U0LCR  |= m8BIT_UART_WORD_LENGTH | mDIVISOR_LATCH_ACCES_BIT; // d³ugosc s³owa, DLAB = 1
	U0DLL = ( unsigned char ) ( ulDivisor & ( unsigned long ) 0xff ); // for 300 baudrate 0xC35&0xFF => 0x35 
	/*U0DLx(L and M) is part of the UART0 Baud Rate Generator and holds the value used to divide the VPB clock (pclk)
	in order to produce the baud rate clock, which must be 16x desired baud rate */
	ulDivisor >>= 8; //0xC for 300 baudrate
	U0DLM = ( unsigned char ) ( ulDivisor & ( unsigned long ) 0xff ); //0xC&0xFF => 0xC
	//together U0DLM & U0DLL give the 0xC35 lath divisior
	U0LCR  &= (~mDIVISOR_LATCH_ACCES_BIT);                       // DLAB = 0 U0LCR- Line Control Regiser (word length/stop bit etc.)
	U0IER  |= mRX_DATA_AVALIABLE_INTERRUPT_ENABLE | mTHRE_INTERRUPT_ENABLE ;               

	// INT
	VICVectAddr1  = (unsigned long) UART0_Interrupt;             // set interrupt service routine address
	VICVectCntl1  = mIRQ_SLOT_ENABLE | VIC_UART0_CHANNEL_NR;     // use it for UART 0 Interrupt
	VICIntEnable |= (0x1 << VIC_UART0_CHANNEL_NR);               // Enable UART 0 Interrupt Channel
}
///////////////////////////////////////////////////////////////////
