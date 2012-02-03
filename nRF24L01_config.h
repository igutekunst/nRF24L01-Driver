#ifndef NRF_CONFIG_H
#define NRF_CONFIG_H


//PIC24F16KA102 specific stuff
#define NRF_CSN_LAT     (LATBbits.LATB9)
#define NRF_CSN_PORT    (PORTBbits.RB9)
#define NRF_CSN_TRIS    (TRISBbits.TRISB9)

#define NRF_CE_LAT      (LATAbits.LATA7)
#define NRF_CE_PORT     (PORTAbits.RA7)
#define NRF_CE_TRIS     (TRISAbits.TRISA7)

//Macro to set CSN pin direction to output
#define NRF_CSN_SET_OUTPUT()  NRF_CSN_TRIS = 0 
//Macro to set CE pin direction to output
#define NRF_CE_SET_OUTPUT()   NRF_CE_TRIS  = 0

//Macro to set CSN pin to logic HIGH
#define NRF_CSN_HIGH()        NRF_CSN_LAT  = 1
//Macro to set CSN pin to logic LOW
#define NRF_CSN_LOW()         NRF_CSN_LAT  = 0
//Macro to set CE pin to logic HIGH
#define NRF_CE_HIGH()         NRF_CE_LAT   = 1
//Macro to set CE pin to logic LOW
#define NRF_CE_LOW()          NRF_CE_LAT   = 0


/*
 * Update this macro to a function that writes 
 * one byte to the SPI bus without twiddling
 * an CS pins (modify above macros to do so)
 */
#define NRF_SPI_TRANSFER(byte) spiTransfer(byte)

#endif
