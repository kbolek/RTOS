#ifndef UART_H
#define UART_H

#define RECIEVER_SIZE 12
#define TRANSMITER_SIZE 20
#define CR 0x0D


void UART_InitWithInt(unsigned int uiBaudRate);
void Uart_GetString(char *acBuffer);
void Uart_PutString(char *acBuffer);

#endif

