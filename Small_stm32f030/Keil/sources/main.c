#include "stm32f0xx.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "ace_usart.h"
#include "ace_pins.h"


#include <stm32f0xx_gpio.h>
#include <stm32f0xx_adc.h>
#include <stm32f0xx_crc.h>

#include "ws281x.h"

#define COMPORT_BAUD 115200
#define BLINK GPIOA, 5

#define TEST GPIOA, 7

const uint32_t time_effect=5000; // one effect duration in ms
const uint32_t time_fade=3000; // fade duration between effects in ms
const uint32_t time_loop=1200; // effect loops every X ms

volatile uint32_t systick=0;
void SysTick_Handler(void)
{
  systick++;
}

uint32_t getTrueRandomNumber(void) {
  uint8_t i;

  ADC_InitTypeDef ADC_InitStructure;

  //enable ADC1 clock
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

  // Initialize ADC 14MHz RC
  RCC_ADCCLKConfig(RCC_ADCCLK_HSI14);
  RCC_HSI14Cmd(ENABLE);
  while (!RCC_GetFlagStatus(RCC_FLAG_HSI14RDY))
    ;

  ADC_DeInit(ADC1);
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Backward;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_TRGO; //default
  ADC_Init(ADC1, &ADC_InitStructure);

  //enable internal channel
  ADC_TempSensorCmd(ENABLE);

  // Enable ADCperipheral
  ADC_Cmd(ADC1, ENABLE);
  while (ADC_GetFlagStatus(ADC1, ADC_FLAG_ADEN) == RESET)
    ;

  ADC1->CHSELR = 0; //no channel selected
  //Convert the ADC1 temperature sensor, user shortest sample time to generate most noise
  ADC_ChannelConfig(ADC1, ADC_Channel_TempSensor, ADC_SampleTime_1_5Cycles);

  // Enable CRC clock 
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);

  for (i = 0; i < 8; i++) {
    //Start ADC1 Software Conversion
    ADC_StartOfConversion(ADC1);
    //wait for conversion complete
    while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)) {
    }

    CRC_CalcCRC(ADC_GetConversionValue(ADC1));
    //clear EOC flag
    ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
  }

  //disable ADC1 to save power
  ADC_Cmd(ADC1, DISABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, DISABLE);

  return CRC_CalcCRC(0xBADA55E5);
}

void Init(void)
{
	
  /* Select HSI as system clock source */   
  RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);   
   
	RCC_PLLCmd(DISABLE);
	RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_12);
	RCC_PLLCmd(ENABLE);
	while(!RCC_GetFlagStatus(RCC_FLAG_PLLRDY));
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);	 

  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,  ENABLE);

	//GPIO_INIT_PIN(BLINK, GPIO_MODE_OUTPUT_PUSH_PULL_LOW);
//	GPIO_INIT_PIN(BTN2, GPIO_MODE_INPUT_PULL_UP);
//	GPIO_INIT_PIN(BTN3, GPIO_MODE_INPUT_PULL_UP);

 
	USART1_init(COMPORT_BAUD, 0);
	SysTick_Config(SystemCoreClock/1000);
}

/* WS281x RGB buffer */
uint8_t rgb_data[3*WS_MAX_LEDS];

typedef hsv palette[6];

const palette rainbow={
	{0, 255, 255},
	{43, 255, 255},
	{85, 255, 255},
	{128, 255, 255},
	{171, 255, 255},
	{213, 255, 255}
};

const palette lightblue={
	{165, 90, 205},
	{160, 120, 215},
	{155, 170, 225},
	{160, 200, 235},
	{165, 230, 245},
	{170, 255, 255}
};

const palette darkblue={
	{175, 255, 255},
	{165, 255, 200},
	{160, 255, 150},
	{160, 255, 100},
	{155, 255,  50},
	{150, 255, 20}
};

const palette fire={
	{0, 255, 255},
	{10, 255, 205},
	{20, 255, 155},
	{30, 255, 105},
	{20, 255, 155},
	{10, 255, 205}
};

const palette green_flash={
	{75, 255, 255},
	{75, 255, 0},
	{70, 255, 255},
	{70, 255, 0},
	{65, 255, 255},
	{65, 255, 0},
};

const palette ryg_flash={
	{0, 255, 255},
	{70, 255, 0},
	{70, 255, 255},
	{40, 255, 0},
	{40, 255, 255},
	{0, 255, 0},
};

const palette pastel={
	{40, 255, 255},
	{00, 155, 255},
	{70, 255, 255},
	{200, 155, 255},
	{50, 255, 255},
	{120, 155, 255},
};

const palette blueyellow={
	{160, 255, 255},
	{120, 255, 255},
	{100, 255, 255},
	{80, 255, 255},
	{60, 255, 255},
	{40, 255, 255},
};

const palette cocacola={
	{00, 255, 255},
	{00, 255, 255},
	{00, 255, 255},
	{00, 0, 255},
	{00, 0, 255},
	{00, 0, 255},
};

const palette russia={
	{00, 0, 255},
	{00, 0, 255},
	{160, 255, 255},
	{160, 255, 255},
	{00, 255, 255},
	{00, 255, 255},
};

const palette rainbow2={
	{00, 0, 255},
	{00, 0, 255},
	{160, 255, 255},
	{160, 255, 255},
	{00, 255, 255},
	{00, 255, 255},
};

const uint8_t led_coord_r[WS_MAX_LEDS]={
	20,
	85, 170, 255, 230, 230,
	85, 170, 255, 230, 230,
	85, 170, 255, 230, 230,
	85, 170, 255, 230, 230,
	85, 170, 255, 230, 230,
	85, 170, 255, 230, 230
};

const uint8_t led_coord_a[WS_MAX_LEDS]={
	0,
	  0,   0,   0, 240, 15,
	 43,  43,  43,  28, 58,
	 85,  85,  85,  70, 100,
	128, 128, 128, 113, 143,
	171, 171, 171, 156, 186,
	213, 213, 213, 198, 228
};

const uint8_t led_coord_br[WS_MAX_LEDS]={
	64,
	  0,   0,   0, 128, 128,
	  0,   0,   0, 128, 128,
	  0,   0,   0, 128, 128,
	  0,   0,   0, 128, 128,
	  0,   0,   0, 128, 128,
	  0,   0,   0, 128, 128
};

uint8_t SatAdd(uint8_t i1, uint8_t i2)
{
	int i=(int)i1+i2;
	if (i>255) return 255;
	return i;
}

void Gamma(hsv *c)
{
	if (*c[2]==0)  *c[2]=0; else
	if (*c[2]<30)  *c[2]=1; else
	if (*c[2]<60)  *c[2]=2; else
	if (*c[2]<90)  *c[2]=4; else
	if (*c[2]<120) *c[2]=8; else
	if (*c[2]<150) *c[2]=16; else
	if (*c[2]<180) *c[2]=32; else
	if (*c[2]<210) *c[2]=64; else
	if (*c[2]<240) *c[2]=128; else
	            *c[2]=255;
}

uint8_t Blend(uint8_t i1, uint8_t i2, uint8_t percent)
{
	int i=i1+(int8_t)(i2-i1)*percent/255;
	return i;
}

uint8_t SatBlend(uint8_t i1, uint8_t i2, uint8_t percent)
{
	int i;
	if (i2>i1) i=i1+(uint8_t)(i2-i1)*percent/255;
	else i=i1-(uint8_t)(i1-i2)*percent/255;
	if (i>255) return 255;
	return i;
}

void ColorAdd(const hsv c1, const hsv c2, hsv res)
{
	res[0]=((uint16_t)c1[0]+c2[0])/2;
	res[1]=((uint16_t)c1[1]+c2[1])/2;
	res[2]=SatAdd(c1[2], c2[2]);
}
	
void ColorBlend(const hsv c1, const hsv c2, uint8_t percent, hsv res)
{
	res[0]=Blend(c1[0], c2[0], percent);
	res[1]=SatBlend(c1[1], c2[1], percent);
	res[2]=SatBlend(c1[2], c2[2], percent);
}
	
void AnimRainbowAll(int phase, const palette p, hsv *vals, int param)
{
	int i, s, f;
	if (param%2) phase=256*6-phase-1;
	for (i=0; i<31; i++)
	{		
		f=i;
		if ((param/2)%2) f=31-f;
		s=(phase+f*256*6/31)/256;
		f=(phase+f*256*6/31)%256;
		ColorBlend(p[s%6], p[(s+1)%6], f, vals[i]);
	}
}

void AnimRainbowRadial(int phase, const palette p, hsv *vals, int param)
{
	int i, s, f;
	if (param%2) phase=256*6-phase-1;
	for (i=0; i<31; i++)
	{		
		f=led_coord_r[i];
		if ((param/2)%2) f=255-f;
		s=(phase+f*230*6/256)/256;
		f=(phase+f*230*6/256)%256;
		ColorBlend(p[s%6], p[(s+1)%6], f, vals[i]);
	}
}

void AnimRainbowRadialMix(int phase, const palette p, hsv *vals, int param)
{
	int i, s, f;
	if (param%2) phase=256*6-phase-1;
	for (i=0; i<31; i++)
	{		
		f=led_coord_r[i];
		if (((led_coord_a[i]+20)/43)%2) f=255-f;
		s=(phase*2+f*230*6/256)/256;
		f=(phase*2+f*230*6/256)%256;
		ColorBlend(p[s%6], p[(s+1)%6], f, vals[i]);
	}
}

void AnimRainbowClock(int phase, const palette p, hsv *vals, int param)
{
	int i, s, f;
	if (param%2) phase=256*6-phase-1;
	for (i=0; i<31; i++)
	{		
		f=led_coord_a[i];
		if ((param/8)%2 && led_coord_br[i]<30) f=255-f;
		if ((param/2)%2) f=255-f;
		s=(phase+f*6)/256;
		f=(phase+f*6)%256;
		ColorBlend(p[s%6], p[(s+1)%6], f, vals[i]);
		if ((param/4)%2 && led_coord_br[i]<30) { vals[i][2]=param/7%5; vals[i][1]/=param/11%3; }
	}
}

void AnimRainbowSpiral(int phase, const palette p, hsv *vals, int param)
{
	int i, s, f;
	if (param%2) phase=256*6-phase-1;
	for (i=0; i<31; i++)
	{		
		f=(led_coord_a[i]+led_coord_r[i]/4)%256;
		if ((param/2)%2) f=255-f;
		s=(phase+f*6)/256;
		f=(phase+f*6)%256;
		ColorBlend(p[s%6], p[(s+1)%6], f, vals[i]);
	}
}

void AnimRainbowTree(int phase, const palette p, hsv *vals, int param)
{
	int i, s, f;
	if (param%2) phase=256*6-phase-1;
	for (i=0; i<31; i++)
	{		
		f=led_coord_br[i];
		if ((param/2)%2) f=255-f;
		s=(phase+f*6)/256;
		f=(phase+f*6)%256;
		ColorBlend(p[s%6], p[(s+1)%6], f, vals[i]);
	}
}

void AnimFlash(int phase, const palette p, hsv *vals, int param)
{
	int i, l, c;
	for (i=0; i<31; i++)
	{
		c=vals[i][2];
		c-=7;
		if (c<0) c=0;
		vals[i][2]=c;
	}
	if (rand()%16 > 7)
	{
		c=rand()%6;
		l=rand()%WS_MAX_LEDS;
		vals[l][0]=p[c][0];
		vals[l][1]=p[c][1];
		vals[l][2]=p[c][2];
	}
}

void AnimWaveRadial(int phase, const palette p, hsv *vals, int param)
{
	int i, s, f;
	int width=30+param%80;
	if (param%2) phase=256*6-phase-1;
	//phase=phase*2%(256*6);
	for (i=0; i<31; i++)
	{		
		f=led_coord_r[i];
		if ((param/2)%2) f=255-f;
		s=phase/6;
		s=s-f;
		if (s < 0) s=-s;
		if (s>128) s=255-s;
		if (s>width) { vals[i][2]=0; }
	  else
		{
			f=(led_coord_a[i]+param%253)%256;
			if (f>128) f=256-f;
			ColorBlend(p[phase/256%6], p[(phase/256+1)%6], phase%256, vals[i]);
			vals[i][2]=(width-s)*255/width;
			Gamma(&vals[i]);
			vals[i][0]=vals[i][0] + (uint8_t)((f-64)*(param%35)/128);
		}
	}
}



#define EFFECTS 8
void Anim(int effect, int phase, const palette p, hsv *vals, int param)
{
	switch (effect)
	{
		case 0: AnimRainbowAll(phase, p, vals, param); break;
		case 1: AnimRainbowRadial(phase, p, vals, param); break;
		case 2: AnimRainbowClock(phase, p, vals, param); break;
		case 3: AnimRainbowRadialMix(phase, p, vals, param); break;
		case 4: AnimFlash(phase, p, vals, param); break;
		case 5: AnimWaveRadial(phase, p, vals, param); break;
		case 6: AnimRainbowSpiral(phase, p, vals, param); break;
		case 7: AnimRainbowTree(phase, p, vals, param); break;
	}
}

hsv frame[2][31];

int main(void)
{
	int32_t i, n, f;
	uint32_t last, st, loop_start, effect_start1, effect_start2;
	int effect1, effect2, effect1_param, effect2_param;
	const palette *p1, *p2;
	Init();
	ws281x_init();
	
	srand(getTrueRandomNumber());
	
	USART1_putchar('\n');
	USART1_putchar('S');
	USART1_putchar('t');
	USART1_putchar('a');
	USART1_putchar('r');
	USART1_putchar('t');
	USART1_putchar('\n');
	
	//for (i=0; i<sizeof(rgb_data); i++) rgb_data[i]=0;
	//for (i=0; i<2; i++) ws281x_hsv2rgb(rainbow[i], &(rgb_data[i*3]));
//	rgb_data[3]=255;
//	rgb_data[8]=255;
//	rgb_data[10]=255;

//	rgb_data[12]=255;
//	rgb_data[13]=255;
//	rgb_data[15]=255;
//	rgb_data[17]=255;
  n=0;
	f=0;
	last=0;
	loop_start=0;
	effect_start1=0;
	effect_start2=0;
	effect1_param=11;
	effect2_param=0;
	effect1=2;
	effect2=2;
	p1=p2=&rainbow;

//	ws281x_send(rgb_data, WS_MAX_LEDS);
//	for(i=0;i<1000000;i++) ;
//	p1=&darkblue;
//	AnimRainbowAll(0, *p1, frame[0], 110);
//	for (i=0; i<31; i++)
//		ws281x_hsv2rgb(frame[0][i], &(rgb_data[i*3]));
//	ws281x_send(rgb_data, WS_MAX_LEDS);
//	while(1);

	while(1)
	{
		if (systick-last > 1000) 
		{
			USART1_putchar('.');
			last+=1000;
		}


		ws281x_send(rgb_data, WS_MAX_LEDS);
		
	  st=systick;
		while (st-loop_start >= time_loop) { loop_start+=time_loop; f++;}
		n=((st-loop_start)*256*6)/time_loop;
		
		if (st-effect_start1 >= time_effect+time_fade && effect_start2==0)
		{ // fade start
			do
			{
				switch (rand()%10)
				{
					case 0: p2=&rainbow; break;
					case 1: p2=&lightblue; break;
					case 2: p2=&darkblue; break;
					case 3: p2=&fire; break;
					case 4: p2=&green_flash; break;
					case 5: p2=&ryg_flash; break;
					case 6: p2=&pastel; break;
					case 7: p2=&blueyellow; break;
					case 8: p2=&cocacola; break;
					case 9: p2=&russia; break;
					
				}
			}
			while (p1==p2);
			do
			{
				effect2=rand() % EFFECTS;
			}
			while (effect1==effect2);
//effect2=2;
			effect2_param=rand();
			effect_start2=effect_start1+time_effect+time_fade;
		}
		if (st-effect_start2 >= time_fade && effect_start2!=0)
		{ // fade ended
			effect1=effect2;
			p1=p2;
			effect1_param=effect2_param;
			effect_start1=effect_start2;
			for (i=0; i<WS_MAX_LEDS; i++)
			{
				frame[0][i][0]=frame[1][i][0];
				frame[0][i][1]=frame[1][i][1];
				frame[0][i][2]=frame[1][i][2];
			}
			effect_start2=0;
		}
		
		if (f==2) f=0;
		if (effect_start2>0)
		{
			Anim(effect1, n, *p1, frame[0], effect1_param);
			Anim(effect2, n, *p2, frame[1], effect2_param);
			for (i=0; i<31; i++)
			{
				int p=(st-effect_start2)*255 / time_fade;
				hsv c;
				ColorBlend(frame[0][i], frame[1][i], p, c);
				ws281x_hsv2rgb(c, &(rgb_data[i*3]));
			}
		}
		else
		{
			Anim(effect1, n, *p1, frame[0], effect1_param);
			for (i=0; i<31; i++)
				ws281x_hsv2rgb(frame[0][i], &(rgb_data[i*3]));
		}

GPIO_SET(TEST);
		for(i=0;i<100000;i++) ;
GPIO_RESET(TEST);
		for(i=0;i<100000;i++) ;
	}
}
