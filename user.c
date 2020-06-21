/******************************************************************************/
/* Files to Include                                                           */
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

#include "user.h"

/******************************************************************************/
/* User Functions                                                             */
/******************************************************************************/

/* <Initialize variables in user.h and insert code for user algorithms.> */

void peripheral_init(void)
{
    TRISA  = TRISA_CONF;
    TRISB  = TRISB_CONF;
    TRISC  = TRISC_CONF_O;

    // Enable weak pull-ups
    //WPUA   = 0x0F;
    WPUA   = 0x0F;
    WPUB   = 0x80;
    RABPU  = 0;

    // Disable analog inputs for all ports
    ANSEL  = 0;
    ANSELH = 0;

    PORTB  = 0;
    PORTC  = 0;

    T0CON  = (T0_ENABLE | T0_8_BIT | T0_DIV_32);
    T1CON  = (T1_ENABLE | T1_RD16  | T1_DIV_1 | T1_OSC_ENABLE | T1_SYNC | T1_CLKSRC_EXT);

}
