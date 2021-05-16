#ifndef UART_H
#define UART_H

#define RECIEVER_SIZE 12
#define CR 0x0D
#define TRANSMITER_SIZE 20

enum eRecieverStatus { EMPTY, READY, OVERFLOW};
void UART_InitWithInt(unsigned int uiBaudRate);
enum eRecieverStatus eReciever_GetStatus(void);
void Reciever_GetStringCopy(char * ucDestination);
enum eTransmiterStatus {FREE, BUSY};
void Transmiter_SendString(char cString[]);
enum eTransmiterStatus Transmiter_GetStatus(void);

#endif

