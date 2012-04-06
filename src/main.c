#include <p24F16KA102.h>
#include <stdio.h>
#define FCY (5620000)

#define BAUDRATE (9600)
#define BRG ( (FCY/4*BAUDRATE) ) -1 )
#include "uart.h"


_FOSC	(FCKSM_CSDCMD & OSCIOFNC_OFF & POSCMOD_EC);
_FWDT	(FWDTEN_OFF);
_FOSCSEL(FNOSC_FRCPLL);

long readProgMem(long addr){
  int addrOffset;
  int high;
  int low;
  TBLPAG = ((addr & 0x7F0000)>>16);
  addrOffset = (addr & 0x00FFFF);
  asm("tblrdh.w [%1], %0" : "=r"(high) : "r"(addrOffset));
  asm("tblrdl.w [%1], %0" : "=r"(low) : "r"(addrOffset));

  return (high <<16) | low;
}
int main(void) {
  AD1PCFG = 0xFFFF;
  OSCCONbits.COSC = 0b001;
  OSCCONbits.CLKLOCK = 1;
  TRISB = 0x404;  
  TRISA = 0;
  UART1HSInit(152);  //9600
  for(;;){
    //char c = UART1GetChar();
    char c = 0x55;

    UART1PutChar(c);
  }
}
