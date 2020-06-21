/******************************************************************************/
/* System Level #define Macros                                                */
/******************************************************************************/

#ifndef _SYSTEM_H
#define _SYSTEM_H

/* Microcontroller MIPs (FCY) */
#define SYS_FREQ        32000000L
#define FCY             SYS_FREQ/4
#define _XTAL_FREQ      SYS_FREQ

/******************************************************************************/
/* System Function Prototypes                                                 */
/******************************************************************************/

/* Custom oscillator configuration funtions, reset source evaluation
functions, and other non-peripheral microcontroller initialization functions
go here. */

void ConfigureOscillator(void); /* Handles clock switching/osc initialization */

#endif
