/***************************************************************************
// 16.01.2017 BMP280 pressure sensor library
// Based on Adafruit library:

***************************************************************************
  This is a library for the BMP280 pressure sensor

  Designed specifically to work with the Adafruit BMP280 Breakout
  ----> http://www.adafruit.com/products/2651

  These sensors use I2C to communicate, 2 pins are required to interface.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit andopen-source hardware by purchasing products
  from Adafruit!

  Written by Kevin Townsend for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ***************************************************************************/

#include "stm32f0xx.h"
#include <stdint.h>
#include "ace_spi.h"
#ifdef ALTITUDE
#include <math.h>
#endif
#include "ace_bmp280.h"

#define BMP280_CHIPID 0x58

enum
{
	BMP280_REGISTER_DIG_T1              = 0x88,
	BMP280_REGISTER_DIG_T2              = 0x8A,
	BMP280_REGISTER_DIG_T3              = 0x8C,

	BMP280_REGISTER_DIG_P1              = 0x8E,
	BMP280_REGISTER_DIG_P2              = 0x90,
	BMP280_REGISTER_DIG_P3              = 0x92,
	BMP280_REGISTER_DIG_P4              = 0x94,
	BMP280_REGISTER_DIG_P5              = 0x96,
	BMP280_REGISTER_DIG_P6              = 0x98,
	BMP280_REGISTER_DIG_P7              = 0x9A,
	BMP280_REGISTER_DIG_P8              = 0x9C,
	BMP280_REGISTER_DIG_P9              = 0x9E,

	BMP280_REGISTER_CHIPID             = 0xD0,
	BMP280_REGISTER_VERSION            = 0xD1,
	BMP280_REGISTER_SOFTRESET          = 0xE0,

	BMP280_REGISTER_CAL26              = 0xE1,  // R calibration stored in 0xE1-0xF0

	BMP280_REGISTER_CONTROL            = 0xF4,
	BMP280_REGISTER_CONFIG             = 0xF5,
	BMP280_REGISTER_PRESSUREDATA       = 0xF7,
	BMP280_REGISTER_TEMPDATA           = 0xFA,
};

typedef struct
{
	uint16_t dig_T1;
	int16_t  dig_T2;
	int16_t  dig_T3;

	uint16_t dig_P1;
	int16_t  dig_P2;
	int16_t  dig_P3;
	int16_t  dig_P4;
	int16_t  dig_P5;
	int16_t  dig_P6;
	int16_t  dig_P7;
	int16_t  dig_P8;
	int16_t  dig_P9;

	uint8_t  dig_H1;
	int16_t  dig_H2;
	uint8_t  dig_H3;
	int16_t  dig_H4;
	int16_t  dig_H5;
	int8_t   dig_H6;

	int32_t t_fine;
} bmp280_calib_data;

bmp280_calib_data bmp280_calib;

void write8(uint8_t reg, uint8_t value)
{
	SpiStart();
  SpiSend(reg & ~0x80); // write, bit 7 low
  SpiSend(value);
  SpiStop();
}

uint8_t read8(uint8_t reg)
{
  uint8_t value;

	SpiStart();
  SpiSend(reg | 0x80); // read, bit 7 high
  value=SpiSend(0xFF);
  SpiStop();
  return value;
}

uint16_t read16(uint8_t reg)
{
  uint16_t value;

	SpiStart();
  SpiSend(reg | 0x80); // read, bit 7 high
  value=((uint16_t)SpiSend(0xFF)) << 8;
	value|=SpiSend(0xFF);
  SpiStop();
  return value;
}

uint16_t read16_LE(uint8_t reg) 
{
  uint16_t temp = read16(reg);
  return (temp >> 8) | (temp << 8);
}

int16_t readS16(uint8_t reg)
{
  return (int16_t)read16(reg);
}

int16_t readS16_LE(uint8_t reg)
{
  return (int16_t)read16_LE(reg);
}

uint32_t read24(uint8_t reg)
{
  uint32_t value;

	SpiStart();
  SpiSend(reg | 0x80); // read, bit 7 high
  value=SpiSend(0xFF);
  value <<= 8;
	value|=SpiSend(0xFF);
  value <<= 8;
	value|=SpiSend(0xFF);
  SpiStop();
  return value;
}

void readCoefficients(void)
{
	bmp280_calib.dig_T1 = read16_LE(BMP280_REGISTER_DIG_T1);
	bmp280_calib.dig_T2 = readS16_LE(BMP280_REGISTER_DIG_T2);
	bmp280_calib.dig_T3 = readS16_LE(BMP280_REGISTER_DIG_T3);

	bmp280_calib.dig_P1 = read16_LE(BMP280_REGISTER_DIG_P1);
	bmp280_calib.dig_P2 = readS16_LE(BMP280_REGISTER_DIG_P2);
	bmp280_calib.dig_P3 = readS16_LE(BMP280_REGISTER_DIG_P3);
	bmp280_calib.dig_P4 = readS16_LE(BMP280_REGISTER_DIG_P4);
	bmp280_calib.dig_P5 = readS16_LE(BMP280_REGISTER_DIG_P5);
	bmp280_calib.dig_P6 = readS16_LE(BMP280_REGISTER_DIG_P6);
	bmp280_calib.dig_P7 = readS16_LE(BMP280_REGISTER_DIG_P7);
	bmp280_calib.dig_P8 = readS16_LE(BMP280_REGISTER_DIG_P8);
	bmp280_calib.dig_P9 = readS16_LE(BMP280_REGISTER_DIG_P9);
}

// Returns temperature in DegC, resolution is 0.01 DegC. Output value of "5123" equals 51.23 DegC
int32_t bmp280_readTemperature(void)
{
  int32_t var1, var2, T;

  int32_t adc_T = read24(BMP280_REGISTER_TEMPDATA);
  adc_T >>= 4;

	var1 = ((((adc_T >> 3) - ((int32_t)bmp280_calib.dig_T1 << 1))) * ((int32_t)bmp280_calib.dig_T2)) >> 11; 
	var2  = (((((adc_T >> 4) - ((int32_t)bmp280_calib.dig_T1)) * ((adc_T >> 4) - ((int32_t)bmp280_calib.dig_T1))) >> 12) * ((int32_t)bmp280_calib.dig_T3)) >> 14; 
	bmp280_calib.t_fine = var1 + var2; 
	T = (bmp280_calib.t_fine * 5 + 128) >> 8;
  return T;
}

// Returns pressure in Pa as unsigned 32 bit integer in Q24.8 format (24 integer bits and 8 fractional bits). 
// Output value of "24674867" represents 24674867/256 = 96386.2 Pa = 963.862 hPa
uint32_t bmp280_readPressure(void) 
{
  int64_t var1, var2, p;
	int32_t adc_P;

  // Must be done first to get the t_fine variable set up
  bmp280_readTemperature();

  adc_P = read24(BMP280_REGISTER_PRESSUREDATA);
  adc_P >>= 4;

  var1 = ((int64_t)bmp280_calib.t_fine) - 128000;
  var2 = var1 * var1 * (int64_t)bmp280_calib.dig_P6;
  var2 = var2 + ((var1*(int64_t)bmp280_calib.dig_P5)<<17);
  var2 = var2 + (((int64_t)bmp280_calib.dig_P4)<<35);
  var1 = ((var1 * var1 * (int64_t)bmp280_calib.dig_P3)>>8) +
    ((var1 * (int64_t)bmp280_calib.dig_P2)<<12);
  var1 = (((((int64_t)1)<<47)+var1))*((int64_t)bmp280_calib.dig_P1)>>33;

  if (var1 == 0) {
    return 0;  // avoid exception caused by division by zero
  }
  p = 1048576 - adc_P;
  p = (((p<<31) - var2)*3125) / var1;
  var1 = (((int64_t)bmp280_calib.dig_P9) * (p>>13) * (p>>13)) >> 25;
  var2 = (((int64_t)bmp280_calib.dig_P8) * p) >> 19;

  p = ((p + var1 + var2) >> 8) + (((int64_t)bmp280_calib.dig_P7)<<4);
  return (uint32_t)p;
}

#ifdef ALTITUDE
float bmp280_readAltitude(float seaLevelhPa) 
{
  float altitude;

  float pressure = bmp280_readPressure(); // in Si units for Pascal
  pressure /= 100;

  altitude = 44330 * (1.0 - pow(pressure / seaLevelhPa, 0.1903));

  return altitude;
}
#endif

bool InitBMP280(void)
{
	InitSPI();
  if (read8(BMP280_REGISTER_CHIPID) != BMP280_CHIPID) return false;

  readCoefficients();
  write8(BMP280_REGISTER_CONTROL, 0x57); // x16 - p, x2 - t
  write8(BMP280_REGISTER_CONFIG, 0x10); // t_sb=000, filter=100 (16)
  return true;
}	

