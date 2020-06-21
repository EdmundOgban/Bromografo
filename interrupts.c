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

#include <stdio.h>
#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */

#endif

#include "hal.h"
#include "macros.h"
#include "pins.h"
#include "lcd.h"
#include "swtimers.h"
#include "fsm.h"
#include "main.h"
#include "interrupts.h"

extern struct fsm_s fsm;
extern struct swtimers_s swtimers;
extern struct swt_handle_s tmr_button_hold;
extern struct swt_handle_s tmr_debouncing;
extern struct globaldata_s gdata;

/******************************************************************************/
/* Per-interrupt Interrupt Routines                                           */
/******************************************************************************/


void timer0_isr(void)
{
    set_timer0(T0_CALIB);

    swtimers.tc++;
    swtimer_scan_for_match(&swtimers);
}

void timer1_isr(void)
{
    set_timer1(T1_CALIB);

    if (gdata.cur_clock->mins == 0 && gdata.cur_clock->secs == 0) {
        fsm_emit_signal(&fsm, RTC_CNT_END);
    }
    else if (gdata.cur_clock->secs == 0) {
        gdata.cur_clock->mins--;
        gdata.cur_clock->secs = 59;
    }
    else {
        gdata.cur_clock->secs--;
    }

}

/*
void iocab_isr(void)
{
    CHECK_FOR_INPUT(INP_GET_BTN_NEXT_PIN() , inputs.btn_next)
    CHECK_FOR_INPUT(INP_GET_BTN_PREV_PIN() , inputs.btn_prev)
    CHECK_FOR_INPUT(INP_GET_BTN_ENTER_PIN(), inputs.btn_enter)

    CHECK_FOR_INPUT_EMIT(INP_GET_DOOR_PIN(), inputs.door_state,
                                 DOOR_OPEN , DOOR_CLOSE)
    CHECK_FOR_INPUT_EMIT(INP_GET_VACM_PIN(), inputs.vacuum_present,
                                 VACUUM_OK , VACUUM_NOK)

    swtimer_enable(&swtimers, &tmr_debouncing);

    RABIF = 0;

    disable_interrupts(IOC_RA0);
    disable_interrupts(IOC_RA1);
    disable_interrupts(IOC_RA2);
    disable_interrupts(IOC_RA3);
    disable_interrupts(IOC_RB7);
}
*/

/******************************************************************************/
/* Main Interrupt Routines                                                    */
/******************************************************************************/

/* High-priority service */

#if defined(__XC) || defined(HI_TECH_C)
void interrupt high_isr(void)
#elif defined (__18CXX)
#pragma code high_isr=0x08
#pragma interrupt high_isr
void high_isr(void)
#else
#error "Invalid compiler selection for implemented ISR routines"
#endif

{
    if (TMR1IF && TMR1IE) {
        timer1_isr();
        clear_interrupt(INT_TIMER1);
    }
/*
    else if (RABIF && RABIE) {
        iocab_isr();
        clear_interrupt(IOC_PORTS);
    }
*/
    else if (TMR0IF && TMR0IE) {
        timer0_isr();
        clear_interrupt(INT_TIMER0);
    }
}

/* Low-priority interrupt routine */
#if defined(__XC) || defined(HI_TECH_C)
void low_priority interrupt low_isr(void)
#elif defined (__18CXX)
#pragma code low_isr=0x18
#pragma interruptlow low_isr
void low_isr(void)
#else
#error "Invalid compiler selection for implemented ISR routines"
#endif
{

      /* This code stub shows general interrupt handling.  Note that these
      conditional statements are not handled within 3 seperate if blocks.
      Do not use a seperate if block for each interrupt flag to avoid run
      time errors. */

#if 0

      /* Determine which flag generated the interrupt */
      if(<Interrupt Flag 1>)
      {
          <Interrupt Flag 1=0>; /* Clear Interrupt Flag 1 */
      }
      else if (<Interrupt Flag 2>)
      {
          <Interrupt Flag 2=0>; /* Clear Interrupt Flag 2 */
      }
      else
      {
          /* Unhandled interrupts */
      }

#endif

}
