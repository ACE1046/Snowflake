/* **************************************************************************/
/* ACE STM32F0xx SPI library                                                */
/* 16.01.2017 modified from F10x to F0xx                                    */
/* **************************************************************************/

#include "stm32f0xx.h"
#include <stdint.h>
#include "ace_pins.h"
#include "ace_spi.h"
#include <string.h>

#define SSEL0 SSEL_PORT->BSRR = GPIO_BSRR_BR(SSEL_PIN)
#define SSEL1 SSEL_PORT->BSRR = GPIO_BSRR_BS(SSEL_PIN)

#define PINSS GPIOB, 1
#define SCK GPIOA, 5
#define MISO GPIOA, 6
#define MOSI GPIOA, 7

#define RCC_APB2Periph_SPIx RCC_APB2Periph_SPI1

uint8_t buf_tx[9]; // буфер, из которого идёт отправка данных
uint8_t buf_in[9]; // буфер, принимающий данные
uint8_t inbuf; // принято байт
uint8_t DMA_started; // DMA запущен

void InitSPI(void)
{
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

	/* Configure SCK pin as Push Pull */
	GPIO_INIT_AF(SCK, GPIO_AF_0);
	GPIO_INIT_PIN(SCK, GPIO_MODE_AF_PUSH_PULL_HIGH);
	/* Configure MOSI pin as Push Pull */
	GPIO_INIT_AF(MOSI, GPIO_AF_0);                                                                         
	GPIO_INIT_PIN(MOSI, GPIO_MODE_AF_PUSH_PULL_HIGH);
	/* Configure MISO pin as input */
	GPIO_INIT_AF(MISO, GPIO_AF_0);
	GPIO_INIT_PIN(MISO, GPIO_MODE_AF_PUSH_PULL_HIGH);
	/* Configure CS pin as input */
	GPIO_INIT_PIN(PINSS, GPIO_MODE_OUTPUT_PUSH_PULL_HIGH);

	SPI1->CR1 =
	  0 * SPI_CR1_BIDIMODE |  // 0 - 2-line, 1 - 1-line
		0 * SPI_CR1_BIDIOE |    // 1-line mode: 0 - receive only, 1 - transmit only
		0 * SPI_CR1_CRCEN |     // CRC enabled
		0 * SPI_CR1_CRCNEXT |   // Send CRC next
		0 * SPI_CR1_CRCL |      // CRC len
		0 * SPI_CR1_RXONLY |    // 0 - full duplex
		1 * SPI_CR1_SSM |       // software slave management, 0 - disabled
		1 * SPI_CR1_SSI |       // software slave select
		0 * SPI_CR1_LSBFIRST |  // 1 - LSB transmitted and received first
		0 * SPI_CR1_SPE |       // SPI enable
		1 * SPI_CR1_BR_2 |      // SPI baud rate, 000 - fPCLK / 2 ... 111 - fPCLK / 256
		1 * SPI_CR1_BR_1 |
		1 * SPI_CR1_BR_0 |
		1 * SPI_CR1_MSTR |      // 1 - master, 0 - slave
		1 * SPI_CR1_CPOL |      // polarity
		1 * SPI_CR1_CPHA;       // phase
		
	SPI1->CR2 =
	  1 * SPI_CR2_FRXTH |     // FIFO reception threshold, 0 - 16-bit, 1 - 8-bit
	  0 * SPI_CR2_DS_3 |      // Data size. 0111 - 8-bit
	  1 * SPI_CR2_DS_2 |
	  1 * SPI_CR2_DS_1 |
	  1 * SPI_CR2_DS_0 |
		0 * SPI_CR2_TXEIE |     // Tx buffer empty interrupt enable
		0 * SPI_CR2_RXNEIE |    // RX buffer not empty interrupt enable
		0 * SPI_CR2_ERRIE |     // Error interrupt enable
		0 * SPI_CR2_FRF |       // 0: SPI Motorola mode, 1: SPI TI mode
		0 * SPI_CR2_NSSP |      // NSS pulse
		0 * SPI_CR2_SSOE | 			// SS output enable
		0 * SPI_CR2_TXDMAEN |   // Tx buffer DMA enable
		0 * SPI_CR2_RXDMAEN;    // Rx buffer DMA enable
		
	SPI1->CR1 |= SPI_CR1_SPE;
}

#define SPI1_DR_8b          (*(__IO uint8_t *)((uint32_t)&(SPI1->DR)))
#define SPI1_DR_8bit (*(__IO uint8_t *)((uint32_t)&(SPI1->DR)))
uint8_t SpiSend(uint8_t b)
{
	while (!(SPI1->SR & SPI_SR_TXE));
	SPI1_DR_8bit = b;
	while (!(SPI1->SR & SPI_SR_RXNE));
	
	return (SPI1_DR_8bit);

//SPI1_DR_8b = b;
//  
//  while (!(SPI1->SR & SPI_SR_TXE));
//  while (SPI1->SR & SPI_SR_BSY);

//	return SPI1_DR_8b;
}

void SpiStart()
{
	GPIO_RESET(PINSS);
}

void SpiStop()
{
	GPIO_SET(PINSS);
}

void SpiFlush()
{
}


