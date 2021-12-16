#ifndef __INC_HSV2RGB_H
#define __INC_HSV2RGB_H

#include <stdint.h>

/// Representation of an HSV pixel (hue, saturation, value (aka brightness)).
struct CHSV {
    union {
                struct {
                    union {
                        uint8_t hue;
                        uint8_t h; };
                    union {
                        uint8_t saturation;
                        uint8_t sat;
                        uint8_t s; };
                    union {
                        uint8_t value;
                        uint8_t val;
                        uint8_t v; };
                };
                uint8_t raw[3];
        };

    /// Array access operator to index into the chsv object
        inline uint8_t& operator[] (uint8_t x) __attribute__((always_inline))
    {
        return raw[x];
    }

    /// Array access operator to index into the chsv object
    inline const uint8_t& operator[] (uint8_t x) const __attribute__((always_inline))
    {
        return raw[x];
    }

    /// default values are UNITIALIZED
    inline CHSV() __attribute__((always_inline)) = default;

    /// allow construction from H, S, V
    inline CHSV( uint8_t ih, uint8_t is, uint8_t iv) __attribute__((always_inline))
        : h(ih), s(is), v(iv)
    {
    }

    /// allow copy construction
    inline CHSV(const CHSV& rhs) __attribute__((always_inline)) = default;

    inline CHSV& operator= (const CHSV& rhs) __attribute__((always_inline)) = default;

    inline CHSV& setHSV(uint8_t ih, uint8_t is, uint8_t iv) __attribute__((always_inline))
    {
        h = ih;
        s = is;
        v = iv;
        return *this;
    }
};

/// Representation of an RGB pixel (Red, Green, Blue)
struct CRGB {
        union {
                struct {
            union {
                uint8_t r;
                uint8_t red;
            };
            union {
                uint8_t g;
                uint8_t green;
            };
            union {
                uint8_t b;
                uint8_t blue;
            };
        };
                uint8_t raw[3];
        };
};

void hsv2rgb_rainbow( const struct CHSV& hsv, struct CRGB& rgb);
void hsv2rgb_rainbow( const struct CHSV* phsv, struct CRGB * prgb, int numLeds);
void hsv2rgb_spectrum( const CHSV& hsv, CRGB& rgb);
void hsv2rgb_screen(const CHSV& hsv, CRGB& rgb);

#endif
