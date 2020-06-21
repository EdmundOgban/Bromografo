/******************************************************************************/
/*Files to Include                                                            */
/******************************************************************************/

#if defined(__XC)
    #include <xc.h>         /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>        /* HiTech General Include File */
#elif defined(__18CXX)
    #include <p18cxxx.h>    /* C18 General Include File */
#endif

#if defined(__XC) || defined(HI_TECH_C)

#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */

#endif

#include "system.h"

#define OSC_8MHZ 0b110
/* Refer to the device datasheet for information about available
oscillator configurations. */
void ConfigureOscillator(void)
{
    OSCCONbits.IRCF = OSC_8MHZ;
    while (!(OSCCONbits.HFIOFS));
    OSCTUNEbits.PLLEN = 1;
    while (!(OSCCON2bits.HFIOFL));
}
