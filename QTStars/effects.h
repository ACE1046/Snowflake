#ifndef EFFECTS_H
#define EFFECTS_H

#include "hsv2rgb.h"

#define NUM_LEDS    150
//#define NUM_LEDS    138 // mysku

#define Pi 3.14159

typedef CHSV palette[6];
typedef palette TPalette;

#define EFFECTS 16
void Anim(int effect, int phase, const palette p, CHSV *vals, int param);
void ColorBlend(const CHSV c1, const CHSV c2, uint8_t percent, CHSV &res);

const palette *GetPalette(int n);

extern const uint8_t led_coord_r[NUM_LEDS];
extern const uint8_t led_coord_a[NUM_LEDS];
extern const uint8_t led_coord_br[NUM_LEDS];

extern uint8_t led_coord_x[NUM_LEDS];
extern uint8_t led_coord_y[NUM_LEDS];


/*// ============== mysku ================
extern uint8_t led_coord_r[NUM_LEDS];
extern uint8_t led_coord_a[NUM_LEDS];
extern const uint8_t led_coord_br[NUM_LEDS];

extern const uint8_t led_coord_x[NUM_LEDS];
extern const uint8_t led_coord_y[NUM_LEDS];
*/

#endif // EFFECTS_H
