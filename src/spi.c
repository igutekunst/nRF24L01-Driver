/*
 * Inits SPI at (1/4) * FCY
 * TODO: Add some options
 * TODO:
 */
#include <p24F16KA102.h>
void spiInit(unsigned char master){
    IFS0bits.SPF1IF = 0; //clear interrupt flag
    IEC0bits.SPI1IE = 0; //Don't enable interrupts
    IPC2bits.SPI1IP = 1; //intterupt priority of 1
    
    SPI1CON1bits.MSTEN = 1;// SPI Master
    
    SPI1CON1bits.SPRE = 0b111; //Secondary presacle 1:1
    SPI1CON1bits.PPRE = 0b10;
    
    SPI1STATbits.SPIROV = 0; //clear overflow flag

    SPI1STATbits.SPIEN = 1; //enable the SPI module

}

/*
 * Blocking function to write a single char to SPI bus
 */
unsigned char spiTransfer(unsigned char c){
    SPI1BUF = c; //write transmit buffer
    while(!SPI1STATbits.SPIRBF);
    return SPI1BUF;
}


