#ifndef SPI_H
#define SPI_H

/*
 * Initialize the SPI device
 */
void spiInit(unsigned char master);
/*
 * Blocking function to write a single char to SPI bus
 */
unsigned char spiTransfer(unsigned char c);

#endif