/*
 * ws281x.c - init & comm api for the ws281x PWM RGB LED
 * 04-01-15 E. Brombaugh
 */

#include "ws281x.h"
#include <stm32f0xx.h>
#include <stm32f0xx_rcc.h>
#include <stm32f0xx_gpio.h>
#include <stm32f0xx_tim.h>
#include <stm32f0xx_dma.h>


#define WS_BUFLEN (WS_MAX_LEDS*3*8+42)

#define SYNC_DEBUG

uint16_t ws_buffer[WS_BUFLEN];
uint8_t ws_status;

/*
 * Initialize the WS281x interface
 */
void ws281x_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	DMA_InitTypeDef   DMA_InitStructure;
	uint16_t i;
	
	/* turn on clock for WS281x GPIO */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	
	/* GPIO PA6 is output for WS281x data from TIM16_CH1 */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_5);
	
#ifdef SYNC_DEBUG
	/* GPIO PA7 is output for GPIO - sync flag for debug */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
#endif
	
	/* Timer 16 clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM16 , ENABLE);
	
	/* Time Base configuration for 800kHz */
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_Period = 59;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM16, &TIM_TimeBaseStructure);

	/* Channel 1 Configuration in PWM1 mode with preload */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
	TIM_OCInitStructure.TIM_Pulse = 18;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
	TIM_OC1Init(TIM16, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM16, TIM_OCPreload_Enable);
	
	/* TIM16 Main Output Enable */
	TIM_CtrlPWMOutputs(TIM16, ENABLE);

	/* TIM16 DMA enable on CC1 */
	TIM_SelectCCDMA(TIM16, ENABLE);
	TIM_DMACmd(TIM16, TIM_DMA_CC1, ENABLE);
	
	/* DMA1 clock enable */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	/* DMA1 Channel3 Config */
	DMA_DeInit(DMA1_Channel3);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(TIM16->CCR1);
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ws_buffer;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_BufferSize = WS_BUFLEN;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel3, &DMA_InitStructure);
	
	/* DMA1 Channel3 enable terminal count interrupt */
	DMA_ITConfig(DMA1_Channel3, DMA_IT_TC, ENABLE);
	
	/* Enable DMA1 chl3 IRQ for end of buffer */
	NVIC_EnableIRQ(DMA1_Channel2_3_IRQn);
	
	/* clear the buffer */
	for(i=0;i<WS_BUFLEN;i++)
		ws_buffer[i] = 0;
	
	/* status is not busy */
	ws_status = 0;
}

/*
 * Send a burst to the WS281x chain
 */
void ws281x_send(uint8_t rgb_data[], uint8_t num_leds)
{
	uint8_t j,k, byte, color[3];
	uint16_t l, i;
	
	/* limit to our buffer size */
	num_leds = num_leds > WS_MAX_LEDS ? WS_MAX_LEDS : num_leds;
	
	/* load buffer with pulsewidths from rgb data */
	l=0;
	ws_buffer[l++] = 0;	/* dummy bit to start */
	ws_buffer[l++] = 0;	/* dummy bit to start */
	for(i=0;i<num_leds;i++)	/* loop over LEDs */
	{
		color[0] = rgb_data[1+3*i];	/* green */
		color[1] = rgb_data[0+3*i];	/* red   */
		color[2] = rgb_data[2+3*i];	/* blue  */
		
		/* convert colors to bits */
		for(j=0;j<3;j++)	/* loop over colors (GRB) */
		{
			/* no gain */
			byte = color[j];

			for(k=0;k<8;k++)	/* loop over 8 bits, msb first */
			{
				ws_buffer[l++] = (byte&0x80) ? 34 : 18;
				byte <<= 1;
			}
		}
	}
	
	/* remaining (up to last 42) words in buffer are all 0 for delay */
	while(l<WS_BUFLEN)
		ws_buffer[l++] = 0;
	
	/* set length */
	DMA_SetCurrDataCounter(DMA1_Channel3, WS_BUFLEN);
	
	/* DMA1 Channel3 enable */
	DMA_Cmd(DMA1_Channel3, ENABLE);
	
	/* clear counter for clean start */
	TIM_SetCounter(TIM16, 0);
	TIM_SetCompare1(TIM16, 0);
		
#ifdef SYNC_DEBUG
	/* Active send */
	GPIOA->ODR |= (1<<7);
#endif
	
	/* Enable Timer */
	TIM_Cmd(TIM16, ENABLE);

	/* set busy status */
	ws_status = 1;
}

/* Handles DMA1_Channel3 interrupt request */
void DMA1_Channel2_3_IRQHandler(void)
{
	if(DMA_GetFlagStatus(DMA1_FLAG_TC3) == SET)
	{
		/* Disable Timer */
		TIM_Cmd(TIM16, DISABLE);

		/* DMA1 Channel3 disable */
		DMA_Cmd(DMA1_Channel3, DISABLE);
		
		/* Clear DMA1_Channel3 interrupts */
		DMA_ClearFlag(DMA1_FLAG_TC3);

		/* not busy */
		ws_status = 0;
		
#ifdef SYNC_DEBUG
		/* Inactive send */
		GPIOA->ODR &= ~(1<<7);
#endif
	}
}

/*
 * Convert HSV triple to RGB triple
 * use algorithm from
 * http://en.wikipedia.org/wiki/HSL_and_HSV#Converting_to_RGB
 */
void ws281x_hsv2rgb(const hsv HSV, uint8_t rgb[])
{
	uint16_t C;
	int16_t Hprime, Cscl;
	uint8_t hs, X, m;
	
	/* default */
	rgb[0] = 0;
	rgb[1] = 0;
	rgb[2] = 0;
	
	/* calcs are easy if v = 0 */
	if(HSV[2] == 0)
		return;
	
	/* C is the chroma component */
	C = ((uint16_t)HSV[1] * (uint16_t)HSV[2])>>8;
	
	/* Hprime is fixed point with range 0-5.99 representing hue sector */
	Hprime = (int16_t)HSV[0] * 6;
	
	/* get intermediate value X */
	Cscl = (Hprime%512)-256;
	Cscl = Cscl < 0 ? -Cscl : Cscl;
	Cscl = 256 - Cscl;
	X = ((uint16_t)C * Cscl)>>8;
	
	/* m is value offset */
	m = HSV[2] - C;
	
	/* get the hue sector (1 of 6) */
	hs = (Hprime)>>8;
	
	/* map by sector */
	switch(hs)
	{
		case 0:
			/* Red -> Yellow sector */
			rgb[0] = C + m;
			rgb[1] = X + m;
			rgb[2] = m;
			break;
		
		case 1:
			/* Yellow -> Green sector */
			rgb[0] = X + m;
			rgb[1] = C + m;
			rgb[2] = m;
			break;
		
		case 2:
			/* Green -> Cyan sector */
			rgb[0] = m;
			rgb[1] = C + m;
			rgb[2] = X + m;
			break;
		
		case 3:
			/* Cyan -> Blue sector */
			rgb[0] = m;
			rgb[1] = X + m;
			rgb[2] = C + m;
			break;
		
		case 4:
			/* Blue -> Magenta sector */
			rgb[0] = X + m;
			rgb[1] = m;
			rgb[2] = C + m;
			break;
		
		case 5:
			/* Magenta -> Red sector */
			rgb[0] = C + m;
			rgb[1] = m;
			rgb[2] = X + m;
			break;
	}
}
