#include "effects.h"
#include <random>

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
    {30, 255, 205},
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

const uint8_t led_coord_r[NUM_LEDS]={
17, 34, 51, 68, 85, 102, 119, 136, 153, 170, 187, 204, 221, 238, 255,   180, 192, 204, 216, 228,    180, 192, 204, 216, 228,
17, 34, 51, 68, 85, 102, 119, 136, 153, 170, 187, 204, 221, 238, 255,   180, 192, 204, 216, 228,    180, 192, 204, 216, 228,
17, 34, 51, 68, 85, 102, 119, 136, 153, 170, 187, 204, 221, 238, 255,   180, 192, 204, 216, 228,    180, 192, 204, 216, 228,
17, 34, 51, 68, 85, 102, 119, 136, 153, 170, 187, 204, 221, 238, 255,   180, 192, 204, 216, 228,    180, 192, 204, 216, 228,
17, 34, 51, 68, 85, 102, 119, 136, 153, 170, 187, 204, 221, 238, 255,   180, 192, 204, 216, 228,    180, 192, 204, 216, 228,
17, 34, 51, 68, 85, 102, 119, 136, 153, 170, 187, 204, 221, 238, 255,   180, 192, 204, 216, 228,    180, 192, 204, 216, 228
};

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


/*// ============== MYSKU ===================
const uint8_t led_coord_x[NUM_LEDS]={
     18,  21,  24,  28,  31,  33,  38,  42,  47,  54,  57,  59,  60,  62,  65,  72,  76,  79,  84,  88,  92,  94,  95,  96,  97,  98,  99, 100,
    125, 131, 138, 145, 153, 160, 167, 201, 198, 195, 190, 185, 178, 171, 165, 160, 154, 147, 142,
     82,  77,  66,  57,  48,  37,  27,  21,  19,  23,  32,  43,  54,  63,  72,  79,  84,  85,  85,  81,  73,  61,  48,  38,  32,  23,  18,
     98,  98,  98,  98,  98,  98,  98,  98,  98,  98,  98,  98,  98, 144, 138, 132, 128, 120, 115, 110, 104, 108, 113, 119, 125, 131, 136, 142, 147, 153,
    162, 162, 162, 162, 163, 163,164, 164, 166, 167, 171, 175, 180, 186, 195, 204, 211, 219, 227, 232, 235, 237, 238, 238, 238, 238, 238, 237, 238, 237, 237, 236, 236, 235
};
const uint8_t led_coord_y[NUM_LEDS]={
    104,  94,  83,  76,  66,  57,  46,  37,  27,  16,  28,  38,  48,  59,  69,  58,  49,  39,  28,  16,  29,  40,  51,  60,  73,  84,  95, 106,
     17,  27,  36,  45,  52,  60,  67,  20,  29,  38,  49,  59,  68,  78,  89,  98, 107, 107,  98,
    135, 129, 125, 125, 128, 134, 143, 153, 165, 173, 177, 178, 179, 179, 183, 186, 194, 202, 212, 222, 232, 235, 235, 232, 231, 225, 220,
    124, 134, 143, 151, 160, 169, 179, 186, 196, 207, 216, 224, 233, 125, 133, 141, 149, 159, 167, 172, 178, 184, 189, 193, 200, 206, 213, 219, 224, 232,
    125, 133, 144, 150, 158, 168, 178, 186, 197, 207, 217, 222, 230, 235, 234, 235, 235, 232, 229, 223, 217, 209, 203, 196, 188, 180, 173, 163, 157, 148, 141, 133, 126, 122
};
uint8_t led_coord_r[NUM_LEDS];
uint8_t led_coord_a[NUM_LEDS];
const uint8_t led_coord_br[NUM_LEDS]={
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
};
*/
// ========================================

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

const palette *GetPalette(int n)
{
  switch(n)
  {
    case  0: return &rainbow; break;
    case  1: return &lightblue; break;
    case  2: return &darkblue; break;
    case  3: return &fire; break;
    case  4: return &green_flash; break;
    case  5: return &ryg_flash; break;
    case  6: return &pastel; break;
    case  7: return &blueyellow; break;
    case  8: return &cocacola; break;
    case  9: return &russia; break;
    case 10: return &chrome; break;
    case 11: return &green_fade; break;
    case 12: return &ryg_fade; break;
    default: return &rainbow;
  }
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
