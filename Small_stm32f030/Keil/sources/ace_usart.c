/* **************************************************************************/
/* ACE STM32F0xx USART library                                              */
/* 06.10.2011                                                               */
/* 02.07.2012                                                               */
/* 10.03.2015 Epic bug fixed: usart1_tx_buf[tx_buf_wr1++]=c in putchar      */
/* 24.05.2016 USART1 remap option                                           */
/* 01.06.2016 fixed bug in IRQ (unnessesery CR clear)                       */
/* 02.06.2016 rewritten without StdLib usage                                */
/* 27.12.2016 modified from F10x to F0xx                                    */
/* **************************************************************************/

#include "ace_usart.h"
#include "ace_pins.h"

#ifndef USART1_REMAP
	#define USART1_RX_PIN GPIOA, 10
	#define USART1_TX_PIN GPIOA, 9
	#define USART1_TXE_PIN GPIOA, 7
#else
	#define USART1_RX_PIN GPIOA, 3
	#define USART1_TX_PIN GPIOA, 2
	#define USART1_TXE_PIN GPIOA, 4
#endif

#ifdef USEUSART1
unsigned char usart1_rx_buf[RX_BUF_SIZE1];
unsigned char usart1_tx_buf[TX_BUF_SIZE1];
volatile int rx_buf_rd1, rx_buf_wr1; // Read and write cursors for rx buffer
volatile int tx_buf_rd1, tx_buf_wr1; // Read and write cursors for tx buffer
char usart1_use_txe; // Use GPIOA.Pin8 as TXE for RS-485

void USART1_init(int BaudRate, char UseTXE)
{
    //USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
		RCC_ClocksTypeDef RCC_ClocksStatus;

    rx_buf_rd1=rx_buf_wr1=tx_buf_rd1=tx_buf_wr1=0;
    usart1_use_txe=UseTXE;

    /* Enable GPIOA, USART1 clock                                           */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    /* Configure USART1 Rx (PA10/PB7) as input floating                         */
		GPIO_INIT_AF(USART1_RX_PIN, GPIO_AF_1);
    GPIO_INIT_PIN(USART1_RX_PIN, GPIO_MODE_INPUT_FLOATING);

    if (UseTXE)
    {
        /* Configure USART1 TxE (PA8/PB5) as push-pull            */
				GPIO_INIT_PIN(USART1_TXE_PIN, GPIO_MODE_OUTPUT_PUSH_PULL_HIGH);
    }

#ifdef USART1_REMAP
	  GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);
#endif

		/* Disable USART */
		USART1->CR1 &= (uint32_t)~((uint32_t)USART_CR1_UE);

		USART1->CR2 = (USART1->CR2 & 0x000040EF) +
			0x0000+ // 0x0000 - 1 stop bit, 0x1000 - reserved, 0x2000 - 2 stop bits, 0x3000 - 1.5 stop bits
			0+ // 0x0800 - clock enable
			0+ // 0x0700 - clock options
			0+ // 0x00E0 - reserved
			0+ // 0x0010 - ADDM7
			0; // 0x000F - reserved
			
		USART1->CR1 = (USART1->CR1 & 0xE0000002) +
			0+  // 0x0000 - 8 bit, 0x1000 - 9 bit
			0+  // 0x0800 - address mark wakeup
			0+  // 0x0000 - no parity, 0x0400 - parity
			0+  // 0x0000 - even parity, 0x0200 - odd parity
			0+  // 0x0100 - PE interrupt enable
			0+  // 0x0080 - TXE interrupt enable
			0+  // 0x0040 - TC interrupt enable
			USART_CR1_RXNEIE+ // 0x0020 - RXNE interrupt enable
			0+  // 0x0010 - IDLE interrupt enable
			USART_CR1_TE+ // 0x0008 - transmitter enable
			USART_CR1_RE+ // 0x0004 - receiver enable
			0+ // 0x0002 - reserved
			0; // disable uart, 0x0001 - enable

    if (usart1_use_txe) USART1->CR1 |= USART_CR1_TCIE; // включаем прерывание на окончание передачи

		USART1->CR3=0; // no hardware flow control, CTS, RTS disable, DMA, IrDA disable
		
		RCC_GetClocksFreq(&RCC_ClocksStatus);
		USART1->BRR=RCC_ClocksStatus.USART1CLK_Frequency / BaudRate;

		USART1->CR1 |= USART_CR1_UE; // Enable USART

    /* Configure USART1 Tx (PA9/PB6) as alternate function open drain with pull up            */
		GPIO_INIT_AF(USART1_TX_PIN, GPIO_AF_1);
		GPIO_INIT_PIN(USART1_TX_PIN, GPIO_MODE_AF_OPEN_DRAIN_UP_HIGH);

//		USART_InitStructure.USART_BaudRate            = Speed;
//    USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
//    USART_InitStructure.USART_StopBits            = USART_StopBits_1;
//    USART_InitStructure.USART_Parity              = USART_Parity_No ;
//    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//    USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
//    USART_Init(USART1, &USART_InitStructure);
//    USART_Cmd(USART1, ENABLE);

    /* Enabling interrupts */
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; // канал
    NVIC_InitStructure.NVIC_IRQChannelPriority = 1; // приоритет
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; // включаем канал
    NVIC_Init(&NVIC_InitStructure); // инициализируем
//    //USART_ITConfig(USART1, USART_IT_TXE, ENABLE);  // включаем прерывание на передачу
//    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);  // включаем прерывание на прием
//    if (usart1_use_txe)
//        USART_ITConfig(USART1, USART_IT_TC, ENABLE);  // включаем прерывание на окончание передачи
}

void USART1_SendNextByte(void) 
{
    if (tx_buf_wr1 != tx_buf_rd1)
    {
        if (usart1_use_txe) GPIO_SET(USART1_TXE_PIN);
        USART1->TDR=usart1_tx_buf[tx_buf_rd1];
        tx_buf_rd1=(tx_buf_rd1+1) % TX_BUF_SIZE1;
    } else
    {
        // USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
				USART1->CR1 &= ~(uint32_t)USART_CR1_TXEIE;
    }
}

void USART1_IRQHandler(void)
{ 
	volatile uint32_t d;
    if (USART1->ISR & USART_ISR_TXE)
    {
        USART1_SendNextByte();
    }

    if (USART1->ISR & USART_ISR_TC)
    {
        //USART_ClearITPendingBit(USART1, USART_IT_TC);
				USART1->ICR |= USART_ICR_TCCF;
        if (usart1_use_txe) GPIO_RESET(USART1_TXE_PIN);
    }

    if ((USART1->ISR & USART_ISR_RXNE) || (USART1->ISR & USART_ISR_ORE))
    {
			if ((USART1->ISR & (USART_ISR_NE | USART_ISR_FE | USART_ISR_PE)) == 0)
      {  
				if (((rx_buf_wr1+1) % RX_BUF_SIZE1) != rx_buf_rd1)
				{
						usart1_rx_buf[rx_buf_wr1]=USART1->RDR;
						rx_buf_wr1=(rx_buf_wr1 +1) % RX_BUF_SIZE1;
				} else
				{
					// input buffer overflow
					d=USART1->RDR;
				}
			}
			else
			{
				// Add '?' on error
						//usart1_rx_buf[rx_buf_wr1]='?';
						//rx_buf_wr1=(rx_buf_wr1 +1) % RX_BUF_SIZE1;
					d=USART1->RDR;
			}
    }
}

void USART1_putchar(unsigned char c)
{
	#ifdef BLOCKING
    while (((tx_buf_wr1+1) % TX_BUF_SIZE1) == tx_buf_rd1) ;
	#else
    if (((tx_buf_wr1+1) % TX_BUF_SIZE1) == tx_buf_rd1) return;
	#endif
    usart1_tx_buf[tx_buf_wr1]=c;
		tx_buf_wr1=(tx_buf_wr1+1) % TX_BUF_SIZE1;

		USART1->CR1 |= USART_CR1_TXEIE;
}

unsigned char USART1_getchar()
{
	unsigned char r;
    if (rx_buf_wr1 != rx_buf_rd1)
    {
        r=usart1_rx_buf[rx_buf_rd1];
        rx_buf_rd1=(rx_buf_rd1+1) % RX_BUF_SIZE1;
    } else
		r=0;

	return r;
}

unsigned short USART1_inbuf()
{
	return ((rx_buf_wr1+RX_BUF_SIZE1-rx_buf_rd1) % RX_BUF_SIZE1);
}
#endif
