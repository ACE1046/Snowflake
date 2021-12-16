// Snowflake decoration
// 2021 ACE ace@imlazy.ru
// If you want to compile this source in Arduino IDE, just rename it to snowflake.ino

#include <Arduino.h>

#include <FastLED.h>

#define LED_PIN     14
#define COLOR_ORDER GRB
#define CHIPSET     WS2812B
#define NUM_LEDS    150

#define BRIGHTNESS  255
#define FRAMES_PER_SECOND 60

#define Pi 3.14159

bool gReverseDirection = false;

CRGB leds[NUM_LEDS];

typedef CHSV palette[6];

#define LED 2

const uint32_t time_effect=5000; // one effect duration in ms
const uint32_t time_fade=3000; // fade duration between effects in ms
const uint32_t time_loop=1800; // effect loops every X ms

uint32_t getTrueRandomNumber(void) {
	return random(65536);
}

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

const palette green_fade={
	{75, 255, 255},
	{75, 255, 170},
	{70, 255, 90},
	{70, 255, 0},
	{65, 255, 90},
	{70, 255, 170},
};

const palette ryg_flash={
	{0, 255, 255},
	{70, 255, 0},
	{70, 255, 255},
	{40, 255, 0},
	{40, 255, 255},
	{0, 255, 0},
};

const palette ryg_fade={
	{0, 255, 255},
	{70, 255, 255},
	{70, 255, 255},
	{40, 255, 255},
	{40, 255, 255},
	{0, 255, 255},
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
	{205, 0, 255},
	{205, 0, 255},
	{160, 255, 255},
	{160, 255, 255},
	{0, 255, 255},
	{0, 255, 255},
};

const palette chrome={
	{180, 255, 200},
	{180, 180, 220},
	{180, 150, 255},
	{44, 255, 155},
	{44, 163, 235},
	{44, 95, 255},
};

// LEDs distance from center, 0-255
const uint8_t led_coord_r[NUM_LEDS]={
17, 34, 51, 68, 85, 102, 119, 136, 153, 170, 187, 204, 221, 238, 255,   180, 192, 204, 216, 228,    180, 192, 204, 216, 228,
17, 34, 51, 68, 85, 102, 119, 136, 153, 170, 187, 204, 221, 238, 255,   180, 192, 204, 216, 228,    180, 192, 204, 216, 228,
17, 34, 51, 68, 85, 102, 119, 136, 153, 170, 187, 204, 221, 238, 255,   180, 192, 204, 216, 228,    180, 192, 204, 216, 228,
17, 34, 51, 68, 85, 102, 119, 136, 153, 170, 187, 204, 221, 238, 255,   180, 192, 204, 216, 228,    180, 192, 204, 216, 228,
17, 34, 51, 68, 85, 102, 119, 136, 153, 170, 187, 204, 221, 238, 255,   180, 192, 204, 216, 228,    180, 192, 204, 216, 228,
17, 34, 51, 68, 85, 102, 119, 136, 153, 170, 187, 204, 221, 238, 255,   180, 192, 204, 216, 228,    180, 192, 204, 216, 228
};

// LEDs angle from vertical, 0-255
const uint8_t led_coord_a[NUM_LEDS]={
#define t 21
	t, t, t, t, t, t, t, t, t, t, t, t, t, t, t, t-3, t-6, t-9, t-12, t-15, t+3, t+6, t+9, t+12, t+15,
#undef t
#define t 64
	t, t, t, t, t, t, t, t, t, t, t, t, t, t, t, t-3, t-6, t-9, t-12, t-15, t+3, t+6, t+9, t+12, t+15,
#undef t
#define t 107
	t, t, t, t, t, t, t, t, t, t, t, t, t, t, t, t-3, t-6, t-9, t-12, t-15, t+3, t+6, t+9, t+12, t+15,
#undef t
#define t 150
	t, t, t, t, t, t, t, t, t, t, t, t, t, t, t, t-3, t-6, t-9, t-12, t-15, t+3, t+6, t+9, t+12, t+15,
#undef t
#define t 192
	t, t, t, t, t, t, t, t, t, t, t, t, t, t, t, t-3, t-6, t-9, t-12, t-15, t+3, t+6, t+9, t+12, t+15,
#undef t
#define t 235
	t, t, t, t, t, t, t, t, t, t, t, t, t, t, t, t-3, t-6, t-9, t-12, t-15, t+3, t+6, t+9, t+12, t+15
};

// LEDs relation to main branch (0) or to side (128)
const uint8_t led_coord_br[NUM_LEDS]={
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128
};

uint8_t led_coord_x[NUM_LEDS];
uint8_t led_coord_y[NUM_LEDS];

uint8_t SatAdd(uint8_t i1, uint8_t i2)
{
	int i=(int)i1+i2;
	if (i>255) return 255;
	return i;
}

void Gamma(CHSV *c)
{
	// if (*c[2]==0)  *c[2]=0; else
	// if (*c[2]<30)  *c[2]=1; else
	// if (*c[2]<60)  *c[2]=2; else
	// if (*c[2]<90)  *c[2]=4; else
	// if (*c[2]<120) *c[2]=8; else
	// if (*c[2]<150) *c[2]=16; else
	// if (*c[2]<180) *c[2]=32; else
	// if (*c[2]<210) *c[2]=64; else
	// if (*c[2]<240) *c[2]=128; else
	//             *c[2]=255;
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

void ColorAdd(const CHSV c1, const CHSV c2, CHSV res)
{
	res[0]=((uint16_t)c1[0]+c2[0])/2;
	res[1]=((uint16_t)c1[1]+c2[1])/2;
	res[2]=SatAdd(c1[2], c2[2]);
}
	
void ColorBlend(const CHSV c1, const CHSV c2, uint8_t percent, CHSV &res)
{
	uint8_t h1, h2;
	h1=c1[0];
	h2=c2[0];
	if (c2.v==0) h2=h1;
	if (c1.v==0) h1=h2;
	if (c2.s==0) h2=h1;
	if (c1.s==0) h1=h2;
	res[0]=Blend(h1, h2, percent);
	res[1]=SatBlend(c1[1], c2[1], percent);
	res[2]=SatBlend(c1[2], c2[2], percent);
}
	
void AnimColorTest(int phase, const palette p, CHSV *vals, int param)
{
	int i, s, f;
	for (i=0; i<NUM_LEDS; i++)
	{		
		f=led_coord_r[i];
		s=(f*230*6/256)/256;
		f=(f*230*6/256)%256;
		ColorBlend(p[s%6], p[(s+1)%6], f, vals[i]);
	}
}

void AnimRainbowAll(int phase, const palette p, CHSV *vals, int param)
{
	int i, s, f;
	if (param%2) phase=256*6-phase-1;
	for (i=0; i<NUM_LEDS; i++)
	{		
		f=i;
		if ((param/2)%2) f=NUM_LEDS-f-1;
		s=(phase+f*256*6/NUM_LEDS)/256;
		f=(phase+f*256*6/NUM_LEDS)%256;
		ColorBlend(p[s%6], p[(s+1)%6], f, vals[i]);
	}
}

void AnimRainbowRadial(int phase, const palette p, CHSV *vals, int param)
{
	int i, s, f;
	if (param%2) phase=256*6-phase-1;
	for (i=0; i<NUM_LEDS; i++)
	{		
		f=led_coord_r[i];
		if ((param/2)%2) f=255-f;
		s=(phase+f*230*6/256)/256;
		f=(phase+f*230*6/256)%256;
		ColorBlend(p[s%6], p[(s+1)%6], f, vals[i]);
	}
}

void AnimRainbowRadialMix(int phase, const palette p, CHSV *vals, int param)
{
	int i, s, f;
	if (param%2) phase=256*6-phase-1;
	for (i=0; i<NUM_LEDS; i++)
	{		
		f=led_coord_r[i];
		if (((led_coord_a[i]+20)/43)%2) f=255-f;
		s=(phase*2+f*230*6/256)/256;
		f=(phase*2+f*230*6/256)%256;
		ColorBlend(p[s%6], p[(s+1)%6], f, vals[i]);
	}
}

void AnimRainbowClock(int phase, const palette p, CHSV *vals, int param)
{
	int i, s, f;
	if (param%2) phase=256*6-phase-1;
	for (i=0; i<NUM_LEDS; i++)
	{		
		f=led_coord_a[i];
		if ((param/8)%2 && led_coord_br[i]<30) f=255-f;
		if ((param/2)%2) f=255-f;
		s=(phase+f*6)/256;
		f=(phase+f*6)%256;
		ColorBlend(p[s%6], p[(s+1)%6], f, vals[i]);
		if ((param/4)%2 && led_coord_br[i]<30) { vals[i][2]=param/7%5; vals[i][1]/=param/11%3+1; }
	}
}

void AnimRainbowSpiral(int phase, const palette p, CHSV *vals, int param)
{
	int i, s, f;
	if (param%2) phase=256*6-phase-1;
	for (i=0; i<NUM_LEDS; i++)
	{		
		f=(led_coord_a[i]+led_coord_r[i]/2)%256;
		if ((param/2)%2) f=255-f;
		s=(phase+f*6)/256;
		f=(phase+f*6)%256;
		ColorBlend(p[s%6], p[(s+1)%6], f, vals[i]);
	}
}

void AnimRainbowTree(int phase, const palette p, CHSV *vals, int param)
{
	int i, s, f;
	if (param%2) phase=256*6-phase-1;
	for (i=0; i<NUM_LEDS; i++)
	{		
		f=led_coord_br[i];
		if ((param/2)%2) f=255-f;
		s=(phase+f*6)/256;
		f=(phase+f*6)%256;
		ColorBlend(p[s%6], p[(s+1)%6], f, vals[i]);
	}
}

void AnimFlash(int phase, const palette p, CHSV *vals, int param)
{
	int i, l, c;
	for (i=0; i<NUM_LEDS; i++)
	{
		c=vals[i][2];
		c-=7;
		if (c<0) c=0;
		vals[i][2]=c;
	}
	for (i=0; i<3; i++)
	{
		c=rand()%6;
		l=rand()%NUM_LEDS;
		vals[l][0]=p[c][0];
		vals[l][1]=p[c][1];
		vals[l][2]=p[c][2];
	}
}

void AnimWaveRadial(int phase, const palette p, CHSV *vals, int param)
{
	int i, s, f, s1, f1;
	int width=30+param%80;
	if (param%2) phase=256*6-phase-1;
	//phase=phase*2%(256*6);
	for (i=0; i<NUM_LEDS; i++)
	{		
		f=led_coord_r[i];
		if ((param/16)%2) f=(f*2)%256;
		if ((param/8)%2) f=(f+led_coord_a[i])%256;
		if ((param/2)%2) f=255-f;
		s=phase/6;
		if ((param/32)%3 == 0) 
		{
			s=phase/3;
			if (s>255) s=511-s;
		}
		s=s-f;
		if (s < 0) s=-s;
		if (s>128) s=255-s;
		if (s>width) { vals[i][2]=0; }
	  else
		{
			f=(led_coord_a[i]+param%253)%256;
			if (f>128) f=256-f;
		  s1=(led_coord_r[i]*6)/256;
		  f1=(led_coord_r[i]*6)%256;
		  ColorBlend(p[s1%6], p[(s1+1)%6], f1, vals[i]);
			vals[i][2]=(width-s)*255/width;
			Gamma(&vals[i]);
			vals[i][0]=vals[i][0] + (uint8_t)((f-64)*(param%35)/128);
		}
	}
}

void AnimWaveRadialMono(int phase, const palette p, CHSV *vals, int param)
{
	int i, s, f, s1, f1;
	int width=30+param%80;
	if (param%2) phase=256*6-phase-1;
	//phase=phase*2%(256*6);
	for (i=0; i<NUM_LEDS; i++)
	{		
		f=led_coord_r[i];
		if ((param/16)%2) f=(f*2)%256;
		if ((param/8)%2) f=(f+led_coord_a[i])%256;
		if ((param/2)%2) f=255-f;
		s=phase/6;
		if ((param/32)%3 == 0) 
		{
			s=phase/3;
			if (s>255) s=511-s;
		}
		s=s-f;
		if (s < 0) s=-s;
		if (s>128) s=255-s;
		if (s>width) { vals[i][2]=0; }
	  else
		{
			f=(led_coord_a[i]+param%253)%256;
			if (f>128) f=256-f;
			f1=led_coord_a[i];
			if (param/4%2) f1+=phase/4;
		  s1=(f1*6)/256;
		  f1=(f1*6)%256;
		  ColorBlend(p[s1%6], p[(s1+1)%6], f1, vals[i]);
			vals[i][2]=(width-s)*255/width;
			Gamma(&vals[i]);
			vals[i][0]=vals[i][0] + (uint8_t)((f-64)*(param%35)/128);
		}
	}
}

void AnimFlag(int phase, const palette p, CHSV *vals, int param)
{
	int i, s, f;
	const int wave[32]={0, 2, 4, 5, 6, 7, 7, 8, 8, 8, 7, 7, 6, 5, 4, 2, 0, -2, -4, -5, -6, -7, -7, -8, -8, -8, -7, -7, -6, -5, -4, -2};
	//p=russia;
	if (p == russia) param=0;
	for (i=0; i<NUM_LEDS; i++)
	{		
		f=                 (int)led_coord_y[i] + wave[(led_coord_x[i]/16+phase/48) % 32]*3;
		if ((param/4)%2) f=(int)led_coord_x[i] + wave[(led_coord_y[i]/16+phase/48) % 32]*3;
		if (f<0) f=0;
		if (f>255) f=255;
		if ((param/2)%2) f=255-f;
		s=(f*6)/256;
		//f=(f*5)%256;
		f=0;
		ColorBlend(p[s%6], p[(s+1)%6], f, vals[i]);
	}
}

void AnimRainbowDecart(int phase, const palette p, CHSV *vals, int param)
{
	int i, s, f;
	//if (param%2) phase=256*6-phase-1;
	for (i=0; i<NUM_LEDS; i++)
	{		
		f=led_coord_y[i];
		if ((param/4)%8 == 1) f=led_coord_x[i];
		if ((param/4)%8 == 2) f=(led_coord_x[i]+led_coord_y[i]) % 256;
		if ((param/4)%8 == 3) f=(led_coord_x[i]+256-led_coord_y[i]) % 256;
		if ((param/4)%8 == 4) f=(led_coord_x[i]+ (led_coord_y[i]>=128 ? led_coord_y[i]-128 : 128-led_coord_y[i])) % 256;
		if ((param/4)%8 == 5) f=(led_coord_x[i]+ (led_coord_y[i]<=128 ? led_coord_y[i] : 255-led_coord_y[i])) % 256;
		if ((param/4)%8 == 6) f=(led_coord_y[i]+ (led_coord_x[i]>=128 ? led_coord_x[i]-128 : 128-led_coord_x[i])) % 256;
		if ((param/4)%8 == 7) f=(led_coord_y[i]+ (led_coord_x[i]<=128 ? led_coord_x[i] : 255-led_coord_x[i])) % 256;
		if ((param/2)%2) f=255-f;
		s=(phase+f*6)/256;
		f=(phase+f*6)%256;
		ColorBlend(p[s%6], p[(s+1)%6], f, vals[i]);
	}
}

void AnimMonochrome(int phase, const palette p, CHSV *vals, int param)
{
	int i, s, f;
	if (param%2) phase=256*6-phase-1;
	s=(phase+256*6)/256;
	f=(phase+256*6)%256;
	for (i=0; i<NUM_LEDS; i++)
	{		
		ColorBlend(p[s%6], p[(s+1)%6], f, vals[i]);
	}
}

void AnimSpotlight(int phase, const palette p, CHSV *vals, int param)
{
	int a, i, s, f, f2, c;

	a=phase/6;
	if (param%2) a=255-a;
	for (i=0; i<NUM_LEDS; i++)
	{
		s=led_coord_a[i]-a;
		if (s<0) s=-s;
		if (s>128) s=256-s;
		f=led_coord_r[i];

		f=(f+(4*(s-64)))*4;
		if (f<0) f=-f;
		if (f>255) f=255;

		c=f;
		if ((param/2)%4 == 1) c=led_coord_a[i];
		if ((param/2)%4 == 2) c=led_coord_r[i];
		if ((param/2)%4 == 3) c=led_coord_r[i]+led_coord_a[i];
		s=(c*6)/256;
		f2=(c*6)%256;
		ColorBlend(p[s%6], p[(s+1)%6], f2, vals[i]);
		vals[i].s=255;
		if (f==0 || f==255) vals[i][2]=0;
	}
}

void AnimArrows(int phase, const palette p, CHSV *vals, int param)
{
	const int border=170;
	int i, s, f;
	if (param%2) phase=256*6-phase-1;
	for (i=0; i<NUM_LEDS; i++)
	{
		if ((param/8)%2)
		  f=((led_coord_r[i]-border)*255/(255-border)+phase) % 256;
		else
		  f=(led_coord_a[i]+phase/6) % 256;
		if (f<0) f=0;
		//if ((param/8)%2 && led_coord_br[i]<30) f=255-f;
		if ((param/2)%2) f=255-f;
		s=(f*6)/256;
		f=(f*6)%256;
		vals[i].v=0;
		if (led_coord_r[i]>=border)
		  ColorBlend(p[s%6], p[(s+1)%6], f, vals[i]);
		else
			if ((param/4)%2 && phase%256>led_coord_r[i] && phase%256-led_coord_r[i]<20)
		    ColorBlend(p[((phase%256)*border/255)%6], p[(((phase%256)*border/255)+1)%6], f, vals[i]);
		//if ((param/4)%2 && led_coord_br[i]<30) { vals[i][2]=param/7%5; vals[i][1]/=param/11%3; }
	}
}

#define rsqr (50*50)
void colorbycoord(int x1, int y1, int x2, int y2, int param, const palette p, CHSV &c)
{
	int d, s, f;
	d=(x1-x2)*(x1-x2) + (y1-y2)*(y1-y2);
	if (d>rsqr) { c.v=0; c.s=255; }
	else
	{
		f=d*256/rsqr;
		if (param%2) f=(param/2)%6;
		s=(f*6)/256;
		f=(f*6)%256;
	  ColorBlend(p[s%6], p[(s+1)%6], f, c);
	}
}

void AnimSpotlightMix(int phase, const palette p, CHSV *vals, int param)
{
	int x1, x2, y1, y2, x, y, i;
	CHSV c1, c2;

	x1=(param)%2;
	x2=(param/2)%2;
	y1=(param/4)%2;
	y2=(param/8)%2;
	x1=x1*2-1;
	x2=x2*2-1;
	y1=y1*2-1;
	y2=y2*2-1;
	x1=((param/3)+256+phase/3*x1) % 512;
	x2=((param/5)+256+phase/3*x2) % 512;
	y1=((param/7)+256+phase/3*y1) % 512;
	y2=((param/8)+256+phase/3*y2) % 512;
	if (x1>=256) x1=511-x1;
	if (x2>=256) x2=511-x2;
	if (y1>=256) y1=511-y1;
	if (y2>=256) y2=511-y2;

	for (i=0; i<NUM_LEDS; i++)
	{
		x=led_coord_x[i];
		y=led_coord_y[i];

		colorbycoord(x, y, x1, y1, param/16, p, c1);
		colorbycoord(x, y, x2, y2, param/96, p, c2);
		ColorBlend(c1, c2, 127, vals[i]);
		if (vals[i].v<128) vals[i].v*=2; else vals[i].v=255;
		//vals[i]=c2;
	}
}

void AnimPlasma(int phase, const palette p, CHSV *vals, int param)
{
	int i, x, y, s, f;
	if ((param/2)%2) phase=256*6-phase-1;
	for (i=0; i<NUM_LEDS; i++)
	{
		x=led_coord_x[i];
		y=led_coord_y[i];

		int color = int
		(
			  128.0 + (128.0 * sin(x / 16.0)+param%777)
			+ 128.0 + (128.0 * sin(y / 8.0)+2*Pi*phase/512+param%555)
			+ 128.0 + (128.0 * sin((x + y) / 16.0)+2*Pi*phase/256)
			+ 128.0 + (128.0 * sin(sqrt(double(x * x + y * y)) / 8.0))
		) / 4;
		color=(color+phase/6) %256;
		if (param%2) color=255-color;
		s=(color*6)/256;
		f=(color*6)%256;
		ColorBlend(p[s%6], p[(s+1)%6], f, vals[i]);
	}
}

#define EFFECTS 16
void Anim(int effect, int phase, const palette p, CHSV *vals, int param)
{
	switch (effect)
	{
		case  0: AnimRainbowAll(phase, p, vals, param); break;
		case  1: AnimRainbowRadial(phase, p, vals, param); break;
		case  2: AnimRainbowClock(phase, p, vals, param); break;
		case  3: AnimRainbowRadialMix(phase, p, vals, param); break;
		case  4: AnimFlash(phase, p, vals, param); break;
		case  5: AnimWaveRadial(phase, p, vals, param); break;
		case  6: AnimRainbowSpiral(phase, p, vals, param); break;
		case  7: AnimRainbowTree(phase, p, vals, param); break;
		case  8: AnimFlag(phase, p, vals, param); break;
		case  9: AnimRainbowDecart(phase, p, vals, param); break;
		case 10: AnimMonochrome(phase, p, vals, param); break;
		case 11: AnimWaveRadialMono(phase, p, vals, param); break;
		case 12: AnimSpotlight(phase, p, vals, param); break;
		case 13: AnimArrows(phase, p, vals, param); break;
		case 14: AnimSpotlightMix(phase, p, vals, param); break;
		case 15: AnimPlasma(phase, p, vals, param); break;

		case 255: AnimColorTest(phase, p, vals, param); break;
	}
}

CHSV frame[2][NUM_LEDS];

int32_t i, n, f;
uint32_t last, st, loop_start, effect_start1, effect_start2;
int effect1, effect2, effect1_param, effect2_param;
const palette *p1, *p2;

void setup()
{
	pinMode(LED, OUTPUT);
	FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
	FastLED.setBrightness( BRIGHTNESS );
	//FastLED.setDither(DISABLE_DITHER);
	randomSeed(analogRead(0));
	Serial.begin(115200);
	n=0;
	f=0;
	last=0;
	loop_start=0;
	effect_start1=0;
	effect_start2=0;
	effect1_param=0;
	effect2_param=0;
	effect1=15;
	effect2=2;
	p1=p2=&rainbow;

	for (int i=0; i<NUM_LEDS; i++)
	{
		led_coord_x[i]= round(sin(2*Pi*led_coord_a[i]/256)*led_coord_r[i]/2)+127;
		led_coord_y[i]=-round(cos(2*Pi*led_coord_a[i]/256)*led_coord_r[i]/2)+127;
	}

	Serial.println("Start");
}

void loop(void)
{
	// for (i=0; i<NUM_LEDS; i++)
	// {
	//   CHSV hsv;
	//   hsv.h=i*5;
	//   hsv.s=255;
	//   hsv.v=255;
	//   hsv2rgb_rainbow(hsv, leds[i]);
	// }
	FastLED.show(); // display this frame
	delay(1000 / FRAMES_PER_SECOND);  
	
	st=millis();
	while (st-loop_start >= time_loop) { loop_start+=time_loop; f++;}
	n=((st-loop_start)*256*6)/time_loop;
	
	if (st-effect_start1 >= time_effect+time_fade && effect_start2==0)
	{ // fade start
		do
		{
			switch (rand()%13)
			{
				case  0: p2=&rainbow; break;
				case  1: p2=&lightblue; break;
				case  2: p2=&darkblue; break;
				case  3: p2=&fire; break;
				case  4: p2=&green_flash; break;
				case  5: p2=&ryg_flash; break;
				case  6: p2=&pastel; break;
				case  7: p2=&blueyellow; break;
				case  8: p2=&cocacola; break;
				case  9: p2=&russia; break;
				case 10: p2=&chrome; break;
				case 11: p2=&green_fade; break;
				case 12: p2=&ryg_fade; break;
				
			}
		}	while (p1==p2);
//p2=&russia;
		do
		{
			effect2=rand() % EFFECTS;
		}	while (effect1==effect2);
//effect2=15;
		if (effect2 == 8 && (rand()%3) == 0) p2=&russia;
		effect2_param=rand();
		effect_start2=effect_start1+time_effect+time_fade;
	}
	if (st-effect_start2 >= time_fade && effect_start2!=0)
	{ // fade ended
		effect1=effect2;
		p1=p2;
		effect1_param=effect2_param;
		effect_start1=effect_start2;
		for (i=0; i<NUM_LEDS; i++)
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
		for (i=0; i<NUM_LEDS; i++)
		{
			int p=(st-effect_start2)*255 / time_fade;
			CHSV c;
			ColorBlend(frame[0][i], frame[1][i], p, c);
			hsv2rgb_rainbow(c, leds[i]);
		}
	}
	else
	{
		Anim(effect1, n, *p1, frame[0], effect1_param);
		for (i=0; i<NUM_LEDS; i++)
			hsv2rgb_rainbow(frame[0][i], leds[i]);
	}
}
