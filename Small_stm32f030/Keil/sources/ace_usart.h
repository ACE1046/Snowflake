/* **************************************************************************/
/* ACE STM32F0xx USART library                                              */
/* 06.10.2011                                                               */
/* **************************************************************************/

#include "stm32f0xx.h"

#define USEUSART1

//#define USART1_REMAP

#define RX_BUF_SIZE1 20
#define TX_BUF_SIZE1 22

// putchar will wait for space in output queue
#define BLOCKING

#ifdef USEUSART1
void USART1_init(int BaudRate, char UseTXE);
void USART1_putchar(unsigned char c);
unsigned char USART1_getchar(void);
unsigned short USART1_inbuf(void);
#endif
