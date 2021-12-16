/*
 * ws281x.h - init & comm api for the ws281x PWM RGB LED
 * 04-01-15 E. Brombaugh
 */

#ifndef __ws281x__
#define __ws281x__

#include "stm32f0xx.h"

#define WS_MAX_LEDS 31

typedef uint8_t hsv[3];

void ws281x_init(void);
void ws281x_send(uint8_t rgb_data[], uint8_t num_leds);
void ws281x_hsv2rgb(const hsv HSV, uint8_t rgb[]);

#endif

