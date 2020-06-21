/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#if defined(__XC)
    #include <xc.h>        /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>       /* HiTech General Include File */
#elif defined(__18CXX)
    #include <p18cxxx.h>   /* C18 General Include File */
#endif

#if defined(__XC) || defined(HI_TECH_C)

#include <stdio.h>
#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */

#endif

#include "system.h"        /* System funct/params, like osc/peripheral config */
#include "user.h"          /* User funct/params, such as InitApp */

#include "hal.h"
#include "cqueue.h"
#include "lcd.h"
#include "swtimers.h"
#include "fsm.h"
#include "macros.h"
#include "pins.h"
#include "strings.h"
#include "main.h"

/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/

              // m     s     m     s     m     s   preheat m     s
__EEPROM_DATA(   1,   30,    5,    0,    0,    0,          1,    0);

struct swtimers_s swtimers;
struct fsm_s fsm;
cQ_s inh_q;

uint8_t * menus[MENU_ELEMENTS_CNT] = { "Lucido", "Carta", "Manuale" };

struct clock_s clocks[MENU_ELEMENTS_CNT] = { { 1, 30 },
                                             { 3, 50 },
                                             { 0,  0 } };

uint8_t buf[LCD_LEN+1];
volatile struct globaldata_s gdata = {
/*  .cur_clock       = */ NULL,
/*  .menu_elms       = */ menus,
/*  .buf             = */ buf,
/*  .menu_pos        = */ 0,
/*  .cur_pos         = */ 0,
/*  .clock_edit      = */ 0
};

extern void fsm_enter(void);

/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/

void main(void)
{
    /* Configure the oscillator for the device */
    ConfigureOscillator();
    /* Initialize I/O and Peripherals for application */
    peripheral_init();
    lcd_init();
    fsm_init(&fsm);
    queue_init(&inh_q, true);
    lcd_puts("Starting up...");
    delay_msecs(2000);

    fsm_enter();

    enable_interrupts(INT_TIMER0);
    enable_interrupts(PERIPHERALS);
    enable_interrupts(GLOBAL);

    for (;;) {
        fsm_run(&fsm);
        fsm_signal_handler(&fsm);
        swtimer_call(&swtimers);
    }
}
