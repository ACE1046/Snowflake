#ifndef __ACEBMP280_H
#define __ACEBMP280_H

#include <stdint.h>
#include <stdbool.h>

bool InitBMP280(void);
float bmp280_readAltitude(float seaLevelhPa);
uint32_t bmp280_readPressure(void);
int32_t bmp280_readTemperature(void);

#endif
