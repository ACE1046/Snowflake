#ifndef __ACESPI_H
#define __ACESPI_H

void InitSPI(void);
uint8_t SpiSend(uint8_t b);
void SpiStart(void);
void SpiStop(void);

#endif
