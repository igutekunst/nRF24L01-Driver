#include <p24F16KA102.h>
#include "uart.h"


_FWDT	(FWDTEN_OFF);
_FOSCSEL(FNOSC_FRCPLL );
_FOSC   ( POSCFREQ_HS & POSCMOD_HS & OSCIOFNC_ON );
#define FCY 12000000


int main(void) {
    UART1HSInit(103);
    for(;;){
      UART1PutChar('c');
    }
}
