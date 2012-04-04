/*
 * File:   main.c
 * Author: robj
 *
 * Created on June 22, 2011, 4:15 AM
 */

#include <p24F16KA102.h>


_FWDT	(FWDTEN_OFF);
_FOSCSEL(FNOSC_FRCPLL );
_FOSC   ( POSCFREQ_HS & POSCMOD_HS & OSCIOFNC_ON );


int main(void) {
    for(;;);
}
